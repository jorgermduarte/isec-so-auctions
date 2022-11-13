#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main( int argc, char* argv[] ) {
    setbuf(stdout, NULL);
    sleep(4);
    printf("sapatos 10 100\n");
    fflush(stdin);
    sleep(5);
    printf("cadeiras 25 60\n");
    fflush(stdin);
    sleep(3);
    printf("apple 10 100\n");
    fflush(stdin);
    sleep(10);
    printf("moveis 100 150\n");
    fflush(stdin);
    sleep(10);
    printf("exit");

    return 1;
}