#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
 
#define READ 0
#define WRITE 1
 
void primes(int num[], int size)
{
        int pipe1[2];
        pipe(pipe1);
        int pid = fork();
        if(pid > 0){
                close(pipe1[READ]);
                for(int i = 0; i < size; i++){
                        write(pipe1[WRITE], &num[i], sizeof(num[i]));
                }
                close(pipe1[WRITE]);
                wait((int*)0);
        }else{
                close(pipe1[WRITE]);
                int numchr[34], indnx = 0;
                int tmp, min;
                while(read(pipe1[READ], &tmp, sizeof(tmp))){
                        if(indnx == 0)
                        {
                                min = tmp;
                                printf("prime %d\n", min);
                                indnx++;
                        }
                        if(tmp % min != 0){
                                numchr[indnx - 1] = tmp;
                                indnx++;
                        }
                }
                close(pipe1[READ]);
                primes(numchr, indnx - 1);
                exit(0);
        }
}
 
int main(int argc, char *argv[])
{
        int num[34];
        int indnx = 0;
        for(int i = 2; i <= 35; i++){
                num[indnx] = i;
                indnx++;
        }
        primes(num, 34);
        exit(0);
}
 
