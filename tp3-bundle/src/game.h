/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "defines.h"

typedef enum direccion_e { ARR = 0x4, ABA = 0x7, DER = 0xA, IZQ = 0xD} direccion;

#define MAX_CANT_PIRATAS_VIVOS           8

#define JUGADOR_A                         0
#define JUGADOR_B                         1

#define MAPA_ANCHO                       80
#define MAPA_ALTO                        44


struct jugador_t;

typedef struct pirata_t
{
    uint index;
    struct jugador_t *jugador;

    // id unica, posicion, tipo, reloj
} pirata_t;


typedef struct jugador_t
{
    uint index;
    pirata_t piratas[MAX_CANT_PIRATAS_VIVOS];
    // coordenadas puerto, posiciones exploradas, mineros pendientes, etc
} jugador_t;


extern jugador_t jugadorA, jugadorB;

// ~ auxiliares dadas ~
uint game_xy2lineal();
pirata_t* id_pirata2pirata(uint id);

// ~ auxiliares sugeridas o requeridas (segun disponga enunciado) ~
void game_pirata_inicializar(pirata_t *pirata, jugador_t *jugador, uint index, uint id);
void game_pirata_erigir(pirata_t *pirata, jugador_t *j, uint tipo);
void game_pirata_habilitar_posicion(jugador_t *j, pirata_t *pirata, int x, int y);
void game_pirata_exploto(uint id);

void game_jugador_inicializar(jugador_t *j);
void game_jugador_lanzar_pirata(jugador_t *j, uint tipo, int x, int y);
pirata_t* game_jugador_erigir_pirata(jugador_t *j, uint tipo);
void game_jugador_anotar_punto(jugador_t *j);
void game_explorar_posicion(jugador_t *jugador, int x, int y);

uint game_valor_tesoro(uint x, uint y);
void game_calcular_posiciones_vistas(int *vistas_x, int *vistas_y, int x, int y);
pirata_t* game_pirata_en_posicion(uint x, uint y);

uint game_syscall_pirata_posicion(uint id, int idx);
uint game_syscall_pirata_mover(uint id, direccion key);
uint game_syscall_manejar(uint syscall, uint param1);
void game_tick(uint id_pirata);
void game_terminar_si_es_hora();
void game_atender_teclado(unsigned char tecla);


#endif  /* !__GAME_H__ */
