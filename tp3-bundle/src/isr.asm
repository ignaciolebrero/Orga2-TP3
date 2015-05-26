; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

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
cero_mr_msg 		dw 'diviste por 0 guachin',0
cero_mr_len 		equ $ - cero_mr_msg

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

	push cero_mr_msg
	push 0
	push 0
	push 0x60
	call print 
	xchg bx,bx
	popad
	iret

_isr1:
	pushad
	popad
	iret

_isr2:
	pushad
	popad
	iret

_isr3:
	pushad
	popad
	iret

_isr4:
	pushad
	popad
	iret

_isr5:
	pushad
	popad
	iret

_isr6:
	pushad
	popad
	iret

_isr7:
	pushad
	popad
	iret

_isr8:
	pushad
	popad
	iret

_isr10:
	pushad
	popad
	iret

_isr11:
	pushad
	popad
	iret

_isr12:
	pushad
	popad
	iret

_isr13:
	pushad
	popad
	iret

_isr14:
	pushad
	popad
	iret

_isr16:
	pushad
	popad
	iret

_isr17:
	pushad
	popad
	iret

_isr18:
	pushad
	popad
	iret

_isr19:
	pushad
	popad
	iret

_isr20:
	pushad
	popad
	iret


;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;

;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;

;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;


