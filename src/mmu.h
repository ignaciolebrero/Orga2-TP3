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

#define KERNEL_PAGE_DIRECTORY 0X27000
#define KERNEL_PAGE_TABLE	  0X28000

typedef struct pageManager{
	uint  cantidad;
	uint* primera_libre;
} __attribute__((__packed__)) pageManager;

pageManager paginas_libres; 

typedef struct page_selector{
	uint contenido[1024];
} __attribute__((__packed__)) page_selector;

typedef struct page_directory_entry{
	unsigned char P:1;
	unsigned char RW:1;
	unsigned char US:1;
	unsigned char PWT:1;
	unsigned char PCD:1;
	unsigned char A:1;
	unsigned char disp0:1;
	unsigned char PS:1;
	unsigned char G:1;
	unsigned char disp:3;
	unsigned int  dir_pagina_tabla:20;
} __attribute__((__packed__)) page_directory_entry;

typedef struct page_table_entry{
	unsigned char P:1;
	unsigned char RW:1;
	unsigned char US:1;
	unsigned char PWT:1;
	unsigned char PCD:1;
	unsigned char A:1;
	unsigned char D:1;
	unsigned char PAT:1;
	unsigned char G:1;
	unsigned char disp:3;
	unsigned int  dir_pagina_mem:20;
} __attribute__((__packed__)) page_table_entry;

void* mmu_gimme_gimme_page_wachin();

void mmu_inicializar();

uint inicializar_dir_pirata(uint fisicmem, uint elteam, uint tipo);
	
void mmu_mapear_posicion_mapa(uint cr3, uint posicion);

void mmu_mapear_pagina(uint virt, uint cr3, uint fisica, uint attrs);

void mmu_unmapear_pagina(uint virt, uint cr3);

void mmu_mover_codigo_pirata(uint cr3, uint* source, uint* destino);

char hay_paginas();
	   /* funciones de directorios */
/* --------------------------------------------- */

void init_page_table(page_table_entry* table);

void init_directory_table(page_directory_entry* table);

void set_directory_entry(page_directory_entry* dir, page_table_entry* table, uint attrs);

void set_table_entry(page_table_entry* table, uint fisic, uint attrs);
	

	   /* funciones pre-refactorizacion */
/* --------------------------------------------- */

void init_table(uint* table);
#endif	/* !__MMU_H__ */
