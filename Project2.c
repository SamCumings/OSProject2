#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>
typedef struct Process Process;
struct Process{
    int proc_num;
    int run_time;
    int end_time;
    size_t mem_size;
    char* mem_loc;
};

int main(int argc, char *argv[]){
    clock_t start_mal,end_mal,start_free,end_free,total_time=0;
    int cycle = 0;
    int processes_started = 0;
    int processes_finished=0;
    long mem_size=0;
//10000 , 2e+6
    Process Processes[50];
    int i=0;

    srand(30);
    for(i=0;i<50;i++){
        Processes[i].run_time=(rand()%(2500-200)+200);
        printf("Process_time: %d\n",Processes[i].run_time);
        Processes[i].mem_size=(size_t)(rand()%(2000000-1000)+1000);    
        printf("Process_size: %ld\n",Processes[i].mem_size);
        Processes[i].end_time=-1;
    }
    for (cycle = 0;processes_finished<50;cycle++){
        if(cycle%50==0&&processes_started<50){ 
            mem_size+=Processes[processes_started].mem_size;
            start_mal=clock();
            Processes[processes_started].mem_loc=malloc(Processes[processes_started].mem_size);

            end_mal=clock();
            Processes[processes_started].proc_num=processes_started;
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
