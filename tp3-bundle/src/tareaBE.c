/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "colors.h"
#include "defines.h"
#include "game.h"
#include "syscall.h"

void task(int x, int y) {
    /* Tarea */
    int ancho = 1;
    int alto  = 2;


    while(1)
    {
        int i;
        syscall_mover(IZQ);
        ancho = ancho + 1;

        for (i = 1; i < alto; i++)
            syscall_mover(ARR);

        for (i = 1; i < ancho; i++)
            syscall_mover(DER);

        syscall_mover(ARR);
        alto = alto + 1;

        for (i = 1; i < ancho; i++)
            syscall_mover(IZQ);

        for (i = 1; i < alto; i++)
            syscall_mover(ABA);
    }

  while(1) { __asm __volatile("mov $2, %%eax":::"eax"); }
}
