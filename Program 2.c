#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
int main(){
int it=1;
printf("The execution times are\nSize\t\t1\t\t\t2\t\t\t4\t\t\t8\n");
while(it<=4){
int r = 500*it, c = 500*it, i, j, sum =0, k;
//dynamically allocate arrays
int **arr1 = (int **)malloc(r * sizeof(int *));
for (i=0; i<r; i++) arr1[i] = (int *)malloc(c * sizeof(int));
int **arr2 = (int **)malloc(r * sizeof(int *));
for (i=0; i<r; i++) arr2[i] = (int *)malloc(c * sizeof(int));
int **arr3 = (int **)malloc(r * sizeof(int *));
for (i=0; i<r; i++) arr3[i] = (int *)malloc(c * sizeof(int));
#pragma omp parallel for collapse(2)
for(i = 0;i < r; i++)
for(j = 0;j < c; j++)
arr1[i][j] = rand()/r;
#pragma omp parallel for collapse(2)
for(i = 0;i < r; i++)
for(j = 0;j < c; j++)
arr2[i][j] = rand()/r;
double start = omp_get_wtime();
for(i = 0;i < r; i++)
for(j = 0;j < c; j++)
for(k = 0;k < r; k++)
arr3[i][j] += arr1[i][k] * arr2[k][j];
double end = omp_get_wtime();
printf("%d\t\t%lf\t\t",r,end-start);
for(int p=2;p<=8;p=p*2){
omp_set_num_threads(p);
double start = omp_get_wtime();
#pragma omp parallel for private(j, k)
for(i = 0;i < r; i++)
for(j = 0;j < c; j++){
arr3[i][j]=0;
for(k = 0;k < r; k++)
arr3[i][j] += arr1[i][k] * arr2[k][j];
}
double end = omp_get_wtime();
printf("%lf\t\t", end-start);
}
printf("\n");
it++;
}return 0;
}