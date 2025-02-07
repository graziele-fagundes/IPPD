#include <omp.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

void bubble_sort(int v[])
{
    #pragma omp parallel for
    for (int i=0; i<10; i++)
    {
       for (int j=0; j<10-i-1; j++)
       {
        if (v[j] > v[j+1])
        {
            #pragma omp critical
            {
                int temp = v[j];
                v[j] = v[j+1];
                v[j+1] = temp;
            }
        }
       } 
    }
}

int main()
{
    int v[10] = {3,5,12,35,61,12,7,43,1,0};

    bubble_sort(v);

    printf("Vetor ordenado:\n");
    for (int i = 0; i < 10; i++) {
        printf("%d ", v[i]);
    }
    printf("\n");
    return 0;
}