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

void inicializar_dir_pirata(){

}

void mmu_mapear_pagina(uint virt, uint cr3, uint fisica, uint attrs){
	//parsea offsets dentro de directorio de paginas
	uint pageDirOffset   = virt >> 22;
	uint pageTableOffset = (virt << 10) >> 22;
	uint pageDirectory   = cr3 >> 12;

	//recorre directorios
	uint* pageTable   = (uint*) *( (uint*) pageDirectory) + pageDirOffset;
	uint* page	 	  = (uint*) *( (uint*) ((uint) pageTable + pageTableOffset));
	
	//arma pagina
	uint* pageSegment = (uint*) ((fisica << 12) + attrs); //TODO: attrrs tiene 16 bits y los attrs del segmento de pagina tienen 12.... la parte alta viene en 0's, como se deberia manejar??? 

	//asigna segmento de pagina
	page = pageSegment; //TODO: WHAT???? esto vale???? D:
}

void mmu_unmapear_pagina(){

}

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */
void mmu_inicializar_dir_kernel(){
	uint i;
	uint* pageDirectory = (uint*) 0x27000;
	uint* pageTable 	= (uint*) 0x28000;

	for(i = 1024; i>0; i--){// inicializo cada elemento del PD.
	  *(pageDirectory + (i*4) - 4) = 0x0;
	}
	
	*(pageDirectory) = *(pageTable + (i*4) - 4) + (uint) 0x3;
}
