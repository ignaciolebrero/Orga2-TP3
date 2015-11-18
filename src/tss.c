/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "mmu.h"
#include "screen.h"

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
    uint attrs = 0x3;

    mmu_mapear_pagina(mem_virt, cr3, mem_fisica, attrs);
}

void tss_inicializar() {
	//tss inicial es basua(no nos importa)
	tss_inicial.ptl 	 = 0;
	tss_inicial.unused0  = 0;
    tss_inicial.esp0 	 = 0;
    tss_inicial.ss0 	 = 0;
    tss_inicial.unused1  = 0;
    tss_inicial.esp1 	 = 0;
    tss_inicial.ss1 	 = 0;
    tss_inicial.unused2  = 0;
    tss_inicial.esp2 	 = 0;
    tss_inicial.ss2 	 = 0;
    tss_inicial.unused3  = 0;
    tss_inicial.cr3 	 = 0;
    tss_inicial.eip 	 = 0;
    tss_inicial.eflags   = 0;
    tss_inicial.eax 	 = 0;
    tss_inicial.ecx 	 = 0;
    tss_inicial.edx 	 = 0;
    tss_inicial.ebx 	 = 0;
    tss_inicial.esp 	 = 0;
    tss_inicial.ebp 	 = 0;
    tss_inicial.esi 	 = 0;
    tss_inicial.edi 	 = 0;
    tss_inicial.es 		 = 0;
    tss_inicial.unused4  = 0;
    tss_inicial.cs 		 = 0;
    tss_inicial.unused5  = 0;
    tss_inicial.ss 		 = 0;
    tss_inicial.unused6  = 0;
    tss_inicial.ds 		 = 0;
    tss_inicial.unused7  = 0;
    tss_inicial.fs 		 = 0;
    tss_inicial.unused8  = 0;
    tss_inicial.gs 		 = 0;
    tss_inicial.unused9  = 0;
    tss_inicial.ldt      = 0;
    tss_inicial.unused10 = 0;
    tss_inicial.dtrap 	 = 0;
    tss_inicial.iomap 	 = 0; 

    //seteo el segmento de tss inicial
    gdt[13].base_0_15   = (unsigned short) ( (uint) &tss_inicial & 0xFFFF);
    gdt[13].base_23_16  = (unsigned char)  ( (uint) &tss_inicial >> 16); 
    gdt[13].base_31_24  = (unsigned char)  ( (uint) &tss_inicial >> 24); 
    
    gdt[13].limit_0_15  = (unsigned short) ( (sizeof(tss_inicial) - 1) & 0xFFFF);
    gdt[13].limit_16_19 = (unsigned char)  ( (sizeof(tss_inicial) - 1) >> 16); // pregutnar!!! la estructura lo corta?
}

void tss_inicializar_idle() {
    //TODO: arreglar los get
    tss_idle.ptl 	  = 0;
	tss_idle.unused0  = 0;
    tss_idle.esp0 	  = 0x27000;
    tss_idle.ss0 	  = 0x50;
    tss_idle.unused1  = 0;
    tss_idle.esp1 	  = 0;
    tss_idle.ss1 	  = 0;
    tss_idle.unused2  = 0;
    tss_idle.esp2 	  = 0;
    tss_idle.ss2 	  = 0;
    tss_idle.unused3  = 0;
    tss_idle.cr3 	  = 0x27000;
    tss_idle.eip 	  = 0x16000;
    tss_idle.eflags   = 0x202;
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
   
    gdt[14].limit_0_15  = (unsigned short) ( (sizeof(tss_idle) - 1) & 0xFFFF);
    gdt[14].limit_16_19 = (unsigned char)  ( (sizeof(tss_idle) - 1) >> 16);

    //setea dir virtual de la tarea idle en la tabla de directorios de paginas (cr3) del kernel
    inicializar_idle_cr3();
}

