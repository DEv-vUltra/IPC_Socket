/*
 * SERVER – AF_UNIX DATAGRAM NON-BLOCKING + SELECT()
 * Nhận dữ liệu từ nhiều client mà không bị chờ.
 * Dùng select() để biết khi nào có dữ liệu đến.
 */

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/select.h>

#define SERVER_PATH "/tmp/unix_dgram_server"

int main() {
    int sockfd;
    struct sockaddr_un addr;
    char buf[256];

    // Tạo socket AF_UNIX dạng datagram
    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    // Xóa file cũ (nếu tồn tại)
    unlink(SERVER_PATH);

    // Gán địa chỉ cho socket server
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SERVER_PATH);
    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }

    //  Đặt socket sang non-blocking mode
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

    printf("Server listening on %s\n", SERVER_PATH);

    // Vòng lặp chính – sử dụng select() để chờ dữ liệu
    while (1) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);

        // select() chờ tối đa 2 giây
        struct timeval timeout = {2, 0};
        int ready = select(sockfd + 1, &readfds, NULL, NULL, &timeout);

        if (ready < 0) {
            perror("select");
            break;
        } else if (ready == 0) {
            printf(" Không có dữ liệu...\n");
            continue;
        }

        //  Nếu socket có dữ liệu, đọc gói tin
        if (FD_ISSET(sockfd, &readfds)) {
            struct sockaddr_un client_addr;
            socklen_t client_len = sizeof(client_addr);
            int n = recvfrom(sockfd, buf, sizeof(buf) - 1, 0,
                             (struct sockaddr*)&client_addr, &client_len);

            if (n < 0) {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                    continue; // Không có gì để đọc
                perror("recvfrom");
                break;
            }

            buf[n] = '\0';
            printf("Nhận từ %s: %s\n", client_addr.sun_path, buf);
        }
    }

    close(sockfd);
    unlink(SERVER_PATH);
    return 0;
}
