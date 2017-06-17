#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char *argv[]){
	
	if(argc != 3){
		printf("We need a path to a socket and a path to chroot\n");
		return EXIT_FAILURE;
	}
	
	char * path=argv[1];
	int pid = fork();
	
	/*
	 * Socket creation for both process.
	 * */
	socklen_t len=sizeof(struct sockaddr_un);
	int sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sock <0){
		perror("socket");
		return EXIT_FAILURE;
	}
	
	if(access( path, F_OK) != -1 ) {
		if(unlink(path)==-1){
			perror("unlink");
			return EXIT_FAILURE;
		}
	}
	
	struct sockaddr_un addr;
	memset(&addr,0,len);
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, path,sizeof(addr.sun_path) -1);
	int fd;
	
	switch(pid){
		case -1:
			perror("fork");
			return EXIT_FAILURE;
		
		case 0:
			/*
			 * We are in child process.
			 * Open directory to get its file descriptor.
			 * */
			fd=open(".",O_DIRECTORY);
			if(fd<0){
				perror("open");
				return EXIT_FAILURE;
			}
			
			/*
			 * Connection to socket.
			 * */
			sleep(1); // wait for server to start.
			if(connect(sock,(struct sockaddr *)&addr, len) == -1){
				perror("client connect");
				return EXIT_FAILURE;
			}
			
			/*
			 * Sends the file descriptor through the socket.
			 * */
			if(send(sock, (void *) &fd, sizeof(int), 0) == -1){
				perror("send");
				return EXIT_FAILURE;
			}
			
			close(sock);
			
			break;
		
		default:
			/*
			 * We are in father process.
			 * Create server.
			 * */
			if(bind(sock,(struct sockaddr *)&addr, len) == -1){
					perror("client bind");
					return EXIT_FAILURE;
			}

			if(listen(sock,1)==-1){
				perror("server listen");
				return EXIT_FAILURE;
			}

			int new = accept(sock, (struct sockaddr *)&addr, &len);
			if(new ==-1){
				perror("accept");
				return EXIT_FAILURE;
			}
			printf("server has accepted connexion\n");
			
			/*
			 * Get file descriptor.
			 * */
			int fd;
			if(recv(new,(void *)&fd, sizeof(int), 0) == -1 ){
				perror("recv");
				return EXIT_FAILURE;
			} 

			printf("file descriptor received: %d\n",fd);

			close(sock);
			
			//system("/bin/bash -i");
			break;
	}
	
	return EXIT_SUCCESS;
}
