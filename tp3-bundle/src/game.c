/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "mmu.h"
#include "tss.h"
#include "screen.h"

#include <stdarg.h>


#define POS_INIT_A_X                      1
#define POS_INIT_A_Y                      1
#define POS_INIT_B_X         MAPA_ANCHO - 2
#define POS_INIT_B_Y          MAPA_ALTO - 2

#define CANT_POSICIONES_VISTAS            9
#define MAX_SIN_CAMBIOS                 999

#define BOTINES_CANTIDAD 8

uint botines[BOTINES_CANTIDAD][3] = { // TRIPLAS DE LA FORMA (X, Y, MONEDAS)
                                        {30,  3, 50}, {30, 38, 50}, {15, 21, 100}, {45, 21, 100} ,
                                        {49,  3, 50}, {49, 38, 50}, {64, 21, 100}, {34, 21, 100}
                                    };

jugador_t jugadorA;
jugador_t jugadorB;


void* error()
{
	__asm__ ("int3");
	return 0;
}

uint game_xy2lineal (uint x, uint y) {
	return (y * MAPA_ANCHO + x);
}

uint game_posicion_valida(int x, int y) {
	return (x >= 0 && y >= 0 && x < MAPA_ANCHO && y < MAPA_ALTO);
}

pirata_t* id_pirata2pirata(uint id_pirata)
{
    // ~ completar ~
	return NULL;
}

uint game_dir2xy(direccion dir, int *x, int *y)
{
	switch (dir)
	{
		case IZQ: *x = -1; *y =  0; break;
		case DER: *x =  1; *y =  0; break;
		case ABA: *x =  0; *y =  1; break;
		case ARR: *x =  0; *y = -1; break;
    	default: return -1;
	}

	return 0;
}

uint game_valor_tesoro(uint x, uint y)
{
	int i;
	for (i = 0; i < BOTINES_CANTIDAD; i++)
	{
		if (botines[i][0] == x && botines[i][1] == y)
			return botines[i][2];
	}
	return 0;
}

// dada una posicion (x,y) guarda las posiciones de alrededor en dos arreglos, uno para las x y otro para las y
void game_calcular_posiciones_vistas(int *vistas_x, int *vistas_y, int x, int y)
{
	int next = 0;
	int i, j;
	for (i = -1; i <= 1; i++)
	{
		for (j = -1; j <= 1; j++)
		{
			vistas_x[next] = x + j;
			vistas_y[next] = y + i;
			next++;
		}
	}
}


void game_inicializar()
{
}

void game_jugador_inicializar_mapa(jugador_t *jug)
{
}

void game_jugador_inicializar(jugador_t *j)
{
	static int index = 0;

	j->index = index++;
    // ~ completar ~

}

void game_pirata_inicializar(pirata_t *pirata, jugador_t *j, uint index, uint id)
{
}

void game_tick(uint id_pirata)
{
}


void game_pirata_relanzar(pirata_t *pirata, jugador_t *j, uint tipo)
{
}

pirata_t* game_jugador_erigir_pirata(jugador_t *j, uint tipo)
{
    // ~ completar ~

	return NULL;
}


void game_jugador_lanzar_pirata(jugador_t *j, uint tipo, int x, int y)
{
}

void game_pirata_habilitar_posicion(jugador_t *j, pirata_t *pirata, int x, int y)
{
}


void game_explorar_posicion(jugador_t *jugador, int c, int f)
{
}


uint game_syscall_pirata_mover(uint id, direccion dir)
{
    // ~ completar
    return 0;
}

uint game_syscall_cavar(uint id)
{
    // ~ completar ~

	return 0;
}

uint game_syscall_pirata_posicion(uint id, int idx)
{
    // ~ completar ~
    return 0;
}

uint game_syscall_manejar(uint syscall, uint param1)
{
    // ~ completar ~
    return 0;
}

void game_pirata_exploto(uint id)
{
}

pirata_t* game_pirata_en_posicion(uint x, uint y)
{
	return NULL;
}


void game_jugador_anotar_punto(jugador_t *j)
{
}



void game_terminar_si_es_hora()
{
}


#define KB_w_Aup    0x11 // 0x91
#define KB_s_Ado    0x1f // 0x9f
#define KB_a_Al     0x1e // 0x9e
#define KB_d_Ar     0x20 // 0xa0
#define KB_e_Achg   0x12 // 0x92
#define KB_q_Adir   0x10 // 0x90
#define KB_i_Bup    0x17 // 0x97
#define KB_k_Bdo    0x25 // 0xa5
#define KB_j_Bl     0x24 // 0xa4
#define KB_l_Br     0x26 // 0xa6
#define KB_shiftA   0x2a // 0xaa
#define KB_shiftB   0x36 // 0xb6


void game_atender_teclado(unsigned char tecla)
{
}
