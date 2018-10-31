#include <stdio.h>

int main () {
    printf ("Hello world !\n");

    return 0;
}

/*
commend :
gcc -o dynamic.o -c dynamic.c
gcc -shared -fPIC -o dynamic.so dynamic.o
sudo cp dynamic.so /usr/lib

*/