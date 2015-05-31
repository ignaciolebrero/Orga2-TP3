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

void mmu_inicializar(){
	
}

void mmu_inicializar_dir_pirata(){

}

void mmu_mapear_pagina(){

}

void mmu_unmapear_pagina(){

}

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */
void mmu_inicializar_dir_kernel(){
	uint  i;
	uint* pageDirectory = (uint*) 0x27000;
	uint* pageTable 	= (uint*) 0x28000;

	uint* actualTable;
	uint* actualPage;
	uint* actualDir;

	//inicializa pageDirectory
	for(i = 1024; i>0; i--){
	  *(pageDirectory + (i*4) - 4) = 0x00000002;
	}

	//setea primero 4MB
	*(pageDirectory) = (uint) pageTable + 0x3; //TODO: revisar, se puede remper aca

	//inicializa pageTable
	uint page = (uint) 0x1400;
	for(i = 1024; i>0; i--){
	  *(pageTable + (i*4) - 4) = page;
	  *(pageTable + (i*4) - 4) = *(pageTable + (i*4) - 4) + (uint) 0x00000003;
	  page = page - (uint) 0x1000;
	}	

	uint j;
	//identity mapping??
	for (j=0; j < 0x003FFFFF; j++){
		actualTable = ( *(pageDirectory + (uint*) (j>>22))) >> 12;
		actualPage  = ( *(actualTable   + (uint*) ((j<<10)>>22))) >> 12;
		actualDir   = ( *(actualPage 	+ (uint*) ((j<<20) >>20)) );		
	    *actualDir  = j;
	}		

}