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
		scheduler.jugador_A.tareas[i].selector = NULL;
		scheduler.jugador_B.tareas[i].selector = NULL;
		scheduler.jugador_A.tareas[i].id = NULL_ID;
		scheduler.jugador_B.tareas[i].id = NULL_ID;

	}

	scheduler.jugador_A.pos = -1;
	scheduler.jugador_B.pos = -1;
	scheduler.jugador_A.cantidad_tareas = 0;
	scheduler.jugador_B.cantidad_tareas = 0;

	scheduler.tareas_systema[0] = (uint*) 0x68; //inicial
	scheduler.tareas_systema[1] = (uint*) 0x70; //idle

	scheduler.tarea_actual = 5;
	scheduler.id_actual    = 0;
}

uint* sched_tick(){
	if ((scheduler.tarea_actual == JUGADOR_A) && (scheduler.jugador_B.cantidad_tareas > 0)) {
		scheduler.tarea_actual = JUGADOR_B;
		return proximaTarea(scheduler.jugador_B);
	} else if (scheduler.jugador_A.cantidad_tareas > 0){
		scheduler.tarea_actual = JUGADOR_A;
		return proximaTarea(scheduler.jugador_A);
	}
	return (uint*) 0x70;
}

uint* proximaTarea(tarea_scheduler tarea) {
	do {
		tarea.pos++;
		if (tarea.pos == 8) { tarea.pos = 0; }
	} while (tarea.tareas[tarea.pos].id == NULL_ID);
	
	return tarea.tareas[tarea.pos].selector;
}

void sched_agregar_tarea(uint jugador, uint posicion_jugador){
	if(sched_hay_tareas_disponibles(&scheduler.jugador_A) == FALSE
	&& sched_hay_tareas_disponibles(&scheduler.jugador_B) == FALSE){
		scheduler.tarea_actual = jugador;
	}

	tarea_scheduler* jugador_actual = scheduler_obtener_jugador(jugador);
	if ( sched_hay_tareas_disponibles(jugador_actual) == TRUE ){
		uint   selector;

		selector = inicializar_tarea(jugador, posicion_jugador);
		sched_colocar_nueva_tarea(selector, jugador_actual, posicion_jugador);
	}
}

void sched_colocar_nueva_tarea(uint selector, tarea_scheduler* jugador, ushort posicion_jugador){
	(*jugador).tareas[posicion_jugador].selector = (uint*) selector;
	(*jugador).tareas[posicion_jugador].id 		 = scheduler.id_actual;
	(*jugador).cantidad_tareas++;

	scheduler.id_actual = (scheduler.id_actual + 1) % MAX_ID; //por ahi hay que matar el id
}

tarea_scheduler* scheduler_obtener_jugador(uint jugador){
	if (jugador == JUGADOR_A) {
		return (tarea_scheduler*) &scheduler.jugador_A; 
	} else {
		return (tarea_scheduler*) &scheduler.jugador_B;
	}
}


char sched_hay_tareas_disponibles(tarea_scheduler* jugador){
	return jugador->cantidad_tareas > 0;
}