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
    int readfd = -1,writefd = -1;
    pid_t childpid = 0;
    ssize_t n;
    char str[MAXLINE];
    strcpy(str," some string to transmit ");
    std::cout<<"Creating pipes..."<<std::endl;
    unlink(FIFO1);
    unlink(FIFO2);
    if  (mkfifo(FIFO1,  FILE_MODE)  ==  EEXIST)  
        std::cout<<"\n  Pipes  is exists"<<std::endl;
    if  (mkfifo(FIFO2,  FILE_MODE)  ==  EEXIST) 
        std::cout<<"\n  Pipes  is exists"<<std::endl;

    std::cout<<"Pipes created..."<<std::endl;
    writefd = open(FIFO2, O_WRONLY);
    if ((writefd != -1)) 
    {
        std::cout<<"Transmitting the string..."<<std::endl;
        write(writefd,str,strlen(str));
        readfd = open(FIFO1, O_RDONLY);
        std::cout<<"Waiting for respond..."<<std::endl;
        while ((n = read(readfd,str, MAXLINE)) > 0) 
        {
            str[n] = 0; 
            std::cout<<"Received string -\""<<str<<"\""<<std::endl;
            break;
        }
        close(readfd);
        close(writefd);
        unlink(FIFO1);
        unlink(FIFO2);
    } 
    else 
        std::cout<<"Can't open pipes..."<<std::endl;
    return 1;    
}