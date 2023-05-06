#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>

using namespace std;

int main() {
    int pipefd[2];
    pid_t pid;
    char buffer[256];
    string message = "Hello parent process!";
    
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    pid = fork();
    
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
    if (pid == 0) { // child process
        close(pipefd[0]); // close unused read end of the pipe
        write(pipefd[1], message.c_str(), message.length() + 1);
        cout << "Child process sent message: " << message << endl;
        close(pipefd[1]); // close write end of the pipe
        close(pipefd[0]); // close unused read end of the pipe
        if(write(pipefd[1], message.c_str(), message.length() + 1) > 0){
            cout << "Child process sent message: " << message << endl;
        }
        close(pipefd[1]); // close write end of the pipe
        exit(EXIT_SUCCESS);
    } else { // parent process
        close(pipefd[1]); // close unused write end of the pipe
        read(pipefd[0], buffer, sizeof(buffer));
        cout << "Parent process received message: " << buffer << endl;
        close(pipefd[0]); // close read end of the pipe
        wait(NULL); // wait for child process to finish
        exit(EXIT_SUCCESS);
    }
    
    return 0;
}
