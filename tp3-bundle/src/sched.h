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

typedef struct tareaSched{
	uint *tareas[8];
	uint pos;
}tareaSched;

typedef struct sched_tareas{
	uint* tareasSystem[2];
	tareaSched tareasA;
	tareaSched tareasB;
	uint tareaActual;
} sched_tareas;