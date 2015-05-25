
/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "defines.h"
#include "idt.h"
#include "isr.h"

#include "tss.h"

idt_entry idt[255] = { }; //TODO: preguntar porque inicializarla con una funcion? no puedo inicializarla aca al igual que la gdt??

idt_descriptor IDT_DESC = {
    sizeof(idt) - 1,
    (unsigned int) &idt
};

/*
    La siguiente es una macro de EJEMPLO para ayudar a armar entradas de
    interrupciones. Para usar, descomentar y completar CORRECTAMENTE los
    atributos y el registro de segmento. Invocarla desde idt_inicializar() de
    la siguiene manera:

    void idt_inicializar() {
        IDT_ENTRY(0);
        ...
        IDT_ENTRY(19);

        ...
    }
*/

//TODO: preguntar, son todos las interrupciones segmentos iguales? o tienen distintos privilegios? que la compuerta tengo un nivel de privilegios diferente en que afecta?
//ver: apuntes/compuerta de interrupcion.png
#define IDT_ENTRY(numero)                                                                                        \
    idt[numero].offset_0_15  = (unsigned short) ((unsigned int)(&_isr ## numero) & (unsigned int) 0xFFFF);        \
    idt[numero].segsel       = (unsigned short) 0x40;   /* codigo de nivel 0 en toda la memoria TODO: preguntar, hay un segmento predeterminado de memoria para esto??*/ \
    idt[numero].attr         = (unsigned short) 0x8700; /* P-DPL-0D110-000-|||| / 1-000-01110-000-|||| / 1000 - 0111 - 0000*/ \
    idt[numero].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## numero) >> 16 & (unsigned int) 0xFFFF);

void idt_inicializar() {
    // Excepciones
    IDT_ENTRY(0);
}
