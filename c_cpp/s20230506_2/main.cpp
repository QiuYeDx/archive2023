#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MSG_SIZE 256

using namespace std;

int main() {
    pid_t pid[3];
    int pipe_fd[2];

    // 创建管道
    if (pipe(pipe_fd) < 0) {
        cerr << "Failed to create pipe" << endl;
        exit(1);
    }

    // 创建三个子进程
    for (int i = 0; i < 3; i++) {
        pid[i] = fork();
        if (pid[i] < 0) {
            cerr << "Failed to fork process" << endl;
            exit(1);
        }
        else if (pid[i] == 0) { // 子进程
            close(pipe_fd[1]); // 关闭管道写端

            char msg[MSG_SIZE];
            int n = read(pipe_fd[0], msg, MSG_SIZE); // 读取消息
            if (n > 0) {
                cout << "Child process " << i + 1 << " received message: " << msg << endl;
            }

            close(pipe_fd[0]); // 关闭管道读端
            exit(0);
        }
    }

    close(pipe_fd[0]); // 父进程关闭管道读端

    char msg[MSG_SIZE];
    sprintf(msg, "Hello child processes!");

    if (write(pipe_fd[1], msg, MSG_SIZE) < 0) { // 向管道中写入消息
        cerr << "Failed to write message to pipe" << endl;
    }

    close(pipe_fd[1]); // 父进程关闭管道写端

    // 等待三个子进程结束
    for (int i = 0; i < 3; i++) {
        waitpid(pid[i], NULL, 0);
    }

    return 0;
}
