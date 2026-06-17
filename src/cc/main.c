#include "conn.h"
#include "util.h"
#include <stdio.h>
#include <stdbool.h>

#define CMD_BLOCKSIZE 1024
#define OUTPUT_SIZE 4096

static void flushin() {
        int c;
        while((c = getchar()) != '\n' && c != EOF);
}


void mainloop(int conn) {
        static char cmd[CMD_SIZE];
        static char output[OUTPUT_SIZE];

        while(1) {
                putchar('#');
                fflush(stdout);

                size_t len;
                bool eof = false;

                for(len = 0; len < CMD_SIZE; ++len) {
                        int c = getchar();

                        if(c == EOF) {
                                eof = true;
                                break;
                        }
                        else if(c == '\n') {
                                break;
                        }

                        cmd[len] = c;
                }

                cmd[len] = 0;


                if(len == 0 || eof) {
                        break;
                }

                if(cmd[len - 1] == '\n') {
                        len--;
                        cmd[len] = 0;
                }

                if(strcmp(cmd, "exit") == 0)
                        break;

                if(len > 0)
                        write(conn, cmd, len);

                // Receive output

                int r = 0;

                if((r = read(conn, output, sizeof(output))) > 0) {
                        output[r] = 0;
                        puts(output);
                }
        }
}

int main(int argc, char **argv) {

        int status = 1;

        short port;
        if(argc > 1) {
                char *endptr;
                long lport = strtol(argv[1], &endptr, 10);

                if(*endptr != '\0' || *argv[1] == '\0' || lport > SHRT_MAX) {
                        fprintf(stderr, "Invalid port: %s", argv[1]);
                        return 1;
                }

                port = (short)lport;
        }
        else {
                port = 7777;
        }

        int server_fd = get_server(port);
        if(server_fd < 0) return 1;

        int conn = accept(server_fd, NULL, NULL);

        mainloop(conn);

        close(conn);
        close(server_fd);
        
        return status;
}
