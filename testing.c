# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <linux/limits.h>
# include <errno.h>
# include <termcap.h>
# include <termios.h>

int main()
{
    int tmp;
    printf("will open now\n");
    tmp = open("1", O_RDONLY); //| O_CREAT
    printf("opened and tmp is %d\n", tmp);
    if (tmp == -1)
    {
        printf("error opening, tmp is %d\n", tmp);
    }
    printf("will close tmp(%d) now\n", tmp);
    close(tmp);
    printf("closed and tmp(%d)\n", tmp);
}