#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
  
int random_bit_generator();  

int main() 
{     
    //printf("%d\n ", random_bit_generator()); 

    // Fixed 32 bit length message to send
    char msg[32];

    for (int i = 0; i < 32; i++){
        msg[i] = random_bit_generator();        
    }

    printf("%s", msg);       
    
    return 0; 
} 

int random_bit_generator() 
{   
        // Use current time as seed for random generator     
        srand(time(0));   
        int num = rand() % 2;
        return num;  
} 