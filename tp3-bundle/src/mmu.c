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
		paginas_libres.primera_libre += (uint) 0x1000; //0x1000 es unapagina en memoria, sera asi??
		paginas_libres.cantidad --;
	} 
	return result;
}

void inicializar_dir_pirata(uint* cr3, uint fisicmem, uint elteam){
	uint* pageDirectory = mmu_gimme_gimme_page_wachin();

	//inicializa pagedirectory sin entradas
	uint i;
	for(i=0; i<1024 ; i++){
		*(pageDirectory + i) = (uint) 0x02;
	}
	*cr3 = (uint) pageDirectory; //esto es asi directo?????????
	breakpoint();
	mmu_mapear_pagina((uint) 0x400000, *cr3, fisicmem, (uint) 0x3); //mapea la direccion de codigo a 0x4000 //los atributos son 0x03?
	if (elteam == JUGADOR_A) {
		mmu_mapear_pagina( (uint) 0x800000, *cr3, (uint) 0x500000, (uint) 0x03);

		mmu_mapear_pagina( (uint) 0x800000 + (uint) 0x1000 * 01, *cr3, (uint) 0x500000 + (uint) 0x1000 * 01, (uint) 0x03);
		mmu_mapear_pagina( (uint) 0x800000 + (uint) 0x1000 * 80, *cr3, (uint) 0x500000 + (uint) 0x1000 * 80, (uint) 0x03);
		mmu_mapear_pagina( (uint) 0x800000 + (uint) 0x1000 * 81, *cr3, (uint) 0x500000 + (uint) 0x1000 * 81, (uint) 0x03);
	} else {
		mmu_mapear_pagina( ((uint) 0x1520000-1), *cr3, ((uint) 0x121FFFF-1), (uint) 0x03);
		
		mmu_mapear_pagina( ((uint) 0x1520000-1) - (uint) 0x1000 * 01, *cr3, ((uint) 0x121FFFF-1) - (uint) 0x1000 * 01, (uint) 0x03);
		mmu_mapear_pagina( ((uint) 0x1520000-1) - (uint) 0x1000 * 80, *cr3, ((uint) 0x121FFFF-1) - (uint) 0x1000 * 80, (uint) 0x03);
		mmu_mapear_pagina( ((uint) 0x1520000-1) - (uint) 0x1000 * 81, *cr3, ((uint) 0x121FFFF-1) - (uint) 0x1000 * 81, (uint) 0x03);
	}	
}

void mmu_mapear_pagina(uint virt, uint cr3, uint fisica, uint attrs){
	uint pageDirectory = cr3 & 0XFFFFF000;
	uint pageDirOffset, pageTableOffset;
	PDE_INDEX(virt, pageDirOffset);
	PTE_INDEX(virt, pageTableOffset);

	//recorre directorios
	uint*  pageTable = (uint*) *( (uint*) (pageDirectory + pageDirOffset)); // limpio atributos

	//revisa si existe la pagina
	uint presentBit = *pageTable & 1;	
	if (presentBit == 0) { //preguntar por el bit de presente
		breakpoint();
		pageTable  = mmu_gimme_gimme_page_wachin();
		init_table(pageTable);
		* (uint*) *( (uint*) (pageDirectory + pageDirOffset)) = (uint) pageTable + 0x3; //preguntar por esto......es muy turbio
	}
	
	uint** page	= (uint**) *( (uint*) ((uint) pageTable + pageTableOffset));

	//arma pagina
	uint* pageSegment = (uint*) (fisica + attrs); 

	//asigna segmento de pagina
	*page = pageSegment;
	tlbflush();
}

void mmu_unmapear_pagina(uint virt, uint cr3){
	//parsea offsets dentro de directorio de paginas
	uint pageDirOffset, pageTableOffset;
	uint pageDirectory = cr3 & 0XFFFFF000;
	PDE_INDEX(virt, pageDirOffset);
	PTE_INDEX(virt, pageTableOffset);

	//recorre directorios
	uint*  pageTable = (uint*)  *( (uint*) pageDirectory + pageDirOffset);
	uint** page	 	 = (uint**) *( (uint*) ((uint) pageTable + pageTableOffset));
	
	//mata pagina
	*page = NULL;
	tlbflush();
}

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */
void mmu_inicializar_dir_kernel(){
	uint i,k;
	uint* pageDirectory = (uint*) KERNEL_PAGE_DIRECTORY;
	uint* pageTable 	= (uint*) KERNEL_PAGE_TABLE;

	init_table(pageDirectory);

	for(i=0; i<3; i++){
		*(pageDirectory + i) = ((uint) 0x28000 + (i * (uint)0x1000) ) + 0x3;
		uint j = (uint) 0x0;
		for(k=0; k < 1024 ; k++){
			*(pageTable + k) =  j + (uint) 0x3;
			j = j + (uint) 0x1000;
		}
		pageTable += (uint) 0x1000;
	}
}

void init_table(uint* table){
	uint i;
	for(i=0; i<1024; i++){
		*(table + i) = (uint) 0x2;
	}
} 