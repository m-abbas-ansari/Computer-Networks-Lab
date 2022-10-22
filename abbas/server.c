// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#define PORT 8080

char *substr(const char *src, int m, int n)
{
    // get the length of the destination string
    int len = n - m;

    // allocate (len + 1) chars for destination (+1 for extra null character)
    char dest = (char)malloc(sizeof(char) * (len + 1));

    // extracts characters between m'th and n'th index from source string
    // and copy them into the destination string
    for (int i = m; i < n && (*(src + i) != '\0'); i++)
    {
        *dest = *(src + i);
        dest++;
    }

    // null-terminate the destination string
    *dest = '\0';

    // return the destination string
    return dest - len;
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Accepting from client until a close message is received
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        char read_buffer[1024] = {0};
        char send_buffer[1024] = {0};
        valread = read(new_socket, read_buffer, 1024);
        // Get time of receival
        time_t t = time(NULL);
        char *time_received = substr(ctime(&t), 11, 16);
        char *time_sent_from_client = substr(read_buffer, 0, 5);
        read_buffer = substr(read_buffer, 6, strlen(read_buffer));

        if (strcmp(read_buffer, "close") == 0)
        {
            printf("Closing connection\n");
            break;
        }
        
        printf("[Client]: %s [Sent at: %s, Received at: %s]\n", read_buffer, time_sent_from_client, time_received);
        printf(">> ");
        scanf("%[^\n]%*c", send_buffer);
        time_t t = time(NULL);
        char *time_sent = substr(ctime(&t), 11, 16);
        char temp[1024] = {0};
        strcpy(temp, time_sent);
        strcat(temp, send_buffer);
        send(new_socket, temp, strlen(temp), 0);     
    }

    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
    return 0;
}
