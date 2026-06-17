#include "conn.h"
#include "util.h"
#include <sys/socket.h>

int get_server(short port) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if(sock < 0)
                ERROR("socket", socket_error);

        int reuse = 1;
        if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
                ERROR("setsockopt(SO_REUSEADDR)", setsockopt_error);

        if(setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) < 0)
                ERROR("setsockopt(SO_REUSEPORT)", setsockopt_error);

        struct sockaddr_in addr = {
                .sin_addr = 0,
                .sin_port = htons(port),
                .sin_family = AF_INET
        };

        if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
                ERROR("bind", bind_error);

        if(listen(sock, 1) < 0)
                ERROR("listen", listen_error);

        return sock;

listen_error:
bind_error:
setsockopt_error:
        close(sock);
socket_error:
        return -1;
}

int get_connection(const char *host, const char *port) {
        struct addrinfo* addrinfo;

        int sock = socket(AF_INET, SOCK_STREAM, 0);

        if(sock < 0)
                ERROR("socket", socket_error);

        if(getaddrinfo("127.0.0.1", "7777", 
                &(struct addrinfo){
                        .ai_family = AF_INET,
                        .ai_socktype = SOCK_STREAM,
                        .ai_flags = AI_PASSIVE,
                        .ai_protocol = 0,
                        .ai_addr = NULL,
                        .ai_addr = NULL,
                        .ai_next = NULL
                },
                &addrinfo
        ) < 0) ERROR("getaddrinfo", getaddrinfo_error);

        int conn_ret = -1;
        for(struct addrinfo *i = addrinfo; i != NULL; i = i->ai_next) {
                conn_ret = connect(sock, i->ai_addr, i->ai_addrlen);
                if(conn_ret == 0)
                        break;
        }

        if(conn_ret < 0)
                ERROR("connect", connect_error);

        return sock;

connect_error:
getaddrinfo_error:
        close(sock);
socket_error:
        return -1;
}
