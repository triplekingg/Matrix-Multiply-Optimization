#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "mm.h"



// Task 1: Flush the cache so that we can do our measurement :)
void flush_all_caches()
{
    // Your code goes here

    //ref: https://stackoverflow.com/questions/11277984/how-to-flush-the-cpu-cache-in-linux-from-a-c-program
    for (long i = 0; i < (length); i++){
        asm volatile("clflush (%0)\n\t"
        :
        : "r"(huge_matrixA + i)
        : "memory");
        asm volatile("clflush (%0)\n\t"
        :
        : "r"(huge_matrixB + i)
        : "memory");
        asm volatile("clflush (%0)\n\t"
        :
        : "r"(huge_matrixC + i)
        : "memory");

    }

    asm volatile("sfence\n\t" ::: "memory");
}

void printMatrix(long *matrix){
    int i = 0;
    for(int j = 0; j<dimensions; j++){
        for(int k = 0; k<dimensions; k++){
            printf("%ld ", matrix[i]);
            i++;
        }
        printf("\n");
    }
}

void getDimensions(){
    int num = 0;
    int fd= open("./input1.in", O_RDONLY);
    char buf[MAXBUF];
    ssize_t numRead;

    //count number of lines to get row and column dimension of matrix
    //since it is a square matrix, we only need number of lines
    while ((numRead = read(fd, buf, MAXBUF)) > 0) {
        for (int i = 0; i < numRead; i++) {
            if (buf[i] == '\n') {
                num+=1;
            }
        }
    }
//    printf("DEBUG: Number of lines %d\n", num);
    length = num * num;
    dimensions = num;
    close(fd);

}

void load_matrix_base()
{
    getDimensions();
    long i;
    huge_matrixA = malloc(sizeof(long)*(long)SIZEX*(long)SIZEX);
    huge_matrixB = malloc(sizeof(long)*(long)SIZEX*(long)SIZEX);
    huge_matrixC = malloc(sizeof(long)*(long)SIZEX*(long)SIZEX);
    // Load the input
    // Note: This is suboptimal because each of these loads can be done in parallel.
    for(i=0;i<((long)SIZEX*(long)SIZEX);i++)
    {
        fscanf(fin1,"%ld", (huge_matrixA+i));
        fscanf(fin2,"%ld", (huge_matrixB+i));
        huge_matrixC[i] = 0;
    }
}

void free_all()
{
    free(huge_matrixA);
    free(huge_matrixB);
    free(huge_matrixC);
}

void multiply_base()
{
    for (int i = 0; i < dimensions; i++) {
        for (int j = 0; j < dimensions; j++) {
            long sum = 0.0;
            for (int k = 0; k < dimensions; k++)
                sum = sum + huge_matrixA[i * dimensions + k] * huge_matrixB[k * dimensions + j];
            huge_matrixC[i * dimensions + j] = sum;
        }
    }

    // Your code here
    //
    // Implement your baseline matrix multiply here.

}

void compare_results()
{fout = fopen("./out.in","r");
    long i;
    long temp1, temp2;
    for(i=0;i<length;i++)
    {
        fscanf(fout, "%ld", &temp1);
        fscanf(ftest, "%ld", &temp2);
//        printf("DEBUG: %ld = %ld \n", temp1, temp2);
        if(temp1!=temp2)
        {
            printf("Wrong solution!\n");
            exit(1);
        }
    }
    printf("Solution is correct\n");
    fclose(fout);
    fclose(ftest);
}

void write_results()
{
    // Your code here
    // Basically, make sure the result is written on fout
    // Each line represent value in the X-dimension of your matrix
    fout = fopen("./out.in","w");
    for (int i = 0; i < dimensions; i++){
        for (int j = 0; j < dimensions; j++){
            char buff[MAXBUF];
            sprintf(buff, "%ld ", huge_matrixC[i * dimensions + j]);
            fwrite(buff, 1, strlen(buff), fout);
        }
        fwrite("\n", 1, strlen("\n"), fout);
    }
    fclose(fout);
}

void load_matrix()
{
    fin1 = fopen("./input1.in","r");
    fin2 = fopen("./input2.in","r");
    getDimensions();
    long i;
    huge_matrixA = malloc(sizeof(long)*(long)dimensions*(long)dimensions);
    huge_matrixB = malloc(sizeof(long)*(long)dimensions*(long)dimensions);
    huge_matrixC = malloc(sizeof(long)*(long)dimensions*(long)dimensions);
    // Load the input
    // Note: This is suboptimal because each of these loads can be done in parallel.
    for(i=0;i<((long)dimensions*(long)dimensions);i++)
    {
        fscanf(fin1,"%ld", (huge_matrixA+i));
        fscanf(fin2,"%ld", (huge_matrixB+i));
        huge_matrixC[i] = 0;
    }

    // Your code here
}



void multiply()
{
    // Your code here
    int i,j,k;
    for(i = 0; i < dimensions; i+= BLOCKSIZE){
        for(j = 0; j < dimensions; j += BLOCKSIZE){
            for(k = 0; k < dimensions; k += BLOCKSIZE){
                for(int i1 = i; i1 < i+BLOCKSIZE;i1++){
                    for(int j1 = j; j1 < j+BLOCKSIZE;j1++){
                        for(int k1 = k; k1 < k+BLOCKSIZE;k1++){
                            huge_matrixC[i1*dimensions + j1] += huge_matrixA[i1*dimensions + k1]*huge_matrixB[k1*dimensions + j1];
                        }
                    }
                }
            }
        }
    }
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
//    printMatrix(huge_matrixA);

    s = clock();
    multiply_base();
    t = clock();
    total_mul_base += ((double)t-(double)s) / CLOCKS_PER_SEC;
    printf("[Baseline] Total time taken during the multiply = %f seconds\n", total_mul_base);
//    printMatrix(huge_matrixC);
    fclose(fin1);
    fclose(fin2);
    fclose(fout);
//    free_all();

    flush_all_caches();

    s = clock();
    load_matrix();
    t = clock();
    total_in_your += ((double)t-(double)s) / CLOCKS_PER_SEC;
    printf("Total time taken during the load = %f seconds\n", total_in_your);
//    printMatrix(huge_matrixA);

    s = clock();
    multiply();
    t = clock();
    total_mul_your += ((double)t-(double)s) / CLOCKS_PER_SEC;
    printf("Total time taken during the multiply = %f seconds\n", total_mul_your);
//    printMatrix(huge_matrixC);
    write_results();
    fclose(fin1);
    fclose(fin2);
    fclose(fout);
    free_all();
    compare_results();

    return 0;
}
