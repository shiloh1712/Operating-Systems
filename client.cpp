#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define MAX 256
#define LOOP 15
#define NSEG 18
#define INT_DIG 9

static int portno;
static struct hostent *server;

using namespace std;

//Some code segments provided by Professor Rincon, available on Blackboard1

class Package{
    public:
        int value;
        int n_digits;
        char segment_code[INT_DIG][NSEG];
        void print(){
            for (int i = n_digits-1; i>=0; i--){
                cout << segment_code[i] << endl;
            }
            cout << endl;
        }
};

void *grandchild_thread(void *x_void_ptr){
    //set up socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
    serv_addr.sin_port = htons(portno);                                                                     

    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
    //error("ERROR connecting");
        cout << "error connecting";

    int dig = *((char *)x_void_ptr) - '0';
    
    int n = write(sockfd, &dig, sizeof(int));
    if (n < 0) 
        // error("ERROR writing to socket");
        cout  << "ERROR writing to socket";
    
    char s_code[NSEG];
    bzero(s_code, NSEG);
    
    n = read(sockfd,&s_code,NSEG);
    if (n < 0) 
        //error("ERROR reading from socket");
        cout << "ERROR reading to socket";  
    
    strcpy((char *)x_void_ptr, s_code);    
    
    close(sockfd);
    return 0;
}

void *child_thread(void *x_void_ptr)
{
    class Package *pkg = (class Package *)x_void_ptr;
    int number = pkg->value;
    pthread_t tid[INT_DIG];
    
    int i;
    for(i = 0; number > 0; i++){
        //get next digit
        pkg->segment_code[i][0] = (number % 10) + '0';

        //create grandchild thread
        pthread_create(&tid[i], NULL, grandchild_thread, &pkg->segment_code[i]);

        number /= 10;
    } 
        pkg->n_digits = i;

    for (i--; i >= 0; i--){
        pthread_join(tid[i], NULL);
    }
	return NULL;
}

int main(int argc, char *argv[])
{
    int next_num;
    int n;
    pthread_t tid[MAX];
    Package pkgs[MAX];

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
        
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
       
    int i = 0;
    while (cin >> pkgs[i].value){ 


        pthread_create(&tid[i], NULL, child_thread,&pkgs[i]);
        i++;
    }
    
    for (int j = 0; j < i; j++){
        pthread_join(tid[j], NULL);
        pkgs[j].print();
    }

    return 0;
}
