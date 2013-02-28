#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int pid;

    switch(pid = fork()){


        case -1:

        case 0:

            execlp("ls", "ls", (char *)NULL);

        default:

    }

    return 0;


}
