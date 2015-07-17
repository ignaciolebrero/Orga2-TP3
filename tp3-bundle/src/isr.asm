; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

extern inicializar_dir_pirata
extern game_reprintar_pantalla
extern game_syscall_manejar
extern game_debuggear_tarea
extern game_atender_teclado
extern game_pirata_exploto
extern mmu_mapear_pagina
extern habilitar_pic
extern resetear_pic
extern game_tick
extern print

BITS 32

;; Excepciones
global _isr0
global _isr1
global _isr2
global _isr3
global _isr4
global _isr5
global _isr6
global _isr7
global _isr8
global _isr10
global _isr11
global _isr12
global _isr13
global _isr14
global _isr16
global _isr17
global _isr18
global _isr19
global _isr20
global _isr32
global _isr33
global _isr70

;; scheduler
sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00

;; PIC
extern fin_intr_pic1

;; Sched
extern sched_tick
extern sched_tarea_actual

;; Screen
extern print

;; Msjs de excepciones
cero_mr_msg 		dw 'diviste por 0 ',0
;cero_mr_len 		equ $ - cero_mr_msg
uno_mr_msg 			dw 'Debug Exception ',0
dos_mr_msg 			dw 'NMI Interrupt ',0
tres_mr_msg 		dw 'Breakpoint',0
cuatro_mr_msg 		dw 'Overflow ',0
cinco_mr_msg 		dw 'BOUND Range Exceeded ',0
seis_mr_msg 		dw 'Invalid Opcode ',0
siete_mr_msg 		dw 'Device not Available ',0
ocho_mr_msg 		dw 'Double fault ',0
diez_mr_msg 		dw 'Invalid TSS ',0
once_mr_msg 		dw 'Segment Not Present ',0
doce_mr_msg 		dw 'Stack-Segment Fault ',0
trece_mr_msg 		dw 'General Protection ',0
catorce_mr_msg		dw 'Page Fault ',0
diezseis_mr_msg		dw 'x87 FPU floating-Point Error ',0
diezsiete_mr_msg	dw 'Alignment Check',0
diezocho_mr_msg		dw 'Machine Check ',0
dieznueve_mr_msg	dw 'SIMD floating-Point Exception ',0
veinte_mr_msg		dw 'Virtualization Exception ',0

;; Debug
debug_habilitado 	  db 0x0
pantalla_debug_activa db 0x0

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

%macro ISR 1
global _isr%1

_isr%1:
    mov eax, %1
    jmp $

%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler

;;
;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
;TODO: preguntar si es mas comodo pasarlo a un archivo en C hacer call a las funciones de ahi apra las interrupciones (suena mas facil de programar)
;TODO:revisar si compila, funciona, eclosiona(?)
_isr0:
	pushad
		jmp matar_pirata
	popad
	iret

_isr1:
	pushad
		jmp matar_pirata
	popad
	iret

_isr2:
	pushad
		jmp matar_pirata
	popad
	iret

_isr3:
	pushad
		jmp matar_pirata
	popad
	iret

_isr4:
	pushad
		jmp matar_pirata
	popad
	iret

_isr5:
	pushad
		jmp matar_pirata
	popad
	iret

_isr6:
	pushad
		jmp matar_pirata
	popad
	iret

_isr7:
	pushad
		jmp matar_pirata
	popad
	iret

_isr8:
	pushad
		jmp matar_pirata
	popad
	iret

_isr10:
	pushad
		jmp matar_pirata
	popad
	iret

_isr11:
	pushad
		jmp matar_pirata
	popad
	iret

_isr12:
	pushad
		jmp matar_pirata
	popad
	iret

_isr13:
	pushad
		jmp matar_pirata
	popad
	iret

_isr14:
	pushad
		jmp matar_pirata
	popad
	iret

_isr16:
	pushad
		jmp matar_pirata
	popad
	iret

_isr17:
	pushad
		jmp matar_pirata
	popad
	iret

_isr18:
	pushad
		jmp matar_pirata
	popad
	iret

_isr19:
	pushad
		jmp matar_pirata
	popad
	iret

_isr20:
	pushad
		jmp matar_pirata
	popad
	iret
	
;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;

_isr32:
	pushad
		call fin_intr_pic1
		mov ax, [pantalla_debug_activa]
		cmp ax, 0x1
		je .fin
		
		call sched_tick

		xor ecx, ecx
		str cx	
		cmp ax, cx
		je .fin

		mov [sched_tarea_selector], ax
		jmp far [sched_tarea_offset]
		
		.fin:	
	popad
	iret

;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;
;Está parcialmente la implementacion de la activacion del debugger
_isr33:
	pushad
	
	call fin_intr_pic1
	in  al, 0x60
	cmp byte al, 0x15 ; codigo de la Y
 	je .rutina_debuger
 	cmp byte [pantalla_debug_activa], 0x1
 	je pop
	cmp byte al, 0x2A
	je .rutinals
	cmp byte al, 0x36
	je .rutinars
	jmp pop

_isr33.rutinals:
	xor  eax, eax
	push eax;jugadorA
	call game_atender_teclado
	add  esp, 4
	jmp  pop	

_isr33.rutinars:
	xor  eax, eax
	mov  eax, 1
	push eax;jugadorB
	call game_atender_teclado
	add  esp, 4
	jmp  pop

_isr33.rutina_debuger:
	mov ax, [debug_habilitado]
	cmp ax, 0x1
	je  _isr33.deshabilitar_debugger
	mov ax, 0x1
	mov [debug_habilitado], ax	
	jmp pop

_isr33.deshabilitar_debugger:
	mov ax,[pantalla_debug_activa]
	cmp ax, 0x1
	je  _isr33.desactivar_pantalla_debug
	jne _isr33.apagar_debugger

_isr33.desactivar_pantalla_debug:
	call game_reprintar_pantalla
	mov ax, 0x0
	mov [pantalla_debug_activa], ax
	mov [debug_habilitado], ax
	jmp pop

_isr33.apagar_debugger:
	mov ax, 0x0
	mov [debug_habilitado], ax
	jmp pop

pop:
	popad
	iret

;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;

_isr70:
	pushad
		push ecx
		push eax
		call game_syscall_manejar
		add  esp, 8

		mov [esp+28], eax

		mov ax, 0x70
		mov [sched_tarea_selector], ax ;idle
		jmp far [sched_tarea_offset]
	popad
	
	iret

;;
;; rutina de muerte de pirata
;; -------------------------------------------------------------------------- ;;

matar_pirata:
	mov ax, [debug_habilitado]
	cmp ax, 0x0
	je  .matar
	mov ax, 0x1
	mov [pantalla_debug_activa], ax
	popad
	pushad
	call game_debuggear_tarea

	jmp .matar

	.matar:
	call game_pirata_exploto
	jmp .fin

	.fin:
	str cx
	cmp cx, 0x70
	je .finb
	mov ax, 0x70
	mov [sched_tarea_selector], ax ;idle
	jmp far [sched_tarea_offset]
.finb:
popad
iret
