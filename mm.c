#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <math.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include "mm.h"

#define MAXBUF 1000
int length;


// Task 1: Flush the cache so that we can do our measurement :)
void flush_all_caches()
{
    // Your code goes here
}

void *load_matrix_A(void *value)
{
    long i;
    int *num = (int *) value;
    int fd= open("./input1.in", O_RDONLY);
    char buf[MAXBUF];
    ssize_t numRead;

    //count number of lines to get row and column dimension of matrix
    //since it is a square matrix, we only need number of lines
    while ((numRead = read(fd, buf, MAXBUF)) > 0) {
        for (int i = 0; i < numRead; i++) {
            if (buf[i] == '\n') {
                *num+=1;
            }
        }
    }
    printf("DEBUG: Number of lines %d\n", *num);

    huge_matrixA = malloc(sizeof(long)*(long)SIZEX*(long)SIZEY);
    // Load the input
    // Note: This is suboptimal because each of these loads can be done in parallel.

    for(i=0;i<((long)SIZEX*(long)SIZEY);i++)
    {
        fscanf(fin1,"%ld", (huge_matrixA+i));
    }
    return NULL;
}

void *load_matrix_B(void *value)
{
    long i;
    int *num = (int *) value;
    huge_matrixB = malloc(sizeof(long)*(long)SIZEX*(long)SIZEY);
    // Load the input
    // Note: This is suboptimal because each of these loads can be done in parallel.

    for(i=0;i<((long)SIZEX*(long)SIZEY);i++)
    {
        fscanf(fin2,"%ld", (huge_matrixB+i));
        num++;
    }
    return NULL;
}

void *set_matrix_C(void *value)
{
    huge_matrixC = malloc(sizeof(long)*(long)SIZEX*(long)SIZEY);
    int *num = (int *) value;
    long i;
    for(i=0;i<((long)SIZEX*(long)SIZEY);i++)
    {
        huge_matrixC[i] = 0;
        num++;
    }
    return NULL;
}

void load_matrix_base()
{
    pthread_t thread;
    int lengthA = 0;
    int lengthB = 0;
    int lengthC = 0;
    pthread_create(&thread, NULL, load_matrix_A, &lengthA);
    pthread_create(&thread, NULL, load_matrix_B, &lengthB);
    pthread_create(&thread, NULL, set_matrix_C, &lengthC);
    pthread_join(thread, NULL);
    //Number of lines squared will give us length of matrix
    //Only required for one matrix as all matrix has the same dimensions
    lengthA *= lengthA;
    length = lengthA;
    printf("Length of each matrix = %d\n", length);

//    load_matrix_A();
//    load_matrix_B();
//    set_matrix_C();
    // Load the input
    // Note: This is suboptimal because each of these loads can be done in parallel.

}

void free_all()
{
    free(huge_matrixA);
    free(huge_matrixB);
    free(huge_matrixC);
}

void multiply_base()
{
    size_t n = sizeof(*huge_matrixA)/sizeof(huge_matrixA[0]);
    printf("Test print\n");
    printf("Length of huge_matrixA is %ld\n", n);


    // Your code here
    //
    // Implement your baseline matrix multiply here.

}

void compare_results()
{
    fout = fopen("./out.in","r");
    long i;
    long temp1, temp2;
    for(i=0;i<((long)SIZEX*(long)SIZEY);i++)
    {
        fscanf(fout, "%ld", &temp1);
        fscanf(fout, "%ld", &temp2);
        if(temp1!=temp2)
        {
            printf("Wrong solution!");
            exit(1);
        }
    }
    fclose(fout);
    fclose(ftest);
}

void write_results()
{
    // Your code here
    //
    // Basically, make sure the result is written on fout
    // Each line represent value in the X-dimension of your matrix
}

void load_matrix()
{

    // Your code here
}



void multiply()
{
    // Your code here
}

int main()
{
    free_all();
    clock_t s,t;
    double total_in_base = 0.0;
    double total_in_your = 0.0;
    double total_mul_base = 0.0;
    double total_mul_your = 0.0;
    fin1 = fopen("./input1.in","r");
    fin2 = fopen("./input2.in","r");
    fout = fopen("./out.in","w");
    ftest = fopen("./reference.in","r");


    flush_all_caches();

    s = clock();
    load_matrix_base();
    t = clock();
    total_in_base += ((double)t-(double)s) / CLOCKS_PER_SEC;
    printf("[Baseline] Total time taken during the load = %f seconds\n", total_in_base);

    s = clock();
    multiply_base();
    t = clock();
    total_mul_base += ((double)t-(double)s) / CLOCKS_PER_SEC;
    printf("[Baseline] Total time taken during the multiply = %f seconds\n", total_mul_base);
    fclose(fin1);
    fclose(fin2);
    fclose(fout);
    free_all();

    flush_all_caches();

    s = clock();
    load_matrix();
    t = clock();
    total_in_your += ((double)t-(double)s) / CLOCKS_PER_SEC;
    printf("Total time taken during the load = %f seconds\n", total_in_your);

    s = clock();
    multiply();
    t = clock();
    total_mul_your += ((double)t-(double)s) / CLOCKS_PER_SEC;
    printf("Total time taken during the multiply = %f seconds\n", total_mul_your);
    write_results();
    fclose(fin1);
    fclose(fin2);
    fclose(fout);
    free_all();
//	compare_results();

    return 0;
}
