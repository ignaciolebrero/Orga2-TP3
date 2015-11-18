/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "game.h"
#include "tss.h"

#endif	/* !__SCHED_H__ */

#define MAX_ID  15
#define NULL_ID 17

typedef struct pirata{
	uint* selector;
	uint  id;
}pirata;

typedef struct tarea_scheduler{
	pirata  tareas[8];
	ushort  cantidad_tareas;
	int 	pos;
}tarea_scheduler;

typedef struct sched_tareas{
	uint* tareas_systema[2];
	uint  tarea_actual;
	tarea_scheduler jugador_A;
	tarea_scheduler jugador_B;
} sched_tareas;

uint* sched_tick();
uint* proximaTarea(tarea_scheduler*);

tarea_scheduler* scheduler_obtener_jugador(uint);
ushort obtener_posicion_libre(tarea_scheduler*);

uint sched_tarea_actual_id();
char sched_hay_tareas_en_ejecucion(tarea_scheduler*);
void inicializar_scheduler();
void sched_agregar_tarea(uint, uint, uint, uint);
void sched_colocar_nueva_tarea(uint, tarea_scheduler*, ushort, uint, uint);
void scheduler_matar_actual_tarea_pirata();
void ejecutar_tarea(tarea_scheduler*);
void screen_matar_pirata(pirata_t*);