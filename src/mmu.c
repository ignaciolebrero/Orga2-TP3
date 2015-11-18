/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"
#include "screen.h"

/* Macros */
/* -------------------------------------------------------------------------- */
#define PDE_INDEX(virt, res) \
 	res = virt >> 22;   

#define PTE_INDEX(virt, res) \
	res = (virt << 10) >> 22;

/* Atributos paginas */
/* -------------------------------------------------------------------------- */

#define PG_PRESENT		1
#define PG_READ_WRITE   1
#define PG_USER			0

/* Direcciones fisicas de codigos */
/* -------------------------------------------------------------------------- */
/* En estas direcciones estan los códigos de todas las tareas. De aqui se
 * copiaran al destino indicado por TASK_<i>_CODE_ADDR.
 */

void mmu_inicializar(){
	paginas_libres.cantidad 	 = 768; //1024-256
	paginas_libres.primera_libre = (uint*) 0x100000;
}

void* mmu_gimme_gimme_page_wachin(){
	void* nueva_pagina = NULL;
	if (paginas_libres.cantidad > 0) {
		nueva_pagina = paginas_libres.primera_libre;
		paginas_libres.primera_libre += (uint) 0x1000;
		paginas_libres.cantidad --;
	} 
	return nueva_pagina;
}

char hay_paginas(){
	return paginas_libres.cantidad == 0;	
}

uint obtener_posicion_inicial_virtual(uint elteam){
	if (elteam == JUGADOR_A) {
		return (uint) (0x800000 + 0x50000*2 + 0x1000);
	} else {
		return (uint) (0x800000 + 0x1000 * ((MAPA_ALTO-2) * MAPA_ANCHO + (MAPA_ANCHO-1)));
	}
}

uint obtener_posicion_fisica_codigo_pirata(uint elteam, uint tipo_pirata){
	if(tipo_pirata == 0){
		return 0x10000 + ( 0x2000 * elteam);
	} else {
		return 0x11000 + ( 0x2000 * elteam);
	}
}

uint inicializar_dir_pirata(uint fisicmem, uint elteam, uint tipo_pirata){ 
	page_directory_entry* pageDirectory = (page_directory_entry*) mmu_gimme_gimme_page_wachin();
	//inicializa pagedirectory sin entradas
	init_directory_table(pageDirectory);

	uint cr3 = (uint) pageDirectory;
	uint posicion_virtual = obtener_posicion_inicial_virtual(elteam);

	mmu_mapear_pagina( posicion_virtual - 0x1000 * 82, cr3, (uint) fisicmem - 0x1000 * 82, (uint) 0x07);
	mmu_mapear_pagina( posicion_virtual - 0x1000 * 81, cr3, (uint) fisicmem - 0x1000 * 81, (uint) 0x07);
	mmu_mapear_pagina( posicion_virtual - 0x1000 * 80, cr3, (uint) fisicmem - 0x1000 * 80, (uint) 0x07);

	mmu_mapear_pagina( posicion_virtual - 0x1000 * 01, cr3, (uint) fisicmem - 0x1000 * 01, (uint) 0x07);
	mmu_mapear_pagina( posicion_virtual + 0x1000 * 00, cr3, (uint) fisicmem - 0x1000 * 00, (uint) 0x07);
	mmu_mapear_pagina( posicion_virtual + 0x1000 * 01, cr3, (uint) fisicmem + 0x1000 * 01, (uint) 0x07);

	mmu_mapear_pagina( posicion_virtual + 0x1000 * 80, cr3, (uint) fisicmem + 0x1000 * 80, (uint) 0x07);
	mmu_mapear_pagina( posicion_virtual + 0x1000 * 81, cr3, (uint) fisicmem + 0x1000 * 81, (uint) 0x07);
	mmu_mapear_pagina( posicion_virtual + 0x1000 * 82, cr3, (uint) fisicmem + 0x1000 * 82, (uint) 0x07);

	uint pos_codigo_pirata = obtener_posicion_fisica_codigo_pirata(elteam, tipo_pirata);
	mmu_mover_codigo_pirata(cr3, (uint*) fisicmem, (uint*) pos_codigo_pirata);	
	
	uint i;
	for (i = 0; i < 1024 * 1024 * 4; i+= 0x1000){
		mmu_mapear_pagina(i, cr3, i, 0x3);
	}

	return cr3;
}

void mmu_mover_codigo_pirata(uint cr3, uint *destino, uint *source){
	uint cr3_tarea_actual = rcr3();

	mmu_mapear_pagina( (uint) 0x403000, cr3_tarea_actual, (uint) source , (uint) 0x7);
	mmu_mapear_pagina( (uint) 0x404000, cr3_tarea_actual, (uint) destino, (uint) 0x7);

	//copia el codigo 
	uint i;
	for(i = 0x0; i < 0x400; i++){
		* (uint*) (0x404000 + i*4) = * (uint*) (0x403000 + i*4);
	}

	mmu_mapear_pagina (0x400000, cr3 , (uint) destino, 0x7);

	mmu_unmapear_pagina( 0x403000, cr3_tarea_actual );
	mmu_unmapear_pagina( 0x404000, cr3_tarea_actual );
}


