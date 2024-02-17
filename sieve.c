#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void sieve(int max_num_to_sieve) {

    // TODO: add your code here!


}

int main(int argc, char **argv) {
    // DO NOT CHANGE THIS! Add your code in sieve!
    int num_to_sieve = 35;
    if (argc == 2) {
        num_to_sieve = atoi(argv[1]);
    }
    sieve(num_to_sieve);
}