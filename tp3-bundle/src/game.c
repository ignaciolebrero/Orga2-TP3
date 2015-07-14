/* ** por compatibilidad se omiten tildes **
================================================================================
TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "mmu.h"
#include "tss.h"
#include "screen.h"
#include "sched.h"

#include <stdarg.h>


#define POS_INIT_A_X     		 1
#define POS_INIT_A_Y     		 1
#define POS_INIT_B_X     		 MAPA_ANCHO - 2
#define POS_INIT_B_Y     		 MAPA_ALTO  - 2

#define CANT_POSICIONES_VISTAS   9
#define MAX_SIN_CAMBIOS          999

#define BOTINES_CANTIDAD 		 8

uint botines[BOTINES_CANTIDAD][3] = { // TRIPLAS DE LA FORMA (X, Y, MONEDAS)
    {30,  3, 50}, {30, 38, 50}, {15, 21, 100}, {45, 21, 100} ,
    {49,  3, 50}, {49, 38, 50}, {64, 21, 100}, {34, 21, 100}
};

jugador_t jugadorA;
jugador_t jugadorB;

pirata_t* piratas[MAX_CANT_PIRATAS_VIVOS*2];

void* error()
{
	__asm__ ("int3");
	return 0;
}

uint game_xy2lineal (uint x, uint y) {
	return (y * MAPA_ANCHO + x);
}

void game_lineal2xy (uint pos, int *x, int *y) {
	*x = pos % MAPA_ANCHO;
	*y = pos / MAPA_ANCHO;
}

uint game_lineal2xy_formato (uint pos) {
	uint x = pos % MAPA_ANCHO;
	uint y = pos / MAPA_ANCHO;
	return (y << 8 | x);
}

uint game_posicion_valida(uint x, uint y) {
	return ( ((x >= 0) && (y >= 0)) && ((x < MAPA_ANCHO) && (y < MAPA_ALTO)) );
}

pirata_t* id_pirata2pirata(uint id_pirata)
{
	if (id_pirata > 7) {
		return jugadorA.piratas[id_pirata % 8];
	} else {
		return jugadorA.piratas[id_pirata % 8];
	}
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

void game_jugador_inicializar_mapa(jugador_t *jug)
{
	uint i;
	for (i = 0; i < 80 * 44; i++) {
		jug->posiciones_descubiertas[i] = 0;
	}
	jug->ultima_posicion_descubierta = 0;
}

void game_inicializar()
{
	uint i;
	pirata_t pirata_nulo, minero_nulo;
	pirata_nulo.id = NULL_ID_PIRATA;
	minero_nulo.id = NULL_ID_MINERO;

	for (i=0; i<8; i++) {
		jugadorA.piratas[i] = &pirata_nulo;
		jugadorB.piratas[i] = &pirata_nulo;
		jugadorA.mineros_pendientes[i] = &minero_nulo;
		jugadorB.mineros_pendientes[i] = &minero_nulo;
	}

	game_jugador_inicializar_mapa(&jugadorA);
	game_jugador_inicializar_mapa(&jugadorB);

	jugadorA.index = 0;
	jugadorA.pos_puerto = 0;
	jugadorA.puntuacion = 0;

	jugadorB.index = 1;
	jugadorB.puntuacion = 0;
	jugadorB.pos_puerto = (MAPA_ALTO * MAPA_ANCHO) - 1;

}

void game_jugador_inicializar(jugador_t *j)
{
	static int index = 0;

	j->index = index++;
    
    uint i = 0;
    for (i = 0; i < 8; ++i) {
    	j->piratas[i] 	 		 = NULL;
    	j->mineros_pendientes[i] = NULL;
    }
}

ushort game_obtener_posicion_pirata_disponible(jugador_t* jugador)
{
    ushort i = 0;
    while( i < 8 ) {
    	if (jugador->piratas[i]->id != NULL_ID_PIRATA) {
    		return i;
    	}
    	i++; 
 	}
    return i;
}

ushort game_obtener_posicion_minero_disponible(jugador_t* jugador)
{
    ushort i = 0;
    while( (jugador->mineros_pendientes[i]->id == NULL_ID_MINERO) && (i < 8) ) { i++; }
    return i;
}

jugador_t* game_obtener_jugador(uint jugador)
{
	if (jugador == 0) {
		return &jugadorA;
	} else {
		return &jugadorB;
	}
}

void game_pirata_inicializar(uint type, uint jugador, uint opcional_pos)
{
	jugador_t* jugador_actual = game_obtener_jugador(jugador);
	uint i = game_obtener_posicion_pirata_disponible(jugador_actual);
	if( i < 8 ){
		pirata_t pirata_actual;
		pirata_actual.jugador = jugador_actual;
		pirata_actual.pos     = pirata_actual.jugador->pos_puerto;
		pirata_actual.type    = type;
		pirata_actual.clock   = 0; 
		pirata_actual.id	  = i + (jugador * 8);
		jugador_actual->piratas[i] = &pirata_actual;
		game_jugador_erigir_pirata(jugador, &pirata_actual, i, opcional_pos);
		agregar_posiciones_mapeadas(&pirata_actual);
	} else if (type == 1) {
		i = game_obtener_posicion_minero_disponible(jugador_actual);
		pirata_t* pirata_actual   = jugador_actual->mineros_pendientes[i];
		pirata_actual->posDestino = opcional_pos;
		pirata_actual->type 	  = type;
		pirata_actual->id 		  = NULL_ID_PIRATA;
	}
}

void game_jugador_erigir_pirata(uint jugador, pirata_t* pirata, uint posicion, uint parametros)
{
    sched_agregar_tarea(jugador, posicion, pirata->type, parametros);
}

void agregar_posiciones_mapeadas(pirata_t *pirata){
	uint i;
	uint cr3 = tss_obtener_cr3(pirata->id);
	for (i = 0; i < pirata->jugador->ultima_posicion_descubierta; i++){
		mmu_mapear_posicion_mapa(cr3, pirata->jugador->posiciones_descubiertas[i]);	
	}
}

void game_tick(uint id_pirata)
{
	if (id_pirata < NULL_ID_PIRATA) {
		pirata_t* pirata = id_pirata2pirata(id_pirata);
		screen_actualizar_reloj_pirata(pirata->jugador, pirata);
	}
	
	screen_actualizar_reloj_global();
}


void game_pirata_relanzar(pirata_t *pirata, jugador_t *j, uint tipo)
{
}

void game_jugador_lanzar_pirata(jugador_t *j, uint tipo, int x, int y)
{
}

void game_pirata_habilitar_posicion(jugador_t *j, pirata_t *pirata, int x, int y)
{
	uint pos = game_xy2lineal(x, y);
	if ( pirata->id != NULL_ID_PIRATA &&
		game_posicion_valida(x,y)     && 
		!posicion_mapeada(pos, j) ){
			//mapeo la posicion a todos los piratas disponibles en ese momento
			uint i;
			for(i = 0; i < 8; i++){
				uint id = j->piratas[i]->id;
				if (id != NULL_ID_PIRATA) {
					mmu_mapear_posicion_mapa(tss_obtener_cr3(id), pos); //mapea posicion nueva
				}
			}
			//la agrega a la tabla de posiciones descubiertas del jugador
			j->posiciones_descubiertas[j->ultima_posicion_descubierta] = pos;
			j->ultima_posicion_descubierta++;
			if( obtener_posicion_botin(pos) < BOTINES_CANTIDAD ) {
				breakpoint();
				game_pirata_inicializar(PIRATA_MINERO, j->index, game_lineal2xy_formato(pos));
			}
	}
}

void game_explorar_posicion(pirata_t* pirata, int c, int f)
{
	game_pirata_habilitar_posicion(pirata->jugador, pirata, c  , f  );
	game_pirata_habilitar_posicion(pirata->jugador, pirata, c+1, f  );
	game_pirata_habilitar_posicion(pirata->jugador, pirata, c-1, f  );
	game_pirata_habilitar_posicion(pirata->jugador, pirata, c  , f-1);
	game_pirata_habilitar_posicion(pirata->jugador, pirata, c  , f+1);
	game_pirata_habilitar_posicion(pirata->jugador, pirata, c+1, f+1);
	game_pirata_habilitar_posicion(pirata->jugador, pirata, c+1, f-1);
	game_pirata_habilitar_posicion(pirata->jugador, pirata, c-1, f+1);
	game_pirata_habilitar_posicion(pirata->jugador, pirata, c-1, f-1);
}

char posicion_mapeada(uint pos, jugador_t* jugador){
	uint i;
	for(i = 0; i < jugador->ultima_posicion_descubierta; i++){
		if (jugador->posiciones_descubiertas[i] == pos) { return TRUE; }
	}
	return FALSE;
}

void game_syscall_pirata_mover(uint id, direccion dir)
{
	int x_orig=0, y_orig=0;
	int pirx=0  , piry=0;
    pirata_t* pirata = id_pirata2pirata(id);

	game_dir2xy(dir, &x_orig, &y_orig);
	game_lineal2xy(pirata->pos, &pirx, &piry);
	
	uint x = pirx + x_orig;
	uint y = piry + y_orig;

	if (game_posicion_valida(x, y)) {
		if (!posicion_mapeada( game_xy2lineal(x,y), pirata->jugador)) {
			if (pirata->type == PIRATA_MINERO) { 
				game_pirata_exploto();
			} else {
				screen_pintar(35, C_BG_BLUE, y, x); //TODO: pintar jugador correspiente
				game_explorar_posicion(pirata, x, y);
				mmu_mover_codigo_pirata(rcr3(), (uint*) (game_xy2lineal(x_orig, y_orig) + 0x500000) , (uint*) (game_xy2lineal(x, y) + 0x500000) );
				pirata->pos = game_xy2lineal(x, y);
			}
		} else {
				mmu_mover_codigo_pirata(rcr3(), (uint*) (game_xy2lineal(x_orig, y_orig) + 0x500000) , (uint*) (game_xy2lineal(x, y) + 0x500000) );
		}
	} else {
		game_pirata_exploto();
	}
}

void game_syscall_cavar(uint id)
{
    pirata_t* pirata = id_pirata2pirata(id);
    uint i = obtener_posicion_botin(pirata->pos);

    if ( i < BOTINES_CANTIDAD ) {
    	pirata->jugador->puntuacion++;
    	botines[i][2]--;
    	if ( botines[i][2] == 0 ) {
    		game_pirata_exploto();
    	}
    }
}

uint game_syscall_pirata_posicion(uint id, int idx)
{
	pirata_t* pirata;
    if (idx == -1) {
    	pirata = id_pirata2pirata(id);
    } else {
    	pirata = id_pirata2pirata( (uint) idx);
    }
    return game_lineal2xy_formato(pirata->pos);
}

void game_syscall_manejar(uint syscall, uint param1)
{
    uint id = sched_tarea_actual_id();
    uint posicion;
    direccion dir;

    switch(syscall){
    	case(0x1):
    	    dir = (direccion) param1;
    	    game_syscall_pirata_mover(id, dir);
    	break;
    	case(0x2):
    		game_syscall_cavar(id);
    	break;
    	case(0x3):	
    		posicion = param1;
    		game_syscall_pirata_posicion(id, posicion);
    	break;
    }
}

uint obtener_posicion_botin(uint posicion)
{
	int x, y, i=0;
	char resultado = FALSE;
	game_lineal2xy(posicion, &x, &y);

	while (i < BOTINES_CANTIDAD && !resultado) {
		resultado = resultado || ( botines[i][0] == x && botines[i][1] == y && botines[i][2] > 0);
		i++;
	}
	return i;
}

void game_pirata_exploto()
{
	//TODO: averiguar en cuales excepciones hay que matar al pirata
	pirata_t* pirata = id_pirata2pirata(sched_tarea_actual_id());
	pirata->id 	 	 = NULL_ID_PIRATA;
	scheduler_matar_actual_tarea_pirata();
	
	/*if (hay_mineros_disponibles(pirata->jugador)) {
		uint posCavar = obtener_pos_cavar_pendiente(pirata->jugador);
		game_pirata_inicializar(PIRATA_MINERO, pirata->jugador->index, posCavar);
	}*/
}

