#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

//int main(int argc, char *argv[]){
#include <time.h>

#define TOTAL_MEM 1024 * 1024 * 100 //104857600 // 1024 * 1024 * 100 // 104857600 = 100MB
#define SMALLEST_SIZE 1024 * 10 //1024 * 10 // 10240 = 10KB
#define LARGEST_SIZE 1024 * 1024 * 2 // 22097152 = 2MB
#define BIN_SIZE 1024 * 1024 * 5 
#define NUM_PROC 10// should be 50
#define MAX_BINS 20


typedef struct Process Process;
struct Process{
    int proc_num;
    int run_time; 
    int end_time;
    size_t mem_size;
    int r;
//    int total_ranges_allocated;
//    int mem2;
    int size;
    char* mem_loc;
};

typedef struct Bins Bins;
struct Bins{
    int howFull;
    int startRange;
    char* bin1_offset;
    char* bin2_offset;
    int bin1_used;
    int bin2_used;
};

Bins g_Bins[MAX_BINS];

typedef struct memory_structure memory_structure;
struct memory_structure{
    int j;
    int offset;

};

void display(memory_structure mem);


void* my_alloc(size_t size);
void my_free(void * ptr);
int size_level(size_t);


void* my_alloc(size_t size){
    int i;
    //printf("my_alloc start\n");
    for(i=0;i<MAX_BINS;i++){
        if(!g_Bins[i].bin1_used){
           // printf("my_alloc bin1 test\n");
            g_Bins[i].bin1_used=1;
            //printf("pointer:  %p\n",g_Bins[i].bin1_offset);
            return (void *)g_Bins[i].bin1_offset;
        } else if(!g_Bins[i].bin2_used){
            g_Bins[i].bin2_used=1;
            return g_Bins[i].bin2_offset;
        }
    }
}
void my_free(void * ptr){
    int i;
    //printf("my_free start\n");
    for(i=0;i<MAX_BINS;i++){
        if(g_Bins[i].bin1_offset==ptr){
            g_Bins[i].bin1_used=0;
            return; 
        } else if(g_Bins[i].bin1_offset==ptr){
            g_Bins[i].bin2_used=0;
            return; 
        }
    }
}

void my_range(range){

}


int main(int argc, char *argv[])
{
    
    int low, high, j, range, k, bins, r;

    clock_t start_mal,end_mal,start_free,end_free,total_time=0;
    int cycle = 0;
    int processes_started = 0;
    int processes_finished=0;
    long mem_size=0;
    Process Processes[50]; // here should be 50
    int i=0;
//    printf("sizeof: %ld",SMALLEST_SIZE*(long)sizeof(Node));
    size_t total_mem = TOTAL_MEM;

    char * start_of_mem = NULL;
    
    start_of_mem = malloc(total_mem);
    printf("start %p\n",start_of_mem);
    {


        char* startRange =0;
        char* endRange=0;
        char* midRange=0;
   
        for(r = 0; r < 20; r++)  //104857600
        {



            startRange = endRange;
            midRange = startRange + BIN_SIZE/2;
            endRange = startRange+BIN_SIZE;

            
            g_Bins[r].bin1_offset=start_of_mem+(long)startRange;
            g_Bins[r].bin1_used=0;
            g_Bins[r].bin2_offset=start_of_mem+(long)midRange;
            g_Bins[r].bin2_used=0;


            printf("\n Range  %d is between: %p-%p, MidRange: %p", r, startRange, endRange,midRange);
            printf("\nbin Range  %d is Bin 1: %p, Bin2: %p", r, g_Bins[r].bin1_offset, g_Bins[r].bin2_offset);
         


         }

    }



    srand(30);

        printf("\n \n Processes are: \n");

    for(i=0;i<50;i++){
        Processes[i].run_time=(rand()%(2500-200)+200);
        //printf("Process_time: %d\n",Processes[i].run_time);
        Processes[i].mem_size=(size_t)(rand()%(LARGEST_SIZE-SMALLEST_SIZE)+SMALLEST_SIZE);    
        //printf("Process_size: %ld\n",Processes[i].mem_size);
        Processes[i].end_time=-1;
    }

    for (cycle = 0;processes_finished<50;cycle++)
    {  // here should be 50
        if(cycle%50==0&&processes_started<50)

            { // here should be 50 both places
            start_mal=clock();
            mem_size+=Processes[processes_started].mem_size;
            //printf("mem_size= %ld\n",mem_size);
            Processes[processes_started].mem_loc=my_alloc(Processes[processes_started].mem_size);

            Processes[processes_started].proc_num=processes_started;
            //printf("do we get here?%p\n",Processes[processes_started].mem_loc);
            *Processes[processes_started].mem_loc='a';
            end_mal=clock();
            total_time+=end_mal-start_mal;
            Processes[processes_started].end_time=cycle+Processes[processes_started].run_time;
            processes_started++;
            }
        //need to make more efficient
        for(i=0;i<50;i++){ // here should be 50
            if(Processes[i].end_time==cycle)
                {
                //printf("i= %d\n",i);
                //printf("end_time= %d\n",Processes[i].end_time);
                //printf("cycle %d\n",cycle);
                start_free=clock();
                my_free(Processes[i].mem_loc);
                end_free=clock();
                total_time+=end_free-start_free;
                processes_finished++;
                printf("processes_finished= %d\n",processes_finished);
                }
        }
    }

    printf("Time: %ld\n",total_time);

//  Allocat for memory manager
//char * mem_size;
/*
mem = malloc(sizeof (int)* mem_size);
if (!mem) {

   for (j = 0; j < 20; j++) {
       mem->range[i].my_free = 1;
       mem->range[i].mem_started = NULL;
       mem->range[i].size = 0;
       }

   mem->total_mem = 1024 * 1024 * 100;
   mem->total_ranges_allocated = 0;

// allocat the actual memory
   mem->mem2 = malloc(1024 * 1024 * 100);
if (mem->mem2 == NULL) {
   printf("Coud not allocate memory\n");
   return;
   }

offset = 0;
for (j = 0; j < 20; j++) {
   mem->range[i].my_free = 1;
   mem->range[i].process_started = mem -> mem2 + offset;
   mem->range[i].size = 1024 * 1024 * 5;
   offset = offset + (1024 * 1024 * 5);
   }

}
*/
return 0;
} 
