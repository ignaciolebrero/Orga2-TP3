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

typedef struct tarea_schededuler{
	uint *tareas[8];
	uint pos;
	ushort cantidad_tareas;
}tareaSched;

typedef struct sched_tareas{
	uint* tareas_systema[2];
	tarea_scheduler tareasA;
	tarea_Scheduler tareasB;
	uint tareaActual;
} sched_tareas;