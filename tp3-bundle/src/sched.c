/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
definicion de funciones del scheduler
*/

#include "sched.h"
#include "i386.h"

sched_tareas scheduler;

void inicializar_scheduler(){
	int i;
	for(i=0; i<8; i++){
		scheduler.tareasA.tareas[i] = 0;
		scheduler.tareasB.tareas[i] = 0;
	}

	scheduler.tareasSystem[0] = (uint*) 0x68; //inicial
	scheduler.tareasSystem[1] = (uint*) 0x70; //idle

	scheduler.tareaActual = SYSTEMA;
}

uint sched_tick(){
	switch(scheduler.tareaActual){
		case(JUGADOR_A):
		break;
		case(JUGADOR_B):
		break;
		case(SYSTEMA):
		break;
	}

	return 0;
}

void proximaTarea() {

}

void agregar_tarea(uint* selector, uint jugador, uint numero){
	if (jugador == JUGADOR_A) {
		scheduler.tareasA.tareas[numero] = selector;
	} else {
		scheduler.tareasB.tareas[numero] = selector;
	}
}