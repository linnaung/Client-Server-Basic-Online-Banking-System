#include "inet.h"
#include <unistd.h>

#define BUFSIZE 1024
#define NAME_LENGTH 50
#define ADDRESS_LENGTH 128
#define PASSWORD_LENGTH 8
#define MOB_NUM_LENGTH 10
int signup(){

	FILE *fp;
        char buffer[BUFSIZE];
        char username[NAME_LENGTH], password[PASSWORD_LENGTH], cpassword[PASSWORD_LENGTH];
        char address[ADDRESS_LENGTH], mobile[MOB_NUM_LENGTH];
        int check = 0;	
	fp = fopen("tem.txt","w+");//file open to write 
	
        printf("Username: ");
        scanf("%s",username);
        do
        {

                if( check > 0 ){
                        printf("Password doesn't match, please type again...\n");
		}
		
                printf("Password: ");
                scanf("%s",password);

                printf("Confirm Password: ");
                scanf("%s",cpassword);
                check++;
        }while(strcmp(password,cpassword ) !=0 );

        printf("Address: ");
        scanf("%s",address);

        printf("Contact Number: ");
        scanf("%s",mobile);
	fprintf(fp,"%s,%s\n",username,password);
	fclose(fp);

 
}

