/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "defines.h"
#include "game.h"

#define CODIGO_BASE       0X400000

#define MAPA_BASE_FISICA  0x500000
#define MAPA_BASE_VIRTUAL 0x800000

void mmu_inicializar();


#endif	/* !__MMU_H__ */
