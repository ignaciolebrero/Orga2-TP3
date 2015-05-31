%include "imprimir.mac"

global start

extern screen_pintar_rect
extern screen_inicializar
extern idt_inicializar
extern mmu_inicializar
extern mmu_inicializar_dir_kernel
extern inicializar_dir_pirata
extern GDT_DESC
extern IDT_DESC

;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg

iniciando_gr_msg db     'Grupo Crash Bash'
iniciando_gr_len equ    $ - iniciando_gr_msg 
;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 0

    ; Habilitar A20
    call habilitar_A20

    ; Cargar la GDT
    lgdt [GDT_DESC]

    ; Setear el bit PE del registro CR0
    MOV eax,cr0
    OR  eax,1
    MOV cr0,eax
    ; Saltar a modo protegido
    jmp 0x40:modoprotegido
    

modoprotegido:
BITS 32
    xor  eax, eax
    mov  ax, 0x50
    mov  ds, ax
    mov  es, ax
    mov  gs, ax
    mov  ss, ax
    mov  ax, 0x60
    mov  fs, ax

    ; Establecer la base de la pila
    mov ebp, 0x27000
    mov esp, ebp

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mp iniciando_mp_msg, iniciando_mp_len, 0x07, 2, 0

    ; Inicializar el juego

    ; Inicializar pantalla
    call screen_inicializar

    ; Inicializar el manejador de memoria
    ;call mmu_inicializar 

    ; Inicializar el directorio de paginas
    call mmu_inicializar_dir_kernel
    
    ; Cargar directorio de paginas

    ; Habilitar paginacion
    mov eax, 0x27000
    mov cr3, eax

    mov eax, cr0
    or eax, 0x800000 ;pagination on!
    mov cr0, eax
    xchg bx,bx

    imprimir_texto_mp iniciando_gr_msg, iniciando_gr_len, 0xE, 0, 64; SERIA HERMOSO, ENCONTRAR EL COLOR PARA QUE QUEDE NARANJA/ROJO EN EL FONDO Y ADELANTE AMARILLO, LO HARIA MAS REALISTA.

    ; Inicializar tss
    call mmu_inicializar
    ; Inicializar tss de la tarea Idle
    call inicializar_dir_pirata
    ; mover el cr3 actual por el elemento que devuelve la funcion, en teoria lo hace en la pila, no?
    ; Inicializar el scheduler

    ; Inicializar la IDT
    call idt_inicializar
    
    ; Cargar IDT
    lidt [IDT_DESC]

    ; Configurar controlador de interrupciones
    mov al, 0xff
    out 0xa1, al
    out 0x21, al
    
    ; Cargar tarea inicial

    ; Habilitar interrupciones

    ; Saltar a la primera tarea: Idle

    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
