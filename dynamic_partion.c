//dynamic memory
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>
//100MB = 104857600
//128MB = 134217728 
#define TOTAL_MEM 134217728
//10KB = 10240
//16KB = 16384
#define SMALLEST_SIZE 16384 
//2MB
#define LARGEST_SIZE 2097152
#define NUM_PROC 50

#define NUM_SMALLEST_PAR TOTAL_MEM/SMALLEST_SIZE
#define max(a, b) (((a)>(b))?(a):(b))
#define min(a, b) (((a)<(b))?(a):(b))

char * g_tot_memory;


typedef struct Process Process;
typedef struct Node Node;
typedef struct Free_Node Free_Node;
typedef struct LLNode LLNode;
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
    int free;
    int split;
    int used;
    char * mem_loc;
    Free_Node * Left;
    Free_Node * Right;
};
struct LLNode{
    int free;
    char * position;
    int l_index;
    int r_index;
};
LLNode link_array[NUM_SMALLEST_PAR*2-1];
Free_Node meta_array[NUM_SMALLEST_PAR*2-1];

void* my_alloc(size_t size);

void my_free(void * ptr);
double log2(double input);
int size_level(size_t);
int left(int i);
int right(int i);
int parent(int i);
int power_of_2(int i);
size_t next_power_of_2(size_t size);
int get_level(size_t size);

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

void insert_link(int level,int index){
    int level_index=(1<<level)-1;
    int temp;
    printf("inside split level: %d,index %d\n",level,index);
    if(link_array[level_index].l_index==link_array[level_index].r_index==level_index){
        link_array[level_index].l_index=index;
        link_array[level_index].r_index=index;
        //link_array[index].l_index=level_index;
        //link_array[index].r_index=level_index;
    } else {
        //fix left
        temp=link_array[level_index].l_index;
        link_array[level_index].l_index=index;
        link_array[index].l_index=temp;
        //fix right
        link_array[temp].r_index=index;
        link_array[index].r_index=level_index;
    }

}
// 168mb / 16 kb = 10752 bytes
void*  my_alloc(size_t size){
    //printf("allocing \n");
    size_t alloc_size = size;
    int level_index = 0;
    int level=0;
    int i;
    int index=0;
    int temp_level=0;
    
    printf("allocing %ld\n",size);
    
    if(!power_of_2(size))
        alloc_size=next_power_of_2(size);
    level_index = (TOTAL_MEM / alloc_size) - 1;

    printf("power of two %d\n",power_of_2(TOTAL_MEM / alloc_size));

    level=floor(log2(level_index+1));
    printf("power of two alloc_size= %ld\n",alloc_size); 
    int start = index=level_index;
    //check if there is a free list there
    for(index=level_index,temp_level=level;;){
        //if you find a good spot on this level use it.
        printf("alloc_size: %ld, attempt size: %d\n",alloc_size,TOTAL_MEM/(1<<temp_level)); 
        if(link_array[index].free && TOTAL_MEM/(1<<temp_level)==alloc_size){
            printf("index= %d\n",index);
            link_array[index].free=0;
            meta_array[index].used=1;
            meta_array[index].free=0;
            break;
        }else if(link_array[index].r_index==start){
            //none good on this level
            temp_level--;
            index=(1<<temp_level)-1;
            start=index;
            printf("none good on this level index= %d\n",index);
        }else{
            printf("1    go to next link index= %d\n",index);
            index=link_array[index].r_index;
            printf("2 go to next link index= %d\n",index);
        }
        if(link_array[index].free && TOTAL_MEM/(1<<temp_level)>alloc_size){
            //split
            link_array[index].free=0;
            insert_link(temp_level+1,left(index));
            insert_link(temp_level+1,right(index));
            link_array[left(index)].free=1;
            link_array[right(index)].free=1;
            index=left(index);
            start=index;
            temp_level++;

            printf("split index= %d\n",index);
        }
    }
    //use index and temp_level to determine where in memory this is
    printf("done with an alloc\n");
    return  link_array[index].position=g_tot_memory+(TOTAL_MEM/(1<<temp_level)*(index-((1<<temp_level)-1)));
}

void my_free(void * ptr){
    //stuff for free
    char * start = g_tot_memory;
    char * part_start = ptr - start;

     

}

int size_level(size_t mem_size){
    double level_exact = log2((double) mem_size);
    return (int)ceil(level_exact);
}
double log2(double input){
    return log(input)/log(2);
}


int main(int argc, char *argv[]){
    printf("test\n");
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

    int split;
    int unused;
    //memory alloc
    start_of_mem = malloc(total_mem);

    g_tot_memory=start_of_mem;

    start_of_meta=start_of_mem;
    
    link_array[0].free=1;
    link_array[0].l_index= 0;
    link_array[0].r_index= 0;
    meta_array[0].free=1;
    meta_array[0].split=0;
    meta_array[0].used=0;
    for(i=1;i<NUM_SMALLEST_PAR*2-1;i++){
        //printf("in loop for init\n");    
        link_array[i].free=0;
        link_array[i].l_index= i;
        link_array[i].r_index= i;

        meta_array[i].free=0;
        meta_array[i].split=0;
        meta_array[i].used=0;
    }
    printf("out of loop for init\n");
    
    //80mb = 83886080 bytes
    //491520 total size of meta structure

    //test space
/*
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
*/



    srand(42);
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
            printf("mem_size= %ld\n",mem_size);
            Processes[processes_started].mem_loc=my_alloc(Processes[processes_started].mem_size);
            printf("do we get here?\n");
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