void mmu_mapear_pagina(uint virt, uint cr3, uint fisica, uint attrs){
	uint pageDirectory = cr3 & 0XFFFFF000;
	uint pageDirOffset, pageTableOffset;
	PDE_INDEX(virt, pageDirOffset);
	PTE_INDEX(virt, pageTableOffset);

	//recorre directorios
	page_directory_entry *directoryEntry = ((page_directory_entry*) pageDirectory) + pageDirOffset;
	page_table_entry *tableEntry;

	//revisa si existe la pagina
	if (directoryEntry->P == 0) { //preguntar por el bit de presente
		tableEntry = (page_table_entry*) mmu_gimme_gimme_page_wachin();
		init_page_table(tableEntry);
		set_directory_entry(directoryEntry, tableEntry, attrs);
	} else {
		uint dir = (uint) directoryEntry->dir_pagina_tabla << 12;
		tableEntry =  (page_table_entry*) dir;
	}

	set_table_entry(tableEntry + pageTableOffset, fisica, attrs);

	tlbflush();
}

void mmu_unmapear_pagina(uint virt, uint cr3){
	//parsea offsets dentro de directorio de paginas
	uint pageDirOffset, pageTableOffset;
	uint pageDirectory = cr3 & 0XFFFFF000;
	PDE_INDEX(virt, pageDirOffset);
	PTE_INDEX(virt, pageTableOffset);

	//recorre directorios
	page_directory_entry* directoryEntry = (page_directory_entry*) pageDirectory  + pageDirOffset;

	if (directoryEntry->P != 0) {
		uint dir = directoryEntry->dir_pagina_tabla << 12;
		page_table_entry* tableEntry = (page_table_entry*) dir + pageTableOffset ; //Preguntar porque esto funciona
		tableEntry->P = 0;
	}

	tlbflush();
}

void mmu_mapear_posicion_mapa(uint cr3, uint posicion) {
	uint fisica  = 0x500000 + (posicion * 0x1000);
	uint virtual = 0x800000 + (posicion * 0x1000);
	uint attrs   = (uint) 0x7;

	mmu_mapear_pagina(virtual, cr3, fisica, attrs);
}

/* Direcciones fisicas de directorios y tablas de paginas del KERNEL */
/* -------------------------------------------------------------------------- */

//La funcion fue escrita previo al agregador de las estructuras page_----_entry, no se cambio en la reimplementacion
void mmu_inicializar_dir_kernel(){
	page_table_entry     *pageTable = (page_table_entry*)     KERNEL_PAGE_TABLE;
	page_directory_entry *p 	    = (page_directory_entry*) KERNEL_PAGE_DIRECTORY;
	uint i;
	init_directory_table(p);

	for(i=0; i <4; i++){
		set_directory_entry(p + i, pageTable + 0x400*i, 0x3);
	}

	for (i = 0; i < 1024 * 1024 * 4; i+= 0x1000){
		mmu_mapear_pagina(i, 0x27000, i, 0x3);
	}
}

void init_page_table(page_table_entry* table){
	uint i;
	for(i=0; i<1024; i++){
		(table + i)->P  = 0;
		(table + i)->RW = PG_READ_WRITE;
		(table + i)->US = PG_USER;
	}
}

void init_directory_table(page_directory_entry* table){
	uint i;
	for(i=0; i<1024; i++){
		(table + i)->P  = 0;
		(table + i)->RW = PG_READ_WRITE;
		(table + i)->US = PG_USER;
	}
} 

void set_directory_entry(page_directory_entry* dir, page_table_entry* table, uint attrs){
	dir->dir_pagina_tabla = (uint)table >> 12;
	dir->disp  = 0;
	dir->G 	   = 0;
	dir->PS    = 0;
	dir->disp0 = 0;
	dir->A 	   = 0;
	dir->PCD   = 0;
	dir->PWT   = 0;
	dir->US    = (attrs & 0x4) >> 2;
	dir->RW    = (attrs & 0x2) >> 1;
	dir->P     =  attrs & 0x1;
}

void set_table_entry(page_table_entry* table, uint fisic, uint attrs){
	table->dir_pagina_mem = fisic >> 12;
	table->disp  = 0; 
	table->G 	 = 0;
	table->PAT   = 0;
	table->D     = 0;
	table->A 	 = 0;
	table->PCD   = 0;
	table->PWT   = 0;
	table->US    = (attrs & 0x4) >> 2;
	table->RW    = (attrs & 0x2) >> 1;
	table->P     =  attrs & 0x1; 
}
