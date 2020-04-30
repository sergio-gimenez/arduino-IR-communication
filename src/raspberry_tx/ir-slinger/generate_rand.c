#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RAND_MAX_32B 4294967296

int main(int argc, char *argv[])
{

    // ** Random message generation ** \\

    // The random number is seeded with the current time.
    srand(time(0));
    char rand_msg[32];

    int num = (rand() % (RAND_MAX_32B + 1));

    for (int i = 0; i < 32; i++){
    // Right shift num, i times and perform bitwise AND with 1
    int current_bit = (num >> i) & 1;

    // Add the i-bit to the i-position of the array
    sprintf(&rand_msg[i], "%d",current_bit);
    }

    printf("IR Random message generated: %s\n", rand_msg);

    // Write generated member in file
    FILE *file;
    file = fopen("expected_message.txt","w");
    /*if(file == NULL) {
    	printf("File not available for writing\n");
    	exit(1);
    }*/
    fprintf(file, "%s", rand_msg);
    fclose(file);

    return 0;
}