char hay_mineros_disponibles(jugador_t* jugador)
{
	uint i;
	char result = FALSE;
	
	for(i=0; i < 8 && !result; i++){
		result = result || jugador->mineros_pendientes[i]->id != NULL_ID_MINERO;
	}
	
	return result;
}

uint obtener_pos_cavar_pendiente(jugador_t* jugador)
{
	uint i = 0;
	while (jugador->mineros_pendientes[i]->id == NULL_ID_MINERO) { i++; }		
	
	jugador->mineros_pendientes[i]->id = NULL_ID_MINERO;
	return jugador->mineros_pendientes[i]->posDestino;
}

pirata_t* game_pirata_en_posicion(uint x, uint y)
{
	pirata_t* pirata = NULL;
	uint i = 0;

	while (i<8 && pirata == NULL) {
		int xl, yl;

		game_lineal2xy(jugadorA.piratas[i]->pos, &xl, &yl);
		if ( xl == x && yl == y ) {
			pirata = jugadorA.piratas[i];
		}

		game_lineal2xy(jugadorB.piratas[i]->pos, &xl, &yl);
		if ( xl == x && yl == y ) {
			pirata = jugadorB.piratas[i];
		}		

		i++;
	}
	return pirata;
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


void game_atender_teclado(unsigned char jugador) //manejado desde isr.asm
{
	if(jugador == 0){
		game_pirata_inicializar(0, 0, 0);
	} else {
		game_pirata_inicializar(0, 1, 0);
	}
}