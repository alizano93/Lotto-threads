/* mi lebreria de hilos basica

*/


#include <stdio.h>
#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

typedef long thread_t;
long mode; // 1 = expropiativo 2= no expropiativo
sigset_t sigThreadMask; // para el manejo de las sennales
int ignorarsennal;
struct sigaction schedulerHandle; //con este es que voy a inicializar el scheduler (ver el thread_init)
ucontext_t mainContext; //funciona como notificador
struc itimerval quantum; //estrutura para el timer
long timeIntervalInMSec;
sigset_t sigsetMask;

void thread_init(long nquantum) {
    if(readylist == null){
        sigemptyset(&sigThreadMask);
        thread_queue_init( &readylist );
        sigaddset(&sigThreadMask, SIGPROF); //para mandar sennal cuando expire el timer
        ignoreSignal = 0;

        thread_queue_init(&readylist); //inicializo la cola de threads

        if(readylist != null) {

            timeIntervalInMSec = nquantum;

            thread main = getNewThread();
            getcontext(&(main->context));

            //Meto a la lista

            memset(&schedylerHandle, 0, sizeof(schedulerHandle));
            schedulerHandle.sa_handler //http://linux.die.net/man/2/sigaction
            sigaction(SIFPROF, &schedulerHandle, NULL);
            //inicializo el quantum
            quantum.it_value.tv_sec = 0;// http://linux.die.net/man/2/setitimer
            quantum.it_value.tv_usec = timeIntervalInMSec;
            quantum.it_interval.tv_sec=0;
            quantum.it_interval.tv_usec = timeIntervalInMSec;
            setitimer(ITIMER_PROF, *quantum, NULL);
        }

    }
}

int thread_create(thread_t *id, void *(*rutina)(void *), void *arg) {
    //creo el contexto, lo meto a la cola de los threads devuelvo 1 si funciono todo
}

void thread_yield(){
    raise(SIGPROF); //con esto deberia bastar mandando la sennal se captura y cambio segun el scheduler
}

void thread_exit(){
    //deberia terminar el contexto y podriamos y deberiamos tener una estructura para meter a los threads terminados, asi que tambien lo meteria a la lista de terminados
}

thread_t self(){
    //devuelvo el id del thread actual // pensaria tener una lista y en la estructura de la lista tener el actual. o tener el thread actual por aparte. estoy analizando
}

int thread_join(){
//espera a que todos hayan terminado
}



