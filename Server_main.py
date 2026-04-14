import socket

def server():
    soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    soc.bind(("127.0.0.1", 2600))
    soc.listen(1)
    x = 0
    while x != 5:
        (clientsocket, addressguest) = soc.accept()
        print(f"Connection from {addressguest}")
        clientsocket.send(b"Hello there !")
        print(clientsocket.recv(16000))
        clientsocket.close()
        x+=1
    soc.close()
    return