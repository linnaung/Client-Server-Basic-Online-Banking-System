#include "inet.h"
#include "avoidCtrlC.c"
#define BUFSIZE 1024


int sockfd, new_sockfd,clilen;
char buffer[BUFSIZE+1];
struct sockaddr_in serv_addr,cli_addr;
char *name="signup.txt";
char *name_balance="balance.txt";

void connection(){


  	if((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0){
                perror("Server: socket() error\n");
                exit(1);
        }
        printf("\nThis basic online banking system is block ctrl-c\n");

        bzero((char *) &serv_addr,sizeof(serv_addr));//we are clearing strcut
        serv_addr.sin_family = AF_INET;// IP family
        serv_addr.sin_addr.s_addr = inet_addr(SERV_IP);//static ip address for server
        serv_addr.sin_port = htons(SERV_TCP_PORT);//server port

        if(bind (sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){//binding socket to server
                perror("Server: bin() error\n");
                exit(1);
        }
        printf("\n waiting for connection...\n");
        listen(sockfd,5);//queue up to five client
	signal(SIGINT,sigproc);// ignore ctrl-c
        signal(SIGQUIT,quitproc);// force to quit is ctrl-\



}

void signupInfo(){

        FILE *fp;
        bzero(buffer,sizeof(buffer));
        recv(new_sockfd,buffer,BUFSIZE,0);



        printf("\nreceived registeration info '%s' from client\n",buffer);
        fp = fopen("signup.txt","w+");
        fprintf(fp,"%s",buffer);
        send(new_sockfd,buffer,BUFSIZE,0);
        fclose(fp);
}

void addBalance(){

	
 	FILE *fp1;
        bzero(buffer,sizeof(buffer));
        recv(new_sockfd,buffer,BUFSIZE,0);


        printf("\nreceived Rm %s amount from client\n",buffer);
        fp1 = fopen("balance.txt","w+");
        fprintf(fp1,"%s",buffer);
        send(new_sockfd,buffer,BUFSIZE,0);
        fclose(fp1);




}

void updateClientBalance(){
        int fd;
        fd = open(name_balance,O_RDONLY);
        if(fd == -1){
                printf("fail to open balance file\n");exit(1);
        }
        read(fd,buffer,BUFSIZE);
        close(fd);
        send(new_sockfd,buffer,BUFSIZE,0);
	printf("resending stroed balance amount Rm %s to client....\n",buffer);


}



int main(){

	//connection function
	connection();
	
	for(;;){	
		clilen = sizeof(cli_addr);//calculating client size IP address
		new_sockfd = accept(sockfd,(struct sockaddr *)&cli_addr,&clilen);//creating new socket for connection state
	
		if(fork() == 0){
			close(sockfd);
	
			if(new_sockfd > 0){
				printf("\nClient connected now\n");
			}
			do{
				recv(new_sockfd,buffer,BUFSIZE, 0);//received through new sockfd and store buffer 
				printf("\nclient typed:[%s]\n",buffer);

				if(!strcmp(buffer,"signup")){
					signupInfo();
				}
				if(!strcmp(buffer,"addbalance")){
					addBalance();
					updateClientBalance();
				}
				if(!strcmp(buffer,"debit")){
					updateClientBalance();
				}
				
				//printf("\nsending back message to client\n");
				//send(new_sockfd,buffer,BUFSIZE,0);
	
			}while (strcmp(buffer,"end"));//comparing buffer /q
			exit(0); 
		}
		close(new_sockfd);//close new socket 
	}
	close(sockfd);//close first socket
}
