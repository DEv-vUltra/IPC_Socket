/*
 * CLIENT – KHÔNG CÓ BỘ PHÂN TÁCH
 * Đọc 1024 byte một lần → có thể dính "HelloWorld"
 */

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define SOCKET_PATH "/tmp/unix_sock_no_delim"

int main() {
    int fd;
    struct sockaddr_un addr;

    // Tạo socket
    fd = socket(AF_UNIX, SOCK_STREAM, 0);

    // Cấu hình địa chỉ server
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);

    // Kết nối server
    connect(fd, (struct sockaddr*)&addr, sizeof(addr));

    // Đọc 1 lần → có thể nhận dính hoặc thiếu
    char buf[1024];
    int n = recv(fd, buf, sizeof(buf) - 1, 0);
    buf[n] = '\0';

    printf("Client received: [%s]\n", buf);

    close(fd);
    return 0;
}
