/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"
/* Atributos paginas */
/* -------------------------------------------------------------------------- */


/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<i>_CODE_ADDR.
 */

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */

void mmu_inicializar(){
	uint i;
	uint* pageDirectory = (uint*) 0x27000;
	uint* pageTable 	= (uint*) 0x28000;

	for(i = 1024; i>0; i--){
	  *(pageDirectory + (i*4) - 4) = 0x00000002;
	}

	*(pageDirectory) = (uint) pageTable + 0x3; //TODO: revisar, se puede remper aca

	uint page = (uint) 0x1400;
	for(i = 1024; i>0; i--){
	  *(pageTable + (i*4) - 4) = page;
	  *(pageTable + (i*4) - 4) = *(pageTable + (i*4) - 4) + (uint) 0x00000003;
	  page = page - (uint) 0x1000;
	}	

	*(pageTable + 512*4) = 0x27003;
}