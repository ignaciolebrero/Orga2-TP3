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

	scheduler.tareasA.pos = -1;
	scheduler.tareasB.pos = -1;
	scheduler.tareasA.cantidad_tareas = 0;
	scheduler.tareasB.cantidad_tareas = 0;

	scheduler.tareas_systema[0] = (uint*) 0x68; //inicial
	scheduler.tareas_systema[1] = (uint*) 0x70; //idle

	scheduler.tareaActual = 5;
}

uint* sched_tick(){
	if ((scheduler.tareaActual == JUGADOR_A) && (scheduler.tareasB.cantidad_tareas > 0)) {
		scheduler.tareaActual = JUGADOR_B;
		return (proximaTarea(scheduler.tareasB));
	} else if (scheduler.tareasA.cantidad_tareas > 0){
		scheduler.tareaActual = JUGADOR_A;
		return (proximaTarea(scheduler.tareasA));
	}
	return (uint*) (0x70);
}

uint* proximaTarea(tarea_scheduler tarea) {
	do {
		tarea.pos++;
		if (tarea.pos == 8) { tarea.pos = 0; }
	} while (tarea.tareas[tarea.pos] == 0);
	
	return tarea.tareas[tarea.pos];
}

void agregar_tarea(uint jugador){
	if(hay_tareas_disponibles(&scheduler.tareasA) == FALSE
	&& hay_tareas_disponibles(&scheduler.tareasB) == FALSE){
		scheduler.tareaActual = jugador;
	}

	tarea_scheduler* jugador_actual = scheduler_obtener_jugador(jugador);
	if ( hay_tareas_disponibles(jugador_actual) == TRUE ){
		ushort posicion_jugador;
		uint   selector;

		posicion_jugador = obtener_posicion_libre(jugador_actual);
		selector 		 = inicializar_tarea(jugador, posicion_jugador);
		colocar_nueva_tarea(selector, jugador_actual, posicion_jugador);
	}
}

void colocar_nueva_tarea(uint selector, tarea_scheduler* jugador, ushort posicion_jugador){
	(*jugador).tareas[posicion_jugador] = (uint*) selector;
	(*jugador).cantidad_tareas++;
}

ushort obtener_posicion_libre(tarea_scheduler* jugador_actual){
    ushort i = 0;
    while( (*jugador_actual).tareas[i] != 0 ) { i++; } //TODO: revisar! esta chequeando por si iomap esta en cero, cuando desseteamos la tarea esto deberia ir en 0, hay otra manera de hacerlo??
    return i;
}

tarea_scheduler* scheduler_obtener_jugador(uint jugador){
	tarea_scheduler* jugador_actual;
	if (jugador == JUGADOR_A) {
		jugador_actual = (tarea_scheduler*) &scheduler.tareasA; 
	} else {
		jugador_actual = (tarea_scheduler*) &scheduler.tareasB;
	}
	return jugador_actual;
}

char hay_tareas_disponibles(tarea_scheduler* jugador_actual){
	char result = (*jugador_actual).cantidad_tareas < 8;
	return result;
}