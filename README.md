# ChrootJailExit
C code to escape chroot jails.
 
## How does it works?
 
So let's remember the situation: you are stuck in a chroot jail so that you cannot go through the / (let's call it A-root)
 
So one thing you need to know if you want to understand the attack is that a process always has its own process-root-directory and can only have one. The only defense used to insure that a process is not leaving its root directory is that "/.." is the same as "/".
 
So the binary will open a directory and get its file descriptor. Then it will chroot our process into a sub-directory (let's call it B-chroot). We need the directory we open earlier to be outside our new chrooted directory. Next, the binary will use the saved file descriptor to change the working directory of our process. Because we are not using "/.." but the file descriptor, there is no protection against it. So, at this point our process and all its children have B-root as their process-root-directory but are already outside. Because our process can't have multiple process-root-directory the A-root has been removed and we can go where we want except under the B-root.
 
If we only do this, when you will call system(), it will use the $PATH which always start with /. This means that it will use the path under B-root. As you may understood, this means that you must have chrooted in a directory with all the directory contained in the $PATH. Of course, it is not always possible (or you will had to create it which is not really discreet). 
 
To resolve this problem, the code will directly try to chroot in the real root directory (or at least in another chrooted root directory if there are nested chroots). In any case, in this root directory, there are every directory you will need to use system(). This means that you no longer need to first chroot in a directory which contains all the directory contained in the $PATH but in any directory. This is more discreet. 
