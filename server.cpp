#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define LOOP 50
#define NSEG 18

using namespace std;

//Some code segments provided by Professor Rincon, available on Blackboard1


void fireman(int){
    while(waitpid(-1, NULL, WNOHANG)>0){}
}

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){
    
    int sockfd, newsockfd, portno, clilen;
     
    int msg;
    char buffer[NSEG];

    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) ;
        //error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) ;
              //error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    signal(SIGCHLD, fireman);

    char seg_code[][NSEG] = {"0 = 1 1 1 1 1 1 0", "1 = 0 1 1 0 0 0 0", "2 = 1 1 0 1 1 0 1", "3 = 1 1 1 1 0 0 1", 
                            "4 = 0 1 1 0 0 1 1", "5 = 1 0 1 1 0 1 1", "6 = 1 0 1 1 1 1 1", "7 = 1 1 1 0 0 0 0",
                            "8 = 1 1 1 1 1 1 1", "9 = 1 1 1 1 0 1 1"};

    while (true){
        //parent should receive connection, not child process
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *)&clilen);

        if (fork()==0){

            if (newsockfd < 0) 
                cout << "error accept";
            //error("ERROR on accept");
                
            n = read(newsockfd,&msg,sizeof(int));

            if (n < 0) 
                cout <<"error reading";
                //error("ERROR reading from socket");

            bzero(buffer, NSEG);
            strcpy(buffer, seg_code[msg]);

            n = write(newsockfd,&buffer,strlen(buffer));
            if (n < 0)
                cout << "errror writing";
                //error("ERROR writing to socket");
            close(newsockfd);
            _exit(0);
        } 
    }
    
    close(sockfd);
    return 0;
}
