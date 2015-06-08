global get_ptl
global get_cr3
global get_eip
global get_eflags

get_ptl:
	;ver
	ret

get_cr3:
	mov ax, cr3
	ret

get_eip:
	mov ax, eip
	ret

get_eflags:
	mov ax, eflags
	ret
