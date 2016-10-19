#include "message_queue.h"
struct my_msg_st 
{
	long int my_msg_type;    /* type of msg q */
        char some_text[MAX_TEXT];  /* Buffer for IPC_INFO*/
};

int main()
{

	int running=1;
	int msgid;
	char buffer[BUFSIZ];
	struct my_msg_st some_data;
	msgid=msgget((key_t)1234,0666|IPC_CREAT);
        if(msgid==-1)
	{
		fprintf(stderr,"Failed creating msgget\n");
		exit(EXIT_FAILURE);
	}
	while(running)
	{
		printf("enter some text:");
		fgets(buffer,BUFSIZ,stdin);
		some_data.my_msg_type=1;
		strcpy(some_data.some_text,buffer);

		if(msgsnd(msgid,(void *)&some_data,MAX_TEXT,0)==-1)
		{
			fprintf(stderr,"Failed msgsnd\n");
			exit(EXIT_FAILURE);
		}
		if(strncmp(buffer,"end",3)==0)
 		{
			running=0;
			exit(0);
		}
	}
	exit(EXIT_SUCCESS);

}

