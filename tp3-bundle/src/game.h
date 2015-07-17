/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "defines.h"
#include "screen.h"

typedef enum direccion_e { ARR = 0x4, ABA = 0x7, DER = 0xA, IZQ = 0xD} direccion;

#define MAX_CANT_PIRATAS_VIVOS 8
#define NULL_ID_PIRATA         17
#define NULL_ID_MINERO         18

#define JUGADOR_A              0
#define JUGADOR_B              1

#define PIRATA_EXPLORADOR      0
#define PIRATA_MINERO          1

#define MAPA_ANCHO             80
#define MAPA_ALTO              44

typedef struct pirata_t{
    struct jugador_t *jugador;
    unsigned char type;
    uint index;
    uint id; //104000
    uint clock;
    int  pos;
    uint posDestino;
    // id unica, posicion, tipo, reloj
} pirata_t;

typedef struct jugador_t{
    pirata_t piratas[MAX_CANT_PIRATAS_VIVOS];
    pirata_t mineros_pendientes[MAX_CANT_PIRATAS_VIVOS]; // porque hay hasta 8 botines
    uint posiciones_descubiertas[MAPA_ALTO * MAPA_ANCHO - 1];
    uint ultima_posicion_descubierta;
    uint pos_puerto;
    uint puntuacion;
    uint index;
    // coordenadas puerto, posiciones exploradas, mineros pendientes, etc
} jugador_t;

extern jugador_t jugadorA, jugadorB;

void game_inicializar();

// ~ auxiliares dadas ~
uint game_xy2lineal();
pirata_t* id_pirata2pirata(uint id);

// ~ auxiliares sugeridas o requeridas (segun disponga enunciado) ~
uint game_pirata_inicializar(uint type, uint jugador, uint opcional_pos);
void game_pirata_erigir(pirata_t *pirata, jugador_t *j, uint tipo, uint parametros);
void game_pirata_habilitar_posicion(jugador_t *j, pirata_t *pirata, int x, int y);
void game_pirata_exploto();
void tarea_suicidarse();

void game_jugador_inicializar(jugador_t *j);
void game_jugador_lanzar_pirata(jugador_t *j, uint tipo, int x, int y);
void game_jugador_erigir_pirata(uint jugador, pirata_t* pirata, uint tipo, uint parametros);
void game_jugador_anotar_punto(jugador_t *j);
void game_explorar_posicion(pirata_t *pirata, int x, int y);

uint game_valor_tesoro(uint x, uint y);
void game_calcular_posiciones_vistas(int *vistas_x, int *vistas_y, int x, int y);
pirata_t*  game_pirata_en_posicion(uint x, uint y);
jugador_t* game_obtener_jugador(uint);

uint game_syscall_pirata_posicion(uint id, int idx);
void game_syscall_pirata_mover(uint id, direccion key);
void game_syscall_manejar(uint syscall, uint param1);
void game_syscall_cavar(uint id);
void game_tick(uint id_pirata);
void game_terminar_si_es_hora();
void game_atender_teclado(unsigned char tecla);
uint game_dir2xy(direccion dir, int *x, int *y);
uint game_xy2lineal(uint x, uint y);
uint game_lineas2xy_formato(uint pos);
uint obtener_posicion_botin(uint);
void atender_debug();

void agregar_posiciones_mapeadas(pirata_t *pirata);
char hay_mineros_disponibles();
uint obtener_pos_cavar_pendiente(jugador_t* jugador);
void game_mapear_posicion(uint id, uint pos);
char posicion_mapeada(uint pos, jugador_t* jugador);
void explorar_posiciones_iniciales(pirata_t* pirata);

#endif  /* !__GAME_H__ */
