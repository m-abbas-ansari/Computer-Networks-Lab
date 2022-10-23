#include <iostream>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;
#define port 5000

char *substr(const char *src, int m, int n)
{
    int len = n - m;

    char *dest = (char *)malloc(sizeof(char) * (len + 1));

    for (int i = m; i < n && (*(src + i) != '\0'); i++)
    {
        *dest = *(src + i);
        dest++;
    }

    *dest = '\0';

    return dest - len;
}

int main()
{
    cout<<"\n******************************************\n";
    cout<<"* Welcome to the Client Side of the Chat *";
    cout<<"\n******************************************\n";
    int nSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (nSocket < 0)
    {
        cout << "Error in creating socket" << endl;
        return 0;
    }

    struct sockaddr_in srv;
    srv.sin_family = AF_INET;
    srv.sin_port = htons(port);

    if (inet_pton(AF_INET, "127.0.0.1", &srv.sin_addr) <= 0)
    {
        cout << "Invalid Address" << endl;
        return 0;
    }

    int clientSocket = connect(nSocket, (struct sockaddr *)&srv, sizeof(srv));
    if (clientSocket < 0)
    {
        cout << "Error in connecting" << endl;
        return 0;
    }
    while (1)
    {
        char read_buffer[1024] = {0};
        char send_buffer[1024] = {0};

        cout << "\nEnter the message: ";
        cin.getline(send_buffer, 1024);

        time_t t = time(NULL);
        char *sendingTime = substr(ctime(&t), 11, 19);

        char finalMessage[1024] = {0};
        strcpy(finalMessage, sendingTime);
        strcat(finalMessage, send_buffer);

        send(nSocket, finalMessage, strlen(finalMessage), 0);
        if (strcmp(send_buffer, "Exit") == 0)
        {
            cout << "\nExiting\n\n\n" << endl;
            break;
        }

        read(nSocket, read_buffer, 1024);
        time_t t1 = time(NULL);
        char *receivalTime = substr(ctime(&t1), 11, 19);
        char *servertSentTime = substr(read_buffer, 0, 8);

        char *receivedMessage = substr(read_buffer, 8, strlen(read_buffer));
        if (strcmp(receivedMessage, "Exit") == 0)
        {
            cout << "Closing connection\n\n\n" << endl;
            break;
        }
        
        cout << "\n[Server]: " << receivedMessage<<endl;
        cout << "[Sent at]: " << servertSentTime<<endl;
        cout << "[Received at]: " << receivalTime << endl;
        cout << endl;
    }
    close(nSocket);
    return 0;
}