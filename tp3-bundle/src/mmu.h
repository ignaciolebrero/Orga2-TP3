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

uint* mmu_gimme_gimme_page_wachin();

void mmu_inicializar();

void inicializar_dir_pirata(uint* cr3, uint fisicmem, uint elteam, uint tipo);
	
void mmu_mapear_pagina(uint virt, uint cr3, uint fisica, uint attrs);

void mmu_unmapear_pagina(uint virt, uint cr3);

void init_table(uint* table);

void mmu_mover_codigo_pirata(uint* origen, uint* destino);

#endif	/* !__MMU_H__ */
