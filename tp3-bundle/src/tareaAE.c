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
    int restantex = MAPA_ANCHO-3;
    int restantey = MAPA_ALTO-3;

    while(1)
    {
        int i;
        for (i = 0; i < restantex; i++)
            syscall_mover(DER);

        restantex = restantex - 3;

        for (i = 0; i < restantey; i++)
            syscall_mover(ABA);

        restantey = restantey - 3;

        for (i = 0; i < restantex; i++)
            syscall_mover(IZQ);

        restantex = restantex - 3;

        for (i = 0; i < restantey; i++)
            syscall_mover(ARR);

        restantey = restantey - 3;
    }

    while(1) { __asm __volatile("mov $2, %%eax":::"eax"); }
}
