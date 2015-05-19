/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "colors.h"
#include "defines.h"
#include "game.h"
#include "syscall.h"
#include "i386.h"

void task(int x_target, int y_target) {
    /* Tarea */

    //breakpoint();
    uint pos = syscall_posicion(-1);
    int x = pos & 0xFF;
    int y = pos >> 8;
    //breakpoint();
    int i;
    for(i=x; i < x_target; i++)
        syscall_mover(DER);

    for(i=y; i < y_target; i++)
        syscall_mover(ABA);

    while(1) {syscall_cavar();}

  while(1) { __asm __volatile("mov $2, %%eax":::"eax"); }
}
