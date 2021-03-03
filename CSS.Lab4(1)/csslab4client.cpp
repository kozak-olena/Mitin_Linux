#include <unistd.h>
#include <stdio.h>
#include <error.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <strings.h>
#include <fstream>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#define MAXLINE 128
#define FILE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define FIFO1 "/tmp/fifo.1"
#define FIFO2 "/tmp/fifo.2"

int main(int argc, char **argv) 
{
    int readfd = -1, writefd = -1;
    pid_t childpid = 0;
    
    ssize_t n = 0;
    char str[MAXLINE];
    std::ofstream fsw("./rezult.txt");
    fsw<<"Opening pipes..."<<std::endl;
    while (1) 
    {
        readfd = open(FIFO2, O_RDONLY, 0);
        if (readfd != -1)  
        {
            fsw<<"Pipes opened..."<<std::endl;
            fsw<<"Waiting for respond..."<<std::endl;
            while ((n = read(readfd,str, MAXLINE)) > 0)
            { 
                str[n] = 0; 
                fsw<<"Received string -\""<<str<<"\""<<std::endl; 
                break;
            }
            strcpy(str,"Ok from other process");
            writefd = open(FIFO1, O_WRONLY, 0);
            fsw << "Transmitting the string -\"" << str << "\"" <<std::endl;
            write(writefd,str,strlen(str));
            close(readfd);
            close(writefd);
            break;
        }
    }
    fsw.close();
    return 1;    
}