/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#define LS_INLINE static __inline __attribute__((always_inline))

/*
 * Syscalls
 */

LS_INLINE unsigned int syscall_mover(direccion d) {
    int ret;

    __asm __volatile(
        "mov $1, %%eax \n"
        "mov %0, %%ecx \n"
        "int $0x66     \n"
        : /* no output*/
        : "m" (d)
        : "eax"
    );

    __asm __volatile("mov %%eax, %0" : "=r" (ret));

    return ret;
}

LS_INLINE unsigned int syscall_cavar() {
    int ret;

    __asm __volatile(
        "mov $2, %%eax \n"
        "int $0x66     \n"
        : /* no output*/
        :
        : "eax"
    );

    __asm __volatile("mov %%eax, %0" : "=r" (ret));

    return ret;
}

LS_INLINE unsigned int syscall_posicion(int idx) {
    int ret;

    __asm __volatile(
        "mov $3, %%eax \n"
        "mov %0, %%ecx \n"
        "int $0x66     \n"
        : /* no output*/
        : "m" (idx)
        : "eax"
    );

    __asm __volatile("mov %%eax, %0" : "=r" (ret));

    return ret;
}

#endif  /* !__SYSCALL_H__ */
