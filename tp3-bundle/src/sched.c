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
		scheduler.tareasA.tareas[i] = NULL;
		scheduler.tareasB.tareas[i] = NULL;
	}

	scheduler.tareasA.pos = -1;
	scheduler.tareasA.cant_tareas = 0;
	scheduler.tareasB.pos = -1;
	scheduler.tareasB.cant_tareas = 0;

	scheduler.tareas_System[0] = (uint*) 0x68; //inicial
	scheduler.tareas_system[1] = (uint*) 0x70; //idle

	scheduler.tareaActual = 5;
}

uint* sched_tick(){
	if ((scheduler.tareaActual == JUGADOR_A) && (scheduler.tareasB.cant_tareas > 0)) {
		scheduler.tareaActual = JUGADOR_B;
		return (proximaTarea(Jugador_B));
	} else if (scheduler.tareasA.cant_tareas > 0){
		scheduler.tareaActual = JUGADOR_A;
		return (proximaTarea(Jugador_A));
	}
	return NULL;
}

uint* proximaTarea(tarea_scheduler tarea) {
	do {
		tarea.pos++;
		if (tarea.pos == 8) { tarea.pos = 0; }
	} while (tarea.tareas[tarea.pos] == NULL);
	
	return tarea.tareas[tarea.pos];
}

void agregar_tarea(uint* selector, uint jugador){
	if (jugador == JUGADOR_A) {
		colocar_nueva_tarea(selector, scheduler.tareasA);
	} else {
		colocar_nueva_tarea(selector, scheduler.tareasB);
	}
}

void colocar_nueva_tarea(uint* selector, tarea_scheduler jugador){
	if (jugador.cantidad_tareas > 8) {
		do {
			tarea.pos++;
			if (tarea.pos == 8) { tarea.pos = 0; }
		} while(jugador.tareas[jugador.pos] != NULL);
		
		jugador.tareas[jugador.pos] = selector;
		jugador.cantidad_tareas++;
	}
}