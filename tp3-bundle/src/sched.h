/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "game.h"

#endif	/* !__SCHED_H__ */

typedef struct tarea_scheduler{
	uint *tareas[8];
	uint pos;
	ushort cantidad_tareas;
}tarea_scheduler;

typedef struct sched_tareas{
	uint* tareas_systema[2];
	tarea_scheduler tareasA;
	tarea_scheduler tareasB;
	uint tareaActual;
} sched_tareas;

void inicializar_scheduler();
uint* sched_tick();
uint* proximaTarea(tarea_scheduler);
void agregar_tarea(uint*, uint);
void colocar_nueva_tarea(uint*, tarea_scheduler);