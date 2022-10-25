import time, socket, sys
from time import ctime

print("\nWelcome to Chat Room\n")
print("Initialising....\n")
time.sleep(1)

s = socket.socket()
host = socket.gethostname()
ip = socket.gethostbyname(host)
port = 1234
s.bind((host, port))
print(host, "(", ip, ")\n")
name = input(str("Enter your name: "))

s.listen(1)
print("\nWaiting for incoming connections...\n")
conn, addr = s.accept()
print("Received connection from ", addr[0], "(", addr[1], ")\n")

s_name = conn.recv(1024)
s_name = s_name.decode()
print(s_name, "has connected to the chat room\nEnter [e] to exit chat room\n")
conn.send(name.encode())

while True:
    message = input(str("\nMe : "))
    send_time = str(ctime()[11:19])
    print("Sent at: ", send_time)
    if message == "[e]":
        message = "Left chat room!"
        conn.send(message.encode())
        conn.send(send_time.encode())
        print("\n")
        break
    conn.send(message.encode())
    conn.send(send_time.encode())
    message = conn.recv(1024)
    receive_time = conn.recv(1024)
    message = message.decode()
    receive_time = receive_time.decode()
    print("\n", s_name, ":", message)
    print("Received at: ", receive_time)
