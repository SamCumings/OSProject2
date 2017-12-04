//dynamic memory
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>
//100MB
#define TOTAL_MEM 104857600
//10KB
#define SMALLEST_SIZE 10240
//2MB
#define LARGEST_SIZE 2097152
#define NUM_PROC 50
#define max(a, b) (((a)>(b))?(a):(b))
#define min(a, b) (((a)<(b))?(a):(b))

char * g_tot_memory;

typedef struct Process Process;
typedef struct Node Node;
typedef struct Free_Node Free_Node;
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

struct Free_Node{
    int level;
    int free;
    Free_Node * Left;
    Free_Node * Right;
};

void* my_alloc(size_t size);

void my_free(void * ptr);
double log2(double input);
int size_level(size_t);
int left(int i);
int right(int i);
int parent(int i);
int power_of_2(int i);
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
int power_of_2(int i){
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
void*  my_alloc(size_t size){
    size_t alloc_size = size;
    if(!power_of_2(size))
        alloc_size=next_power_of_2(size);
    

}

void my_free(void * ptr){

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
    printf("sizeof: %ld\n",SMALLEST_SIZE*(long)sizeof(Node));
    size_t total_mem = TOTAL_MEM;

    char * start_of_mem = NULL;
    char * start_of_meta = NULL;

    //memory alloc
    start_of_mem = malloc(total_mem);

    g_tot_memory=start_of_mem;

    start_of_meta=start_of_mem;
    
    //80mb = 83886080 bytes
    //491520 total size of meta structure

    //test space

    printf("power of 2 test %d: %d\n",32,power_of_2(32));
    
    printf("power of 2 test %d: %d\n",33,power_of_2(33));
    
    printf("next power of 2 test %d: %ld\n",24,(long)next_power_of_2(24));

    printf("next power of 2 test %d: %ld\n",32,(long)next_power_of_2(32));

    printf("next power of 2 test %d: %ld\n",1048576,(long)next_power_of_2(1048576));

    printf("power of 2 test %d: %d\n",1048576,power_of_2(1048576));

    printf("power of 2 test %d: %d\n",33554432,power_of_2(33554432));

    printf("power of 2 test %d: %d\n",10240,power_of_2(10240));
    
    printf("next power of 2 test %d: %zu\n",10240,next_power_of_2(10240));
   
    printf("next power of 2 test %d: %zu\n",2097152,next_power_of_2(2097152));

    printf("power of 2 test %d: %d\n",2097152,power_of_2(2097152));




    srand(30);
    for(i=0;i<50;i++){
        Processes[i].run_time=(rand()%(2500-200)+200);
        //printf("Process_time: %d\n",Processes[i].run_time);
        Processes[i].mem_size=(size_t)(rand()%(LARGEST_SIZE-SMALLEST_SIZE)+SMALLEST_SIZE);    
        //printf("Process_size: %ld\n",Processes[i].mem_size);
        Processes[i].end_time=-1;
    }
    for (cycle = 0;processes_finished<50;cycle++){
        if(cycle%50==0&&processes_started<50){ 
            start_mal=clock();
            mem_size+=Processes[processes_started].mem_size;
           //printf("mem_size= %ld\n",mem_size);
            Processes[processes_started].mem_loc=my_alloc(Processes[processes_started].mem_size);

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
                //printf("i= %d\n",i);
                //printf("end_time= %d\n",Processes[i].end_time);
                //printf("cycle %d\n",cycle);
                start_free=clock();
                my_free(Processes[i].mem_loc);
                end_free=clock();
                total_time+=end_free-start_free;
                processes_finished++;
                //printf("processes_finished= %d\n",processes_finished);
            }
        }
    }
    printf("Time: %ld\n",total_time);
    return 0;
}
