#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
//#include <malloc.h>
#include<stdbool.h>

#include <pthread.h>
/* This display function is what displays all the information such as the value of the matrixes, total time elapsed and if they are the same*/
void Display_info(int dimensions, float* mat);
/*
 This function is responsible for generating random numbers to fill the matix
 */
void Fill_Matrix(int size, float* data);
/* This Function is the compasrison in which we multiply the matrix manually and get a time stamp of the total time*/
void No_Thread_Matrix_Mult(int dimensions, float* Dummy_Martix1, float* Dummy_Martix2, float* Product, float* No_Thread_Matrix_Product);
/* This Function is what takes in the input of the user and multioples the matrix with the number of threads and gives a time stamp on the total time elapsed*/
void Thread_Matrix_Mult(int dimensions, float* Dummy_Martix1, float* Dummy_Martix2, float* Product, int noThread);
/* This Function comapres the result of the matrix with no threads and with multiple threads if they are the same it tells us the matrixes are the same*/
void Compare_Results(int dimensions,float * No_Thread_Matrix_Product , float * Product);

int main(int argc, char* argv[])
{
    // this is the default matrix dimnesion size
int dimensions = 500;
    //This is the default max number of threds that can be asked for
int noThread = 500;

    // this will keep track of the total time each matrix respectivley
struct timeval t1, t2;
    // this is how the compiler gets the value
float elapsed_time = 0.F;

    // make sure the input is in the right way like size and then number of threads
if (argc < 2) {
printf("Wrong Input: %s <dimensions> <no_threads>\n", argv[0]);
exit(0);
}
// takes in the value from the user and sets the size of the matrix
dimensions = atoi(argv[1]);
if (dimensions > 1000)
dimensions = 1000;
    // takes the 2nd argument from the command line and sets that to the number of threads
noThread = atoi(argv[2]);
// this will make it so that the random number generated are truly random
srand(time(NULL));

    /* Dummy_matrix1 and Dummy_Matrix2 are the function that will have random numbers generated and placed in them this will also save a copy later on so we can comare the results later on*/
float* Dummy_Martix1 = (float*)malloc(dimensions * dimensions * sizeof(float));
float* Dummy_Martix2 = (float*)malloc(dimensions* dimensions * sizeof(float));
    // The product function multiples the two dummy matrixes and saves their product
float* Product = (float*)malloc(dimensions * dimensions * sizeof(float));
    //Just like the periovous function and we need 2 so we can compare the differnce or similarity between them
float* No_Thread_Matrix_Product = (float*)malloc(dimensions * dimensions* sizeof(float));
if (Dummy_Martix1 == NULL || Dummy_Martix2 == NULL || Product == NULL) {
printf("Failed to allocate memory.\n");
exit(-1);
}

Fill_Matrix(dimensions * dimensions, Dummy_Martix1);
Fill_Matrix(dimensions * dimensions, Dummy_Martix2);
// checks if the number entered is less than the default we have set for optimal speed
if (dimensions <= 500) {
printf("Dummy_Matrix1 = \n");
Display_info(dimensions, Dummy_Martix1);

printf("Dummy_Matrix2 = \n");
Display_info(dimensions, Dummy_Martix2);
}
// this is how we are getting the total time elaplsed by both the matrixes
gettimeofday(&t1, NULL);
// multipy the random matrixes without any threads so we get a baseline of the answer and the time
No_Thread_Matrix_Mult(dimensions, Dummy_Martix1, Dummy_Martix2, Product, No_Thread_Matrix_Product);

gettimeofday(&t2, NULL);
elapsed_time = (t2.tv_sec + t2.tv_usec / 1000000.) - (t1.tv_sec + t1.tv_usec / 1000000.);
printf("elapsed time (single thread)\t= %15f sec\n", elapsed_time);

    // again checks if the dimension is of a alid size  so the threads arent a large number and take for ever to do
if (dimensions <= 500) {
printf("Dummy_Matrix1 * Dummy_Matrix2 (single thread) = \n");
Display_info(dimensions, No_Thread_Matrix_Product);
}
// get the total time
gettimeofday(&t1, NULL);
// multiply the two matrixes with threads and then store the results
Thread_Matrix_Mult(dimensions, Dummy_Martix1, Dummy_Martix2, Product, noThread);
gettimeofday(&t2, NULL);
elapsed_time = (t2.tv_sec + t2.tv_usec / 1000000.) - (t1.tv_sec + t1.tv_usec / 1000000.);
printf("elapsed time (%d threads)\t= %15f sec\n", noThread, elapsed_time);

if (dimensions <= 500) {
printf("Dummy_Matrix1* Dummy_Matrix2 (%d threads) = \n", noThread);
Display_info(dimensions, Product);
}
// this will comapre the reults to see if they are the same or not and then it frees the memory that was allocated to the program in the //begenning so we woudnt run into segmentaion faults
Compare_Results(dimensions,No_Thread_Matrix_Product,Product);
free(Dummy_Martix1);
free(Dummy_Martix2);
// free(Product);

return 0;
}
/* this is the function that displays all the infor in the code such as the dimesions, values and if theyre equal of not*/
void Display_info(int dimensions, float* mat)
{
if (dimensions>= 20)
return;
    /* this uses a forloop to go though each location in the matrix and print it on the screen in its respective location, like a 2d array*/
int i, j;
for (i = 0; i < dimensions; i++) {
for (j = 0; j < dimensions; j++)
printf("%.2f ", mat[i * dimensions + j]);
printf("\n");
}
}
/* works simialr to the display function where its set up like a 2D array and it comapres each number with the other matix and makes sure they are the same, By default its set to false and chances when every thing is the same*/
void Compare_Results(int dimensions,float * No_Thread_Matrix_Product , float * Product)
{
 int i,j;
bool check=false;
for(i=0;i<dimensions;i++){
for(j=0;j<dimensions;j++){

// if they are both the same then we make check rue and exit this function
if(No_Thread_Matrix_Product[i*dimensions+j]==Product[i*dimensions+j])
check=true;}}
if(check==true)
printf("success  the Matrices are same \n ");
else
printf("Matrices are not same ");

}
// This is the function that generates random number between 0-29 and puts them in the matix, its a lownumber so its doesnt take too long to multiply and calculate the values
void Fill_Matrix(int size, float* data)
{
int i = 0;
for (i = 0; i < size; i++)
data[i] = rand() % 10;;
}

