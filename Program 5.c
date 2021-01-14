#include<stdio.h>
#include<omp.h>
#include<math.h>
#define CLUSTER_SIZE 4
#define POINTS_SIZE 1000
int cluster[CLUSTER_SIZE][2] = {{75, 25}, {25, 25}, {25, 75}, {75, 75}};
long long cluster_count[CLUSTER_SIZE];
int points[POINTS_SIZE][2];
void populate_points() {
for(long long i = 0; i < POINTS_SIZE; i++) {
srand(i);
points[i][0] = rand() % 100;
points[i][1] = rand() % 100;
}
}
double get_distance(int x1, int y1, int x2, int y2) {
int x = x2-x1, y = y2-y1;
return (double)sqrt((x * x) + (y * y));
}
int main() {
double t;
populate_points();
long long i;
int nt = 0;
printf("Number of points is %lld\n",POINTS_SIZE);
printf("T1\t\tT2\t\tT4\t\tT8\n");
for(nt=1;nt<9;nt*=2)
for(i = 0; i < CLUSTER_SIZE; i++)
cluster_count[i] = 0;
t = omp_get_wtime();
#pragma omp parallel for reduction(+:cluster_count) num_threads(nt)
for(i = 0; i < POINTS_SIZE; i++) {
double min_dist = 100, cur_dist = -1;
int j, cluster_index = -1;
for(j = 0; j < CLUSTER_SIZE; j++) {
cur_dist = get_distance(points[i][0], points[i][1], cluster[j][0], cluster[j][1]);
if(cur_dist<min_dist) {
min_dist = cur_dist;
cluster_index = j;
}
}
cluster_count[cluster_index]++;
}
t = omp_get_wtime() - t;
printf("%lf\t", t);
if (nt==8)
for(i = 0; i < CLUSTER_SIZE; i++)
printf("\nCluster (%d, %d): %lld", cluster[i][0], cluster[i][1], cluster_count[i]);
}
printf("\n");
}