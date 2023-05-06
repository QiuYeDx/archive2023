#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>

using namespace std;

#define MSG_SIZE 256

int main() {
    int pipefd[2];
    pid_t pid[3];
    char buffer[256];
    string message = "Hello parent process!";
    
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // 创建三个子进程
    for (int i = 0; i < 3; i++) {
        pid[i] = fork();
        if (pid[i] < 0) {
            cerr << "Failed to fork process" << endl;
            exit(1);
        }
        else if (pid[i] == 0) { // 子进程
            close(pipefd[0]); // 关闭管道读端

            int n = write(pipefd[1], (message.append(" I'm Child ").append(to_string(i + 1))).append("\n").c_str(), message.length()); // 读取消息
            if (n > 0) {
                cout << "Child process " << i + 1 << " sent message: " << message << endl;
            }
            close(pipefd[1]); // 关闭管道写端
            exit(0);
        }
    }
    
    close(pipefd[1]); // 关闭管道写端
    sleep(1);
    read(pipefd[0], buffer, sizeof(buffer));
    // sleep(1);
    cout << "Parent process received message: \n" << buffer << endl;
    
    // sleep(1);
    // read(pipefd[0], buffer, sizeof(buffer));
    // cout << "Parent process received message: \n" << buffer << endl;
    
    close(pipefd[0]); // 关闭管道读端
    exit(EXIT_SUCCESS);
    
    return 0;
}
