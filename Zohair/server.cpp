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
    cout << "\n--------------------------------------------\n";
    cout << "| SOCKET PROGRAMMING | POINT-TO-POINT CHAT |\n";
    cout << "--------------------------------------------\n";
    cout << "\n\t\t| SERVER SIDE |\n"
         << endl;

    struct sockaddr_in address;
    int opt = 1;

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        cout << "Socket creation failed!" << endl;
        return 0;
    }
    cout << "Socket created successfully!" << endl;

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        cout << "Socket options failed!" << endl;
        return 0;
    }
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        cout << "Socket binding failed!" << endl;
        return 0;
    }
    cout << "Socket binded successfully!" << endl;

    if (listen(server_fd, 3) < 0)
    {
        cout << "Socket listening failed!" << endl;
        return 0;
    }
    cout << "Socket listening successfully!" << endl;

    int new_socket;
    int addrlen = sizeof(address);
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        cout << "Socket accepting failed!" << endl;
        return 0;
    }
    cout << "Socket accepted successfully!" << endl;

    do
    {
        char r_buffer[1024] = {0};
        char s_buffer[1024] = {0};

        if (read(new_socket, r_buffer, 1024) < 0)
        {
            cout << "Failed to receive message!" << endl;
            return 0;
        }

        time_t now = time(0);
        char *received_time = substr(ctime(&now), 11, 19);
        char *clientSentTime = substr(r_buffer, 0, 8);
        char *msg = substr(r_buffer, 8, strlen(r_buffer));

        if (strcmp(msg, "exit") == 0)
        {
            cout << "Client has left the chat!" << endl;
            break;
        }

        cout << endl
             << "| Client | : " << msg << endl;
        cout << "Received at " << received_time << endl;
        cout << "Sent at " << clientSentTime << endl;

        cout << endl
             << "Enter message to send: ";
        cin.getline(s_buffer, 1024);

        time_t now2 = time(0);
        char *sent_time = substr(ctime(&now2), 11, 19);

        char final_msg[1024] = {0};
        strcat(final_msg, sent_time);
        strcat(final_msg, s_buffer);

        send(new_socket, final_msg, strlen(final_msg), 0);

        if (send(new_socket, final_msg, strlen(final_msg), 0) < 0)
        {
            cout << "Failed to send message!" << endl;
            return 0;
        }
        cout << endl;
    } while (1);

    close(server_fd);
    shutdown(new_socket, SHUT_RDWR);
    return 0;
}
