#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

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
    cout<<"* Welcome to the Server Side of the Chat *";
    cout<<"\n******************************************\n";

    struct sockaddr_in srv;
    
    int nSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (nSocket < 0)
    {
        cout << "Error in creating socket" << endl;
        return 0;
    }
    cout << "Socket created" << endl;

    srv.sin_family = AF_INET;
    srv.sin_port = htons(port);
    srv.sin_addr.s_addr = INADDR_ANY;

    int nRet = bind(nSocket, (struct sockaddr *)&srv, sizeof(srv));
    if (nRet < 0)
    {
        cout << "Error in binding" << endl;
        return 0;
    }
    cout << "Binded Successfully" << endl;

    nRet = listen(nSocket, 3);
    if (nRet < 0)
    {
        cout << "Error in listening" << endl;
        return 0;
    }
    cout << "Listening" << endl;

    int nLen = sizeof(srv);
    int newSocket = accept(nSocket, (struct sockaddr *)&srv, (socklen_t *)&nLen);
    if (newSocket < 0)
    {
        cout << "Error in accepting" << endl;
        return 0;
    }
    cout << "Accepted" << endl;

    while (1)
    {
        char read_buffer[1024] = {0};
        char send_buffer[1024] = {0};

        if (read(newSocket, read_buffer, 1024) < 0)
        {
            cout << "Error in reading" << endl;
            return 0;
        }

        time_t t1 = time(NULL);
        char *receivalTime = substr(ctime(&t1), 11, 19);
        char *clientSentTime = substr(read_buffer, 0, 8);

        char *receivedMessage = substr(read_buffer, 8, strlen(read_buffer));
        if (strcmp(receivedMessage, "Exit") == 0)
        {
            cout << "Closing connection\n\n\n" << endl;
            break;
        }

        cout << "[Client]: " << receivedMessage<<endl;
        cout << "[Sent at]: " << clientSentTime<<endl;
        cout << "[Received at]: " << receivalTime << endl;

        cout << "\nEnter the message: ";
        cin.getline(send_buffer, 1024);

        time_t t2 = time(NULL);
        char *sendingTime = substr(ctime(&t2), 11, 19);
        
        char finalMessage[1024] = {0};
        strcpy(finalMessage, sendingTime);
        strcat(finalMessage, send_buffer);

        send(newSocket, finalMessage, strlen(finalMessage), 0);
        cout << endl;
    }
    close(newSocket);
    shutdown(nSocket, SHUT_RDWR);
    return 0;
}