// its gets the two dummy matrixes and it multoples them same as a 2D array fassion and stores the values in another matrix.
void No_Thread_Matrix_Mult(int dimensions, float* Dummy_Martix1, float* Dummy_Martix2, float* Product, float *No_Thread_Matrix_Product)
{
int i = 0, j = 0, k = 0;

for (i = 0; i < dimensions; i++) {
for (j = 0; j < dimensions; j++) {
Product[i * dimensions + j] = 0.F;
No_Thread_Matrix_Product[i * dimensions + j] = 0.F;

for (k = 0; k < dimensions; k++)
{Product[i * dimensions + j] += Dummy_Martix1[i * dimensions + k] * Dummy_Martix2[k * dimensions+ j];
No_Thread_Matrix_Product[i *dimensions + j] += Dummy_Martix1[i * dimensions+ k] * Dummy_Martix2[k * dimensions+ j];
}}
}
}

typedef struct {
int id;
int noThread;
int dimensions;
float* Dummy_Martix1, * Dummy_Martix2, * Product;
} ThreadInfo;
// this is the final matrix thats stored and later used to see if the two matrixes match up
void* Final_Thread_Multiply(void* param);

// Worls similar to the no thread function where it will go in and multiply but this uses threads to do it and it uses a mutex so the threads cant
// interfeer with eachother so this gives more and more overhead as it gets biger and bigger
void Thread_Matrix_Mult(int dimensions, float* Dummy_Martix1, float* Dummy_Martix2, float* Product, int noThread)
{
int i = 0;
pthread_t* tid = (pthread_t*)malloc(sizeof(pthread_t) * noThread);
ThreadInfo* threadInfo = (ThreadInfo*)malloc(sizeof(ThreadInfo) * noThread);
pthread_attr_t attr;
pthread_attr_init(&attr);

for (i = 0; i < noThread; i++) {
threadInfo[i].id = i;
threadInfo[i].noThread = noThread;
threadInfo[i].dimensions = dimensions;
threadInfo[i].Dummy_Martix1 = Dummy_Martix1;
threadInfo[i].Dummy_Martix2 = Dummy_Martix2;
threadInfo[i].Product = Product;


pthread_create(&tid[i], &attr, Final_Thread_Multiply, &threadInfo[i]);
}

for (i = 0; i < noThread; i++)
pthread_join(tid[i], NULL);
free(tid);
free(threadInfo);
}

// this is the function that stores the final answer and it gets the matching values from both the matrixes and stores them in the respecive index
void* Final_Thread_Multiply(void* param)
{
ThreadInfo* pInfo = (ThreadInfo*)param;
int i = 0, j = 0, k = 0;
int row_start = pInfo->id * (pInfo->dimensions / pInfo->noThread);
int row_end = (pInfo->id + 1) * (pInfo->dimensions/ pInfo->noThread);

for (i = row_start; i < row_end; i++) {
for (j = 0; j < pInfo->dimensions; j++) {
pInfo->Product[i * pInfo->dimensions + j] = 0.F;
for (k = 0; k < pInfo->dimensions; k++) {
pInfo->Product[i * pInfo->dimensions+ j] += pInfo->Dummy_Martix1[i * pInfo->dimensions + k] * pInfo->Dummy_Martix2[k * pInfo->dimensions+ j];
}
}
}
pthread_exit(0);
}
