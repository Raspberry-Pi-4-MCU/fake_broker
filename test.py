import socket
import time
HOST = '127.0.0.1'
PORT = 1843
clientMessage = "{ \"header\": 1, \"topic_id\": 5, \"body\": \"2.1\"}"
clientMessage2 = "{ \"header\": 2, \"topic_id\": 5, \"body\": \"3.1\"}"
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((HOST, PORT))
client.sendall(clientMessage.encode())

while True:
    time.sleep(1)
    client.sendall(clientMessage2.encode())
    serverMessage = str(client.recv(1024), encoding='utf-8')
    print('Server:', serverMessage)
    time.sleep(5)
client.close()