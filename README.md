# ChrootJailExit
C code to escape chroot jails.

## How does it works?

So lets remember the situation: you are stuck in a chroot jail so that you cannot go through the / (lets call it A-root)

So one think you need to know if you want to understand the attack is that a process always has its own process-root-directory and can only have one. The only defense used to insure that a process is not leaving its root directory is that "/.." is the same as "/".

So the binary will open a directory and get its file descriptor. Then it will chroot our process into a sub-directory (lets call it B-chroot). We need the directory we open earlier to be outside our new chrooted directory. Next, the binary will use the saved file descriptor to change the working directory of our process. Because we are not using "/.." but the file descriptor, there is no protection against it. So, at this point our process and all its children have B-root as their process-root-directory but are already outside. Because our process can't have multiple process-root-directory the A-root has been removed and we can go where we want except under the B-root.       
