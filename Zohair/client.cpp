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
    cout << "\n--------------------------------------------\n";
    cout << "| SOCKET PROGRAMMING | POINT-TO-POINT CHAT |\n";
    cout << "--------------------------------------------\n";
    cout << "\n\t\t| CLIENT SIDE |\n"
         << endl;

    struct sockaddr_in address;

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0)
    {
        cout << "Socket creation failed!" << endl;
        return 0;
    }
    cout << "Socket created successfully!" << endl;

    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0)
    {
        cout << "Invalid Address" << endl;
        return 0;
    }

    int clientSocket = connect(client_fd, (struct sockaddr *)&address, sizeof(address));

    if (clientSocket < 0)
    {
        cout << "Error in connecting to server!" << endl;
        return 0;
    }

    do
    {
        char r_buffer[1024] = {0};
        char s_buffer[1024] = {0};

        cout << "Enter message to send: ";
        cin.getline(s_buffer, 1024);

        time_t t = time(NULL);
        char *sendingTime = substr(ctime(&t), 11, 19);

        char finalMessage[1024] = {0};
        strcpy(finalMessage, sendingTime);
        strcat(finalMessage, s_buffer);

        send(client_fd, finalMessage, strlen(finalMessage), 0);

        if (strcmp(s_buffer, "exit") == 0)
        {
            cout << "Exiting!" << endl;
            break;
        }

        read(client_fd, r_buffer, 1024);
        time_t t1 = time(NULL);

        char *receivingTime = substr(ctime(&t1), 11, 19);
        char *serverSentTime = substr(r_buffer, 0, 8);

        char *serverMessage = substr(r_buffer, 8, strlen(r_buffer));

        if (strcmp(serverMessage, "exit") == 0)
        {
            cout << "Connection terminated!" << endl;
            break;
        }

        cout << endl
             << "| Server | : " << serverMessage << endl;
        cout << "Received at " << receivingTime << endl;
        cout << "Sent at " << serverSentTime << endl;
        cout << endl;
    } while (1);

    close(client_fd);
    return 0;
}