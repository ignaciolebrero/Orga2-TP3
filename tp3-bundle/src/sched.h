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

typedef struct tarea_scheduler{
	uint* tareas[8];
	uint pos;
	ushort cantidad_tareas;
}tarea_scheduler;

typedef struct sched_tareas{
	uint* tareas_systema[2];
	uint  tareaActual;
	tarea_scheduler tareasA;
	tarea_scheduler tareasB;
} sched_tareas;

uint* sched_tick();
uint* proximaTarea(tarea_scheduler);

tarea_scheduler* scheduler_obtener_jugador(uint);
ushort obtener_posicion_libre(tarea_scheduler*);

char hay_tareas_disponibles(tarea_scheduler*);
void inicializar_scheduler();
void agregar_tarea(uint);
void colocar_nueva_tarea(uint, tarea_scheduler*, ushort);
