#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <array>
#include <pthread.h> 
#include <sched.h>

using namespace std;


void carga(){
    for(int i=0; i<250000000; i++);
}
void *th2(void *arg);
void *th3(void *arg);

int main(){
	int res;
    pthread_t thread2;

    pthread_attr_t thread_attr;
    sched_param scheduling_value;
    int  max_priority, min_priority;
    
    res = pthread_attr_init(&thread_attr);
    if (res != 0) {
        perror("Falha na criação de Atributo");
        exit(EXIT_FAILURE);
    }

    res = pthread_attr_setschedpolicy(&thread_attr, SCHED_OTHER);
    if (res != 0) {
        perror("Falha na configuração de política de escalonamento");
        exit(EXIT_FAILURE);
    }

    res = pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
    if (res != 0) {
        perror("Falha na configuração de atributo detached");
        exit(EXIT_FAILURE);
    }


    res = pthread_create(&thread2, &thread_attr, th2, (void *) 2);
    if (res != 0) {
        perror("Falha na criação da thread");
        exit(EXIT_FAILURE);
    }
    sleep(5);
    max_priority = sched_get_priority_max(SCHED_OTHER);
    min_priority = sched_get_priority_min(SCHED_OTHER);
    scheduling_value.sched_priority = max_priority;
    cout << "VOU SETAR PRIORIDADE MAXIMA! ------ " << max_priority << endl;
    cout << "A MINIMA SERIA: --------- " << min_priority << endl;
    res = pthread_attr_setschedparam(&thread_attr, &scheduling_value);
    if (res != 0) {
        perror("Falha na configuração da pol�tica de escalonamento");
        exit(EXIT_FAILURE);
    }
    cout << "MUDOU!\n";
    sleep(20);

}

void *th2(void *arg){
	int ret;
	int policy = 0;

	pthread_t this_thread = pthread_self();
	struct sched_param params;
     ret = pthread_getschedparam(this_thread, &policy, &params);
     if (ret != 0) {
         std::cout << "Couldn't retrieve real-time scheduling paramers" << std::endl;
     }
 
     // Check the correct policy was applied
     if(policy != SCHED_FIFO) {
         std::cout << "Scheduling is NOT SCHED_FIFO!" << std::endl;
     } else {
         std::cout << "SCHED_FIFO OK" << std::endl;
     }
 
     // Print thread scheduling priority
     std::cout << "Thread priority is " << params.sched_priority << std::endl;
     int a=0;
	while(true){
		carga();
		ret = pthread_getschedparam(this_thread, &policy, &params);
	    if (ret != 0) {
	        std::cout << "Couldn't retrieve real-time scheduling paramers" << std::endl;
	    }
	    std::cout << "Thread priority is " << params.sched_priority << std::endl;
		cout << "acabou!\n";
		sched_setscheduler(0, 0, &params);
		if(a==15){
			//params.sched_priority = sched_get_priority_max(SCHED_OTHER);	
			cout << "ASEASDPOAKSOPDKASOP\n";
		}
		a++;
	}
}