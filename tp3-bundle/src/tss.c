/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "mmu.h"

extern uint get_eflags();
extern uint get_ptl();
extern uint get_cr3();
extern uint get_eip();


tss tss_inicial;
tss tss_idle;

tss tss_jugadorA[MAX_CANT_PIRATAS_VIVOS];
tss tss_jugadorB[MAX_CANT_PIRATAS_VIVOS];



void inicializar_idle_cr3(){ //punto B, creo que esta "bien" hecho
    uint mem_fisica = (uint) 0x00016000;
    uint mem_virt   = (uint) 0x00016000;
    uint cr3   = 0x27000;
    uint attrs = 0;

    mmu_mapear_pagina(mem_virt, cr3, mem_fisica, attrs);
}

void tss_inicializar() {
	//tss inicial es basura(no nos importa)
	  tss_inicial.ptl 	   = 0;
	  tss_inicial.unused0  = 0;
    tss_inicial.esp0 	   = 0;
    tss_inicial.ss0 	   = 0;
    tss_inicial.unused1  = 0;
    tss_inicial.esp1 	   = 0;
    tss_inicial.ss1 	   = 0;
    tss_inicial.unused2  = 0;
    tss_inicial.esp2 	   = 0;
    tss_inicial.ss2 	   = 0;
    tss_inicial.unused3  = 0;
    tss_inicial.cr3 	   = 0;
    tss_inicial.eip 	   = 0;
    tss_inicial.eflags   = 0;
    tss_inicial.eax 	   = 0;
    tss_inicial.ecx 	   = 0;
    tss_inicial.edx 	   = 0;
    tss_inicial.ebx 	   = 0;
    tss_inicial.esp 	   = 0;
    tss_inicial.ebp 	   = 0;
    tss_inicial.esi 	   = 0;
    tss_inicial.edi 	   = 0;
    tss_inicial.es 		   = 0;
    tss_inicial.unused4  = 0;
    tss_inicial.cs 		   = 0;
    tss_inicial.unused5  = 0;
    tss_inicial.ss 		   = 0;
    tss_inicial.unused6  = 0;
    tss_inicial.ds 		   = 0;
    tss_inicial.unused7  = 0;
    tss_inicial.fs 		   = 0;
    tss_inicial.unused8  = 0;
    tss_inicial.gs 		   = 0;
    tss_inicial.unused9  = 0;
    tss_inicial.ldt 	   = 0;
    tss_inicial.unused10 = 0;
    tss_inicial.dtrap 	 = 0;
    tss_inicial.iomap 	 = 0; 

    //seteo el segmento de tss inicial
    gdt[13].base_0_15   = (unsigned short) ( (uint) &tss_inicial & 0xFFFF);
    gdt[13].base_23_16  = (unsigned char)  ( (uint) &tss_inicial >> 16); 
    gdt[13].base_31_24  = (unsigned char)  ( (uint) &tss_inicial >> 24); 
    
    gdt[13].limit_0_15  = (unsigned short) ( ((uint) &tss_inicial + (sizeof(tss_inicial) - 1)) & 0xFFFF);
    gdt[13].limit_16_19 = (unsigned char)  ( ((uint) &tss_inicial + (sizeof(tss_inicial) - 1)) >> 16); // pregutnar!!! la estructura lo corta?

    //TODO: arreglar los get
    tss_idle.ptl 	    = 0;
	  tss_idle.unused0  = 0;
    tss_idle.esp0 	  = 0x27000;
    tss_idle.ss0 	    = 0x50;
    tss_idle.unused1  = 0;
    tss_idle.esp1 	  = 0;
    tss_idle.ss1 	    = 0;
    tss_idle.unused2  = 0;
    tss_idle.esp2 	  = 0;
    tss_idle.ss2 	    = 0;
    tss_idle.unused3  = 0;
    tss_idle.cr3 	  = 0x27000;
    tss_idle.eip 	  = 0x16000;
    tss_idle.eflags = 0x202;
    tss_idle.eax 	  = 0;
    tss_idle.ecx 	  = 0;
    tss_idle.edx 	  = 0;
    tss_idle.ebx 	  = 0;
    tss_idle.esp 	  = 0x27000;
    tss_idle.ebp 	  = 0x27000;
    tss_idle.esi 	  = 0;
    tss_idle.edi 	  = 0;
    tss_idle.es 	  = 0x50;
    tss_idle.unused4  = 0;
    tss_idle.cs 	  = 0x40;
    tss_idle.unused5  = 0;
    tss_idle.ss 	  = 0x50;
    tss_idle.unused6  = 0;
    tss_idle.ds 	  = 0x50;
    tss_idle.unused7  = 0;
    tss_idle.fs 	  = 0x50;
    tss_idle.unused8  = 0;
    tss_idle.gs 	  = 0x50;
    tss_idle.unused9  = 0;
    tss_idle.ldt 	  = 0;
    tss_idle.unused10 = 0;
    tss_idle.dtrap 	  = 0;
    tss_idle.iomap 	  = 0xFFFF;

    //setea el segmento de tss idle
    gdt[14].base_0_15   = (unsigned short) ( (uint) &tss_idle & 0xFFFF );
    gdt[14].base_23_16  = (unsigned char)  ( (uint) &tss_idle >> 16 ); 
    gdt[14].base_31_24  = (unsigned char)  ( (uint) &tss_idle >> 24 ); 
   
    gdt[14].limit_0_15  = (unsigned short) ( ((uint) &tss_idle + (sizeof(tss_idle) - 1)) & 0xFFFF);
    gdt[14].limit_16_19 = (unsigned char)  ( ((uint) &tss_idle + (sizeof(tss_idle) - 1)) >> 16); // pregutnar!!! la estructura lo corta?

    //setea dir virtual de la tarea idle en la tabla de directorios de paginas (cr3) del kernel
    //inicializar_idle_cr3();
}

