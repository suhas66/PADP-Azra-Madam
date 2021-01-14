#include<stdio.h>
#include<omp.h>
#include<ctype.h>
#include<string.h>
#define COUNT 6
char search_words[COUNT][20] = {"parallel","chapter","from","The","be","a"};
long counts[COUNT];
int line_c = 0;
int is_equal(char* a,const char* key, int ignore_case){
int len_a = strlen(a),len_b = strlen(key);
if(len_a != len_b)return 0;
if(ignore_case)return strcasecmp(a,key)==0;
return strcmp(a,key)==0;
}
void read_word(char *temp, FILE *fp){
int i=0; char ch;
while((ch = fgetc(fp)) != EOF && isalpha(ch) == 0);
while(ch != EOF && isalpha(ch) != 0){
temp[i++] = ch;
ch = fgetc(fp);
}temp[i] = '\0';
}
long determine_count(const char *file_name, const char *key, int ignore_case){
int key_index=0,key_len = strlen(key),i=0;
long word_count=0;
char ch,temp[40];
FILE *fp = fopen(file_name,"r");
while(feof(fp) == 0){
read_word(temp,fp);
if(is_equal(temp,key,ignore_case) != 0)
word_count++;
//printf("%s ",temp);
} //printf("\nWord %s: %ld",key,word_count);
return word_count;
}
int main(){
int i;
char* my_files[5]={"file1.txt","file2.txt","file3.txt","file4.txt","file5.txt"};
for(i=0;i<COUNT;i++) counts[i] = 0;
printf("Size\t\tT1\t\tT2\t\tT4\t\tT8");
for(int iter=0; iter<5; iter++){
FILE *fp = fopen(my_files[iter],"r");
fseek(fp, 0L, SEEK_END); //printf("\nFile size: %.2lfKB\n",ftell(fp)/1024.0);
printf("\n%.2lfKB\t",ftell(fp)/1024.0);
if(iter<3)
printf("\t");
fclose(fp);
for(int t=1; t<=8; t*=2){ //printf("\n%d Threads: ",t);
omp_set_num_threads(t);
double start = omp_get_wtime();
#pragma omp parallel for
for(i=0;i<COUNT;i++)
counts[i] = determine_count(my_files[iter],search_words[i],1);
double end = omp_get_wtime() - start;
// for(i=0;i<COUNT;i++)
// printf("%s: %ld ",search_words[i],counts[i]);
printf("%lfs\t",end);
}}
printf("\n");
}