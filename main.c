#include "fft.h"
#include <stdio.h>
#define SIZE 4
void testInput(double *input, int size)
{
    int i = 0;
    for(;i < size; i ++)
    {
        input[i] = sin(2*M_PI*i / size + M_PI / 4);
    }
}

int main(int argc, const char * argv[]) {  
    double input[SIZE];  
    Complex dst[SIZE];  
    testInput(input, SIZE);
    FFT(input, dst, SIZE);  
} 