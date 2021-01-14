#include<mpi.h>
#include<stdlib.h>
#include<stdio.h>
void ring_io ( int p, int id ){
int i,j,n,test,dest,source,n_test_num = 5,test_num = 10;
int n_test[5] = { 100, 1000, 10000, 100000, 1000000 };
double tave, tmax, tmin, wtime, *x;
MPI_Status status;
if (id == 0){
printf("\n Timings based on %d experiments\n", test_num );
printf(" N double precision values were sent in a ring transmission starting\n" );
printf(" and ending at process 0 and using a total of %d processes.\n", p );
printf("\n N T min T ave T max\n\n" );
}
for ( i = 0; i < n_test_num; i++ ){
n = n_test[i];
x = (double *)malloc(n * sizeof(double));
if (id == 0){
dest = 1;
source = p - 1;
tave = 0.0;
tmin = 1.0E+30;
tmax = 0.0;
for ( test = 1; test <= test_num; test++ ){
for ( j = 0; j < n; j++ )
x[j] = ( double ) ( test + j );
wtime = MPI_Wtime();
MPI_Send(x, n, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD );
MPI_Recv(x, n, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, &status );
wtime = MPI_Wtime() - wtime;
tave = tave + wtime;
if(wtime < tmin) tmin = wtime;
if(tmax < wtime) tmax = wtime;
}
tave = tave/(double)test_num;
printf(" %8d %14.6g %14.6g %14.6g\n", n, tmin, tave, tmax );
}else{
source = id - 1;
dest = ( id + 1 )%p;
for ( test = 1; test <= test_num; test++ ){
MPI_Recv(x, n, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, &status);
MPI_Send(x, n, MPI_DOUBLE, dest, 0, MPI_COMM_WORLD);
}
}
free (x);
}
}

int main(int argc, char **argv){
int error,id,p;
MPI_Init ( &argc, &argv );
MPI_Comm_size ( MPI_COMM_WORLD, &p );
MPI_Comm_rank ( MPI_COMM_WORLD, &id );
if ( id == 0 ){
printf("\nRING_MPI:\n C/MPI version\n" );
printf( " Measure time required to transmit data around a ring of processes\n" );
printf( "\n The number of processes is %d\n", p );
}
ring_io ( p, id );
MPI_Finalize ( );
if ( id == 0 )
printf ( "\nRING_MPI:\n Normal end of execution.\n" );
}