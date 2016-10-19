#include "inet.h"
#include "message_queue.h"
#include "menu.c"
#include "avoidCtrlC.c"
#include "signup.c"
#define BUFSIZE 1024
#define INPUT 10

int sockfd;
char buffer[BUFSIZE+1];
struct sockaddr_in serv_addr;
char userInput[INPUT];
char *name="tem.txt";
char *check="update.txt";
int fd;
int start_session = 0;


void connection(){

	if((sockfd = socket(AF_INET, SOCK_STREAM,0)) <0){//socket start TCP connection if less than zero print error
                perror("Client: socket() error\n");
                exit(1);
        }
        if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){//connection start if less than zero, show error
                perror("client: connect() error\n");
                exit(1);
        }

}

void sendData(){

        send(sockfd,buffer,BUFSIZE,0);
}
void receiveData(){

        bzero(buffer,sizeof(buffer));//clearing buffer or setting buffer to zero
        recv(sockfd,buffer,BUFSIZE,0);
}


int sendSignuptoServer(){
	

	start_session = 1;

        fd = open (name, O_RDONLY);
        if(fd == -1){
                printf("faile to open file....\n");exit(1);
        }
        read(fd,buffer,BUFSIZE);
        close(fd);
        sendData();
        printf("\nsending signup info to server....\n");
        receiveData();
        printf("\nRegisteration Success....\n");

	start_session = 100;

        
}



int addBalance(){

start_session = 100;
       
        int add, min = 10, max = 1000;
        printf("\nplease enter the amount that you want to add balance to your account...\n");
        printf("\nthe accept amount is Rm 10 and Rm 1000\n");
        scanf("%d",&add);
        if(add < min || add > max){
                printf("you have invalid amount....please try again.....\n");
                do{
                        scanf("%d",&add);
                }while(add < min || add > max);

                sprintf(buffer,"%d",add);//integer to string
                printf("you have entered Rm %s amount....\n",buffer);
                //send(sockfd,balance,BSIZE,0);
                sendData();
                printf("\nsending balance amount to sever....\n");
                receiveData();
                printf("\nbalance is successfully sent to server....\n");
		//storing update balance 
		FILE *fp;
	        fp=fopen("update.txt","w+");
        	fprintf(fp,"%s",buffer);
        	fclose(fp);


        }
        else if(add > min || add < max){

                sprintf(buffer,"%d",add);//integer to string
                printf("\nyou have entered Rm %s amount....\n",buffer);
                sendData();
                printf("\nsending balance amount to server....\n");
                receiveData();
                printf("\nbalance is successfully sent to server....\n");
		//storing update balance
		FILE *fp;
	        fp=fopen("update.txt","w+");
        	fprintf(fp,"%s",buffer);
        	fclose(fp);


        }
        else{
                printf("invalid input...please try again....\n");
                do{
                        scanf("%d",&add);
                }while(add < min || add >max);
                sprintf(buffer,"%d",add);//integer to string
                printf("you have entered Rm %s amount....\n",buffer);
                //send(sockfd,balance,BSIZE,0);
                sendData();
                printf("\nsending balance amount to sever....\n");
                receiveData();
                printf("\nbalance is successfully sent to server....\n");

        }


}


void updateBalance(){

	int i;
	int amount=0;
	start_session=100;
        receiveData();
        printf("\nyour update balance is Rm %s amount....\n",buffer);

	i =atoi(buffer);//string to integer
	//printf("integer value is %d",i);
	do{
		printf("enter amount to debit: ");
		scanf("%d",&amount);
		if(amount > i){
			printf("enterd amount greater than current balance\n");
		}
	

	}while(amount > i);

	amount = i-amount;
	//printf("your update balance is now Rm %d\n",amount);
	sprintf(buffer,"%d",amount);//integer to string


	//pipe
	int pipefd[2],j;
	if(pipe(pipefd)==-1){
		perror("pipe error");exit(1);
	}
	write(pipefd[1],buffer,BUFSIZE);
	read(pipefd[0],buffer,BUFSIZE);
	printf("your update is now RM %s\n",buffer);

	//store or wirting to file 
	FILE *fp;
	fp=fopen("update.txt","w+");
	fprintf(fp,"%s",buffer);
	fclose(fp);


}
void check2(){
	start_session=100;

	int fd1;
        fd1 = open (check, O_RDONLY | O_TRUNC);
        if(fd1 == -1){
                printf("faile to open file....\n");exit(1);
        }
        read(fd1,buffer,BUFSIZE);
	printf("your latest balance is Rm %s\n",buffer);
        close(fd);
        //sendData();
        //printf("\nsending signup info to server....\n");
        //receiveData();
        //printf("\nRegisteration Success....\n");

       



}


int main(int argc, char *argv[]){
	
	if(argc <= 1){
		exit(1);
	}
	bzero((char*)&serv_addr, sizeof(serv_addr));//this line initialize server address to zero 
	serv_addr.sin_family = AF_INET;//IP family
	serv_addr.sin_port = htons (SERV_TCP_PORT);//converts ports no in host byte to a port number
	inet_pton (AF_INET, argv[1],&serv_addr.sin_addr);//

	//connection function
	connection();


	do{

		printf("\ntype menu: ");
		scanf("%s",userInput);
		send(sockfd,userInput,INPUT, 0);
		if(!strcmp(userInput,"menu")){
			menu();
		}
		if(!strcmp(userInput,"signup")){

			if(start_session==1){
				printf("you have signed up!!!i\n");
			}
			else if(start_session==0){
				signup();
				sendSignuptoServer();
			}		
		}
		if(!strcmp(userInput,"check1")){

			if(start_session==0){
				printf("please signup to check balance\n");
			}
			else if(start_session>0){
				 printf("\nCurrent Balance is Rm zero\n");
			}
		}
		if(!strcmp(userInput,"addbalance")){

			if(start_session==0){
				printf("please signup to add balance\n");
			}
			else if(start_session>0){
				addBalance();
				//updateBalance();
			}

		}
		if(!strcmp(userInput,"debit")){

			if(start_session==0){
				printf("please signup to debit\n");
			}
			else if(start_session>0){
				updateBalance();
			}
		}
		if(!strcmp(userInput,"check2")){

			if(start_session==0){
				printf("please signup to check your update balance\n");
			}
			else if(start_session>0){

				check2();
			}
		}
		if(!strcmp(userInput,"telnet")){
			runTelnet():
		}

		if(!strcmp(userInput,"end")){
			exit(0);
		}
		else{
		
		}
	
			//send(sockfd, buffer, BUFSIZE, 0);
			//bzero(buffer, sizeof(buffer));
			//recv(sockfd, buffer, BUFSIZE, 0);
			//printf("Message received from server: %s\n", buffer);



	}while(strcmp(buffer,"end"));
	close(sockfd);
}
