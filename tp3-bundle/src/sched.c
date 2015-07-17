/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
definicion de funciones del scheduler
*/

#include "sched.h"
#include "i386.h"
#include "screen.h"

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
	if( scheduler.tarea_actual == JUGADOR_A && scheduler.jugador_A.cantidad_tareas > 0) {
		return scheduler.jugador_A.tareas[scheduler.jugador_A.pos].id;
	} else if (scheduler.tarea_actual == JUGADOR_B && scheduler.jugador_B.cantidad_tareas > 0) {
		return scheduler.jugador_B.tareas[scheduler.jugador_B.pos].id;
	} else {
		return NULL_ID;
	}
}

uint* sched_tick(){
	uint* selector = (uint*) 0x70;
	game_tick(sched_tarea_actual_id());
	if (scheduler.tarea_actual == JUGADOR_A) {
		if (scheduler.jugador_B.cantidad_tareas > 0) {
			scheduler.tarea_actual = JUGADOR_B;
			selector = proximaTarea(&scheduler.jugador_B);
		} else if (scheduler.jugador_A.cantidad_tareas > 0) {
			selector = proximaTarea(&scheduler.jugador_A);	
		}
	} else {
		if (scheduler.jugador_A.cantidad_tareas > 0) {
			scheduler.tarea_actual = JUGADOR_A;
			selector = proximaTarea(&scheduler.jugador_A);
		} else if (scheduler.jugador_B.cantidad_tareas > 0) {
			selector = proximaTarea(&scheduler.jugador_B);
		}
	}
	return selector;
}

uint* proximaTarea(tarea_scheduler *tarea) {
	do{
		tarea->pos++;
		if (tarea->pos == 8) { tarea->pos = 0; }
	} while (tarea->tareas[tarea->pos].id == NULL_ID);
	return tarea->tareas[tarea->pos].selector;
}

void sched_agregar_tarea(uint jugador, uint posicion_jugador, uint tipo, uint parametros){
	if(sched_hay_tareas_en_ejecucion(&scheduler.jugador_A) == FALSE
	&& sched_hay_tareas_en_ejecucion(&scheduler.jugador_B) == FALSE){
		scheduler.tarea_actual = jugador;
	}
	
	if (jugador == JUGADOR_A) {
		sched_colocar_nueva_tarea(parametros, &scheduler.jugador_A, posicion_jugador, jugador, tipo);
	} else {
		sched_colocar_nueva_tarea(parametros, &scheduler.jugador_B, posicion_jugador, jugador, tipo);
	}
}

void sched_colocar_nueva_tarea(uint parametros, tarea_scheduler* jugador, ushort posicion_jugador, uint numero_jugador, uint tipo){
	uint selector;
	selector = inicializar_tarea(numero_jugador, posicion_jugador, tipo, parametros);

	jugador->tareas[posicion_jugador].selector = (uint*) selector;
	jugador->tareas[posicion_jugador].id 	   = posicion_jugador + (numero_jugador * 8);
	jugador->cantidad_tareas++;
}

char sched_hay_tareas_en_ejecucion(tarea_scheduler* jugador){
	return jugador->cantidad_tareas > 0;
}

void scheduler_matar_actual_tarea_pirata(){
	if (scheduler.tarea_actual == JUGADOR_A) {
		ejecutar_tarea(&scheduler.jugador_A);
	} else {
		ejecutar_tarea(&scheduler.jugador_B);		
	}
}

void ejecutar_tarea(tarea_scheduler *jugador){
	uint* selector = jugador->tareas[jugador->pos].selector;

	//elimino la tarea del jugador
	jugador->cantidad_tareas--;
	jugador->tareas[jugador->pos].id = NULL_ID;

	//dejo la tarea libre en la gdt
	uint gdtpos = (uint) selector >> 3;
	gdt[gdtpos].p = 0;

}