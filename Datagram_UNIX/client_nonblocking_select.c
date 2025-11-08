/*
 * CLIENT – Gửi dữ liệu dạng AF_UNIX datagram tới server.
 * Mỗi client có địa chỉ riêng, gửi từng gói độc lập.
 */

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define SERVER_PATH "/tmp/unix_dgram_server"

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_un server_addr, client_addr;
    char client_path[64];

    if (argc < 2) {
        printf("Cách dùng: %s <message>\n", argv[0]);
        return 1;
    }

    //  Tạo socket datagram
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);

    //  Tạo tên riêng cho client (để server biết ai gửi)
    snprintf(client_path, sizeof(client_path), "/tmp/unix_dgram_client_%d", getpid());
    unlink(client_path);

    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sun_family = AF_UNIX;
    strcpy(client_addr.sun_path, client_path);
    bind(sockfd, (struct sockaddr*)&client_addr, sizeof(client_addr));

    //  Cấu hình địa chỉ server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, SERVER_PATH);

    // Gửi message tới server
    sendto(sockfd, argv[1], strlen(argv[1]), 0,
           (struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("Đã gửi: %s\n", argv[1]);

    close(sockfd);
    unlink(client_path);
    return 0;
}