uint inicializar_tarea(uint jugador, uint jugador_posicion, uint tipo, uint parametros){
    tss *jugador_actual = tss_obtener_jugador(jugador);
    uint memoria_fisica;
    if (jugador == 0) {
        memoria_fisica = 0x500000 + 0x50000*2 + 0x1000;
    } else {
        memoria_fisica = 0x500000 + 0x1000 * ((MAPA_ALTO-2) * MAPA_ANCHO + (MAPA_ANCHO-2)); // TODO: cambiar posiciones
    }

    jugador_actual[jugador_posicion].ptl     = 0;
    jugador_actual[jugador_posicion].unused0 = 0;
    jugador_actual[jugador_posicion].esp0    = (uint) mmu_gimme_gimme_page_wachin() + 0x1000;
    jugador_actual[jugador_posicion].ss0     = 0x50;
    jugador_actual[jugador_posicion].unused1 = 0;
    jugador_actual[jugador_posicion].esp1    = 0;
    jugador_actual[jugador_posicion].ss1     = 0;
    jugador_actual[jugador_posicion].unused2 = 0;
    jugador_actual[jugador_posicion].esp2    = 0;
    jugador_actual[jugador_posicion].ss2     = 0;
    jugador_actual[jugador_posicion].unused3 = 0;
    jugador_actual[jugador_posicion].cr3     = inicializar_dir_pirata(memoria_fisica, jugador, tipo);
    jugador_actual[jugador_posicion].eip     = 0x400000;
    jugador_actual[jugador_posicion].eflags  = 0x202;
    jugador_actual[jugador_posicion].eax     = 0;
    jugador_actual[jugador_posicion].ecx     = 0;
    jugador_actual[jugador_posicion].edx     = 0;
    jugador_actual[jugador_posicion].ebp     = 0;
    jugador_actual[jugador_posicion].esp     = 0x401000-0xC;
    jugador_actual[jugador_posicion].ebp     = 0x401000-0xC;
    jugador_actual[jugador_posicion].edi     = 0;
    jugador_actual[jugador_posicion].es      = 0x5B;
    jugador_actual[jugador_posicion].unused4 = 0;
    jugador_actual[jugador_posicion].cs      = 0x4B;
    jugador_actual[jugador_posicion].unused5 = 0;
    jugador_actual[jugador_posicion].ss      = 0x5B;
    jugador_actual[jugador_posicion].unused6 = 0;
    jugador_actual[jugador_posicion].ds      = 0x5B;
    jugador_actual[jugador_posicion].unused7 = 0;
    jugador_actual[jugador_posicion].fs      = 0x5B;
    jugador_actual[jugador_posicion].unused8 = 0;
    jugador_actual[jugador_posicion].gs      = 0x5B;
    jugador_actual[jugador_posicion].unused9 = 0;
    jugador_actual[jugador_posicion].iomap   = 0xFFFF;

    uint gdt_posicion = obtener_segmento_disponible();

    //setea segmento libre
    gdt[gdt_posicion].base_0_15   = (unsigned short) (uint) &(jugador_actual[jugador_posicion]);
    gdt[gdt_posicion].base_23_16  = (unsigned char)  (uint) &(jugador_actual[jugador_posicion]) >> 16; 
    gdt[gdt_posicion].base_31_24  = (unsigned char)  (uint) &(jugador_actual[jugador_posicion]) >> 24; 
   
    gdt[gdt_posicion].limit_0_15  = (unsigned short)  ( ( (uint) (sizeof(jugador_actual[jugador_posicion]) - 1)) & 0xFFFF);
    gdt[gdt_posicion].limit_16_19 = (unsigned char)   ( ( (uint) (sizeof(jugador_actual[jugador_posicion]) - 1)) >> 16);

    gdt[gdt_posicion].p = 1;
    //apilo parametros de la tarea
    apilar_parametros(parametros & 0xFF, (parametros >> 8) & 0xFF, memoria_fisica);

    //devuelve selector
    return (gdt_posicion << 3);
}

void apilar_parametros(uint x, uint y, uint dir){
    mmu_mapear_pagina(0x402000, rcr3(), dir, 0x7);

    * (uint*) (0x402000 + 0x1000 - 0x4) = y;
    * (uint*) (0x402000 + 0x1000 - 0x8) = x;
    * (uint*) (0x402000 + 0x1000 - 0xC) = 0;

    mmu_unmapear_pagina(0x402000, rcr3());
}

uint obtener_segmento_disponible(){
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

uint tss_obtener_cr3(uint id){
    tss* jugador      = tss_obtener_jugador(id / 8);
    uint posicion_tss = id % 8;
    return jugador[posicion_tss].cr3;
}

uint tss_obtener_cs(uint id){
    tss* jugador      = tss_obtener_jugador(id / 8);
    uint posicion_tss = id % 8;
    return jugador[posicion_tss].cs;
}

uint tss_obtener_ds(uint id){
    tss* jugador      = tss_obtener_jugador(id / 8);
    uint posicion_tss = id % 8;
    return jugador[posicion_tss].ds;
}

uint tss_obtener_es(uint id){
    tss* jugador      = tss_obtener_jugador(id / 8);
    uint posicion_tss = id % 8;
    return jugador[posicion_tss].es;
}

uint tss_obtener_fs(uint id){
    tss* jugador      = tss_obtener_jugador(id / 8);
    uint posicion_tss = id % 8;
    return jugador[posicion_tss].fs;
}

uint tss_obtener_gs(uint id){
    tss* jugador      = tss_obtener_jugador(id / 8);
    uint posicion_tss = id % 8;
    return jugador[posicion_tss].gs;
}

uint tss_obtener_ss(uint id){
    tss* jugador      = tss_obtener_jugador(id / 8);
    uint posicion_tss = id % 8;
    return jugador[posicion_tss].ss;
}