uint inicializar_tarea(uint jugador, ushort jugador_posicion){
   tss *jugador_actual = tss_obtener_jugador(jugador);

   jugador_actual[jugador_posicion].ptl      = 0;
   jugador_actual[jugador_posicion].unused0  = 0;
   jugador_actual[jugador_posicion].esp0     = (uint) mmu_gimme_gimme_page_wachin();
   jugador_actual[jugador_posicion].ss0      = (ushort) 0x27000;
   jugador_actual[jugador_posicion].unused1  = 0;
   jugador_actual[jugador_posicion].esp1     = 0;
   jugador_actual[jugador_posicion].ss1      = 0;
   jugador_actual[jugador_posicion].unused2  = 0;
   jugador_actual[jugador_posicion].esp2     = 0;
   jugador_actual[jugador_posicion].ss2      = (ushort) 0x1000 ;//+ (uint) x;
   jugador_actual[jugador_posicion].unused3  = 0;
   
   uint* cr3 = &jugador_actual[jugador_posicion].cr3;
   inicializar_dir_pirata(cr3, (uint)0x1000 + jugador + ((uint)0x1000*jugador),  jugador);
   
   jugador_actual[jugador_posicion].eip      = 0x400000;
   jugador_actual[jugador_posicion].eflags   = (uint) 0x202;
   jugador_actual[jugador_posicion].eax      = 0;
   jugador_actual[jugador_posicion].ecx      = 0;
   jugador_actual[jugador_posicion].edx      = 0;
   jugador_actual[jugador_posicion].ebx      = 0;
   jugador_actual[jugador_posicion].esp      = 0x401000-0x12;
   jugador_actual[jugador_posicion].ebp      = 0x401000-0x12;
   jugador_actual[jugador_posicion].esi      = 0;
   jugador_actual[jugador_posicion].edi      = 0;
   jugador_actual[jugador_posicion].es       = 0x50;
   jugador_actual[jugador_posicion].unused4  = 0;
   jugador_actual[jugador_posicion].cs       = 0x48;
   jugador_actual[jugador_posicion].unused5  = 0;
   jugador_actual[jugador_posicion].ss       = 0x50;
   jugador_actual[jugador_posicion].unused6  = 0;
   jugador_actual[jugador_posicion].ds       = 0x50;
   jugador_actual[jugador_posicion].unused7  = 0;
   jugador_actual[jugador_posicion].fs       = 0x50;
   jugador_actual[jugador_posicion].unused8  = 0;
   jugador_actual[jugador_posicion].gs       = 0x50;
   jugador_actual[jugador_posicion].unused9  = 0;
   jugador_actual[jugador_posicion].ldt      = 0;
   jugador_actual[jugador_posicion].unused10 = 0;
   jugador_actual[jugador_posicion].dtrap    = 0;
   jugador_actual[jugador_posicion].iomap    = 0xFFFF;

    ushort gdt_posicion = obtener_segmento_disponible();

    //setea segmento libre
    gdt[gdt_posicion].base_0_15   = (unsigned short) ( (uint) jugador_actual & 0xFFFF );
    gdt[gdt_posicion].base_23_16  = (unsigned char)  ( (uint) jugador_actual >> 16 ); 
    gdt[gdt_posicion].base_31_24  = (unsigned char)  ( (uint) jugador_actual >> 24 ); 
   
    gdt[gdt_posicion].limit_0_15  = (unsigned short) ( ((uint) jugador_actual + (sizeof(jugador_actual) - 1)) & 0xFFFF);
    gdt[gdt_posicion].limit_16_19 = (unsigned char)  ( ((uint) jugador_actual + (sizeof(jugador_actual) - 1)) >> 16); // pregutnar!!! la estructura lo corta?

    gdt[gdt_posicion].p = 1;

    //devuelve selector
    return (gdt_posicion << 3);
}

ushort obtener_segmento_disponible(){
    int i = 15;
    while( gdt[i].p == 1 ) { i++; }
    return i;
}

tss* tss_obtener_jugador(uint jugador){
    if (jugador == JUGADOR_A) { 
        return (tss*) &tss_jugadorA;
    } else {
        return (tss*) &tss_jugadorB; 
    }
}