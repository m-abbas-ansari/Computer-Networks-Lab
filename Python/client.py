import time, socket, sys
from time import ctime

print("\nWelcome to Chat Room\n")
print("Initialising....\n")
time.sleep(1)

s = socket.socket()
shost = socket.gethostname()
ip = socket.gethostbyname(shost)
print(shost, "(", ip, ")\n")
host = input(str("Enter server address: "))
name = input(str("\nEnter your name: "))
port = 1234
print("\nTrying to connect to ", host, "(", port, ")\n")
time.sleep(1)
s.connect((host, port))
print("Connected...\n")

s.send(name.encode())
s_name = s.recv(1024)
s_name = s_name.decode()
print(s_name, "has joined the chat room\nEnter [e] to exit chat room\n")

while True:
    message = s.recv(1024)
    receive_time = s.recv(1024)
    message = message.decode()
    receive_time = receive_time.decode()
    print("\n", s_name, ":", message)
    print("Received at: ", receive_time)
    message = input(str("\nMe : "))
    send_time = str(ctime()[11:19])
    print("Sent at: ", send_time)
    if message == "[e]":
        message = "Left chat room!"
        s.send(message.encode())
        s.send(send_time.encode())
        print("\n")
        break
    s.send(message.encode())
    s.send(send_time.encode())