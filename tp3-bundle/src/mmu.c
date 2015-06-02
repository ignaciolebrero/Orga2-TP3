/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"


/* Macros */
/* -------------------------------------------------------------------------- */
#define PDE_INDEX(virt, res) \
 	res = virt >> 22;   

#define PTE_INDEX(virt, res) \
	res = (virt << 10) >> 22;

/* Atributos paginas */
/* -------------------------------------------------------------------------- */


/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<i>_CODE_ADDR.
 */

void mmu_inicializar(){
	paginas_libres.cantidad 	 = 768; //1024-256
	paginas_libres.primera_libre = (uint*) 0x100000;
}

uint* mmu_gimme_gimme_page_wachin(){
	uint* result = NULL;
	if (paginas_libres.cantidad > 0) {
		result = paginas_libres.primera_libre;
		
		paginas_libres.primera_libre ++; //WARNING: si muere puede ser aca por el ++, va de a 4?
		paginas_libres.cantidad --;
	} 
	return result;
}

void inicializar_dir_pirata(uint cr3, char team){
	/*uint i;
	uint* pageDirectory = (uint*) 0x2000;
	uint* pageTable 	= (uint*) 0x28000;
	*/
	//*pageDirectory = //creamos los directorios? y se los asignamos a este cr3????	
}

void mmu_mapear_pagina(uint virt, uint cr3, uint fisica, uint attrs){
	//parsea offsets dentro de directorio de paginas
	uint pageDirOffset, pageTableOffset;
	PDE_INDEX(virt, pageDirOffset);
	PTE_INDEX(virt, pageTableOffset);
	uint pageDirectory   = cr3   >> 12;

	//recorre directorios
	uint* pageTable   = (uint*)  *( (uint*) pageDirectory) + pageDirOffset;
	uint** page	 	  = (uint**) *( (uint*) ((uint) pageTable + pageTableOffset));
	
	//arma pagina
	uint* pageSegment = (uint*) ((fisica << 12) + attrs); //TODO: attrrs tiene 16 bits y los attrs del segmento de pagina tienen 12.... la parte alta viene en 0's, como se deberia manejar??? 

	//asigna segmento de pagina
	*page = pageSegment; //TODO: WHAT????
	tlbflush();
}

void mmu_unmapear_pagina(uint virt, uint cr3){
	//parsea offsets dentro de directorio de paginas
	uint pageDirOffset, pageTableOffset;
	PDE_INDEX(virt, pageDirOffset);
	PTE_INDEX(virt, pageTableOffset);
	uint pageDirectory   = cr3   >> 12;

	//recorre directorios
	uint*  pageTable = (uint*)  *( (uint*) pageDirectory) + pageDirOffset;
	uint** page	 	 = (uint**) *( (uint*) ((uint) pageTable + pageTableOffset));
	
	*page = NULL;
	tlbflush();
}

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */
void mmu_inicializar_dir_kernel(){
	uint i;
	uint* pageDirectory = (uint*) 0x27000;
	uint* pageTable 	= (uint*) 0x28000;
	
	*pageDirectory = (uint) 0x28003;

	uint j = (uint) 0x0;
	for(i=0; i< 1024 ; i++){
		*(pageTable + i) =  j + (uint) 0x3;
		j = j + (uint) 0x1000;
	}
}
