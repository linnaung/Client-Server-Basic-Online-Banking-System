#include "t.h"

//#define D(...) fprintf(new_stream, __VA_ARGS__)

int main() {
	int sock;
	struct sockaddr_in name;
	char buf[MAX_MSG_LENGTH] = {0};

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0){
	 	printf("opening socket");exit(1);
	}
	int optval = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

	name.sin_family = AF_INET;
	name.sin_addr.s_addr = INADDR_ANY;
	name.sin_port = htons(PORT);
	if(bind(sock, (void*) &name, sizeof(name))){
		 printf("binding tcp socket");exit(1);
	}
	if(listen(sock, 1) == -1){
		 printf("listen error");exit(1);
	}
	struct sockaddr cli_addr;
	int cli_len = sizeof(cli_addr);
	int new_socket, new_fd, pid;
	FILE* new_stream;
	
	if(new_fd = dup(STDERR_FILENO) == -1){
		 printf("dup error");exit(1);
	}
	new_stream = fdopen(new_fd, "w");
	setbuf(new_stream, NULL); // sin buffering
	
	printf("Telnet server started...\n");
	while(new_socket = accept(sock, &cli_addr, &cli_len)) {
		printf("Client connected.\nForking... ");
		if(pid = fork()){
			 printf("child pid = %d.\n", pid);
		}
		else {
			pid = getpid();
			if(new_socket < 0){ 
				printf("accept");exit(1);
			}
			if(dup2(new_socket, STDOUT_FILENO) == -1){
				 printf("dup2 error");exit(1);
			}
			if(dup2(new_socket, STDERR_FILENO) == -1){
				 printf("dup2");exit(1);
			}
			while(1) {
				int readc = 0, filled = 0;
				while(1) {
					readc = recv(new_socket, buf+filled, MAX_MSG_LENGTH-filled-1, 0);
					if(!readc) break;
					filled += readc;
					if(buf[filled-1] == '\0') break;
				}
				if(!readc) {
					printf("\t[%d] Client disconnected.\n", pid);
					break;
				}
				printf("\t[%d] Command received: %s", pid, buf);
				system(buf);
				printf("\t[%d] Finished executing command.\n", pid);
				send(new_socket, "> ", 3, MSG_NOSIGNAL);
			}
			close(new_socket);
			printf("\t[%d] Dying.", pid);
			exit(0);
		}
	}
	fclose(new_stream);
	close(sock);
	return 0;
}
