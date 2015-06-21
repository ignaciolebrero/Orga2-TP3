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

	scheduler.tarea_actual = 4;
}

uint sched_tarea_actual_id(){
	if (scheduler.tarea_actual == JUGADOR_A) {
		return scheduler.jugador_A.tareas[scheduler.jugador_A.pos].id;
	} else {
		return scheduler.jugador_B.tareas[scheduler.jugador_B.pos].id;
	}
	return NULL_ID;
}

uint* sched_tick(){
	uint* selector;
	
	if ((scheduler.tarea_actual == JUGADOR_A) && (scheduler.jugador_B.cantidad_tareas > 0)) {
		scheduler.tarea_actual = JUGADOR_B;
		selector = proximaTarea(scheduler.jugador_B);
	} else if (scheduler.jugador_A.cantidad_tareas > 0){
		scheduler.tarea_actual = JUGADOR_A;
		selector = proximaTarea(scheduler.jugador_A);
	} else {
		selector = (uint*) 0x70;
	}

	game_tick(sched_tarea_actual_id());
	return selector;
}

uint* proximaTarea(tarea_scheduler tarea) {
	do {
		tarea.pos++;
		if (tarea.pos == 8) { tarea.pos = 0; }
	} while (tarea.tareas[tarea.pos].id == NULL_ID);
	
	return tarea.tareas[tarea.pos].selector;
}

void sched_agregar_tarea(uint jugador, uint posicion_jugador, uint tipo){
	if(sched_hay_tareas_en_ejecucion(&scheduler.jugador_A) == FALSE
	&& sched_hay_tareas_en_ejecucion(&scheduler.jugador_B) == FALSE){
		scheduler.tarea_actual = jugador;
	}
	uint   selector;
	tarea_scheduler* jugador_actual = scheduler_obtener_jugador(jugador);

	selector = inicializar_tarea(jugador, posicion_jugador, tipo);
	sched_colocar_nueva_tarea(selector, jugador_actual, posicion_jugador, jugador);
}

void sched_colocar_nueva_tarea(uint selector, tarea_scheduler* jugador, ushort posicion_jugador, uint numero_jugador){
	jugador->tareas[posicion_jugador].selector = (uint*) selector;
	jugador->tareas[posicion_jugador].id 	   = posicion_jugador * (numero_jugador + 1) ;
	jugador->cantidad_tareas++;
}

tarea_scheduler* scheduler_obtener_jugador(uint jugador){
	if (jugador == JUGADOR_A) {
		return (tarea_scheduler*) &scheduler.jugador_A; 
	} else {
		return (tarea_scheduler*) &scheduler.jugador_B;
	}
}

char sched_hay_tareas_en_ejecucion(tarea_scheduler* jugador){
	return jugador->cantidad_tareas > 0;
}

void scheduler_matar_actual_tarea_pirata(){
	tarea_scheduler* jugador = scheduler_obtener_jugador(scheduler.tarea_actual);
	uint* selector = jugador->tareas[jugador->pos].selector;

	//elimino la tarea del jugador
	jugador->cantidad_tareas--;
	jugador->tareas[jugador->pos].id = NULL_ID;

	//dejo la tarea la tarea libre en la gdt
	uint gdtpos = (uint) selector >> 3;
	gdt[gdtpos].p = 0;
	//TODO: como voy ahora a la idle?????
}