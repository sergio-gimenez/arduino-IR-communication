#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
  
int random_bit_generator();  

int main() 
{     
    //printf("%d\n ", random_bit_generator()); 

    // Fixed 32 bit length message to send
    char msg[32];

    for (long int i = 0; i < 32; i++){
        msg[i] = random_bit_generator(i);        
    }

    for (int i = 0; i < 32; i++){
    printf("%d", msg[i]);
    //printf("%d", random_bit_generator());       
    }
    printf("\n\n");
    return 0; 
} 

int random_bit_generator(unsigned int seed) 
{   
        // Use current time as seed for random generator     
        srand(time(0));   
        int num = rand() % 2;
        return num;  
} 