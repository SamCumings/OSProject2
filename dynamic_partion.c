//dynamic memory
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>

#define TOTAL_MEM 104857600
#define SMALLEST_SIZE 10240
#define LARGEST_SIZE 10485760
#define NUM_PROC 50
#define max(a, b) (((a)>(b))?(a):(b))
#define min(a, b) (((a)<(b))?(a):(b))

typedef struct Process Process;
typedef struct Node Node;
struct Process{
    int proc_num;
    int run_time;
    int end_time;
    size_t mem_size;
    char* mem_loc;
};
struct Node{
    Process Pro;
    struct Node * Left;
    struct Node * Right;
};

void* my_alloc(size_t size);

void my_free(void * ptr);
double log2(double input);
int size_level(size_t);
int left(int i);
int right(int i);
int parent(int i);
bool power_of_2(int i);
size_t next_power_of_2(size_t size);

//binary tree functions (as array)
int left(int i){
    return 2*i + 1;
}
int right(int i){
    return 2*i + 2;
}
int parent(int i){
    return (i-1)/2;
}
//in bits if i is power of 2 then it is 10000000 so -1 is 0111111111 so bitwise and will always produce 0.
bool power_of_2(int i){
    return !(i& (i- 1)); 
}
// get the next power of 2
size_t next_power_of_2(size_t size){
    size--;
    size |= size >> 1;
    size |= size >> 2;
    size |= size >> 4;
    size |= size >> 8;
    size |= size >> 16;
    size++;
    return size;
    
}


int size_level(size_t mem_size){
    double level_exact = log2((double) mem_size);
    return (int)ceil(level_exact);
}
double log2(double input){
    return log(input)/log(2);
}

void * my_alloc(size_t size){

    return NULL;
}
void my_free(void * ptr){

}

int main(int argc, char *argv[]){
    clock_t start_mal,end_mal,start_free,end_free,total_time=0;
    int cycle = 0;
    int processes_started = 0;
    int processes_finished=0;
    long mem_size=0;
    Process Processes[50];
    int i=0;
    printf("sizeof: %ld",SMALLEST_SIZE*(long)sizeof(Node));
    size_t total_mem = TOTAL_MEM;

    void * start_of_mem = NULL;
    char * start_of_meta = NULL;
    start_of_mem = malloc(total_mem);
    start_of_meta=start_of_mem;
    
    //80mb = 83886080 bytes
    //491520 total size of meta structure

    srand(30);
    for(i=0;i<50;i++){
        Processes[i].run_time=(rand()%(2500-200)+200);
        printf("Process_time: %d\n",Processes[i].run_time);
        Processes[i].mem_size=(size_t)(rand()%(LARGEST_SIZE-SMALLEST_SIZE)+SMALLEST_SIZE);    
        printf("Process_size: %ld\n",Processes[i].mem_size);
        Processes[i].end_time=-1;
    }
    for (cycle = 0;processes_finished<50;cycle++){
        if(cycle%50==0&&processes_started<50){ 
            start_mal=clock();
            mem_size+=Processes[processes_started].mem_size;
            printf("mem_size= %ld\n",mem_size);
            Processes[processes_started].mem_loc=malloc(Processes[processes_started].mem_size);

            Processes[processes_started].proc_num=processes_started;
            *Processes[processes_started].mem_loc='a';
            end_mal=clock();
            total_time+=end_mal-start_mal;
            Processes[processes_started].end_time=cycle+Processes[processes_started].run_time;
            processes_started++;
        }
        //need to make more efficient
        for(i=0;i<50;i++){
            if(Processes[i].end_time==cycle){
                printf("i= %d\n",i);
                printf("end_time= %d\n",Processes[i].end_time);
                printf("cycle %d\n",cycle);
                start_free=clock();
                free(Processes[i].mem_loc);
                end_free=clock();
                total_time+=end_free-start_free;
                processes_finished++;
                printf("processes_finished= %d\n",processes_finished);
            }
        }
    }
    printf("Time: %ld\n",total_time);
    return 0;
}
