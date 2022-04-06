#define SIZEX 22000
#define SIZEY 22000

//Additional Definitions
#define MAXBUF 1000
#define BLOCKSIZE 100

long * huge_matrixA;
long * huge_matrixB;
long * huge_matrixC;

FILE *fin1, *fin2, *fout, *ftest;

void flush_all_caches();
void load_matrix_base();
void free_all();
void multiply_base();
void compare_results();

// Your job is to finish these three functions in the way that it is fast
void write_results();
void load_matrix();
void multiply();

// Any additional variables needed go here
int length;
int dimensions;

// Any helper functions go here
void printMatrix(long *matrix);
void getDimensions();
void *load_matrix_A();
void *load_matrix_B();
void *set_matrix_C();
