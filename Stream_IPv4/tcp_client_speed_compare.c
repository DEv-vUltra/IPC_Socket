#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>

#define SERVER_IP "192.168.55.100"
#define SERVER_PORT 6789
#define MSG "Hello"
#define COUNT 1000


double now() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {0};
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &addr.sin_addr);
    connect(sock, (struct sockaddr*)&addr, sizeof(addr));

    double start = now();
    for (int i = 0; i < COUNT; i++)
        send(sock, MSG, strlen(MSG), 0);
    double end = now();

    printf("TCP: Sent %d messages in %.6f sec\n", COUNT, end - start);
    close(sock);
}
