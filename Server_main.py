import socket

def server():
    soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    soc.bind(("127.0.0.1", 2600))
    soc.listen(1)
    print("Server is ready and listening")
    while True:
        print("waiting for connection")
        (clientsocket, addressguest) = soc.accept()
        print(f"Connection from {addressguest}")
        clientsocket.send(b"Hello there !")
        print(clientsocket.recv(16000))
        clientsocket.close()
    soc.close()
    return
server()