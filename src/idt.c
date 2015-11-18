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

//ver: apuntes/compuerta de interrupcion.png
#define IDT_ENTRY(numero)                                                                                        \
    idt[numero].offset_0_15  = (unsigned short) ((unsigned int)(&_isr ## numero) & (unsigned int) 0xFFFF);        \
    idt[numero].segsel       = (unsigned short) 0x40;   /* codigo de nivel 0 en toda la memoria */ \
    idt[numero].attr         = (unsigned short) 0x8E00; /* P-DPL-0D110-000-||||| / 1-00-01110-000-||||| */ \
    idt[numero].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## numero) >> 16 & (unsigned int) 0xFFFF);

#define IDT_ENTRY_SYSCALL(numero)                                                                                        \
    idt[numero].offset_0_15  = (unsigned short) ((unsigned int)(&_isr ## numero) & (unsigned int) 0xFFFF);        \
    idt[numero].segsel       = (unsigned short) 0x40;   /* codigo de nivel 0 en toda la memoria */ \
    idt[numero].attr         = (unsigned short) 0xEE00; /* P-DPL-0D110-000-||||| / 1-11-01110-000-||||| */ \
    idt[numero].offset_16_31 = (unsigned short) ((unsigned int)(&_isr ## numero) >> 16 & (unsigned int) 0xFFFF);


void idt_inicializar() { //TODO: falta separar IDT_ENTRY por cada una en particular
    // Excepciones | 9, 15 y 21 en adelante quedan reservadas
    IDT_ENTRY(0);  // Divide error
    IDT_ENTRY(1);  // Debug Exception
    IDT_ENTRY(2);  // NMI Interrupt
    IDT_ENTRY(3);  // Breakpoint
    IDT_ENTRY(4);  // Overflow
    IDT_ENTRY(5);  // BOUND Range Exceeded
    IDT_ENTRY(6);  // Invalid Opcode
    IDT_ENTRY(7);  // Device not Available
    IDT_ENTRY(8);  // Double fault (tarjeta roja :P)
    IDT_ENTRY(10); // Invalid TSS
    IDT_ENTRY(11); // Segment Not Present
    IDT_ENTRY(12); // Stack-Segment Fault
    IDT_ENTRY(13); // General Protection
    IDT_ENTRY(14); // Page Fault
    IDT_ENTRY(16); // x87 FPU floating-Point Error
    IDT_ENTRY(17); // Alignment Check
    IDT_ENTRY(18); // Machine Check
    IDT_ENTRY(19); // SIMD floating-Point Exception
    IDT_ENTRY(20); // Virtualization Exception
    IDT_ENTRY(32); // Tick
    IDT_ENTRY(33); // Teclado
    IDT_ENTRY_SYSCALL(70); // REVISAR ESTO ES LA INTERRUPCION DE SOFT
}
    
