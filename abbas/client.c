// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
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
    int sock = 0, valread, client_fd;
    struct sockaddr_in serv_addr;
    // char *hello = "Hello from client";

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((client_fd = connect(sock, (struct sockaddr *)&serv_addr,
                             sizeof(serv_addr))) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    // communuicate with server until a close message is sent
    while (1)
    {
        char read_buffer[1024] = {0};
        char send_buffer[1024] = {0};
        printf(">> ");
        scanf("%[^\n]%*c", send_buffer);
        time_t t = time(NULL);
        char *time_sent = substr(ctime(&t), 11, 16);
        char temp[1024] = {0};
        strcpy(temp, time_sent);
        strcat(temp, send_buffer);
        send(sock, temp, strlen(temp), 0);
        if (strcmp(send_buffer, "close") == 0)
        {
            break;
        }
        valread = read(sock, read_buffer, 1024);
        printf("[Server]: %s\n", read_buffer);
    }

    printf("Closing the connected socket\n");
    close(client_fd);
    return 0;
}
