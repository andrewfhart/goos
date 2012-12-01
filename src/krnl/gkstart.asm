;        __  _________  _________  _________ /\
;     __/\_\/\  _____ \/\  _____ \/\   _____\  \
;    / /:/ /\:\ \___/\ \:\ \___/\ \:\  \___/_   \
;   / /:/ / _\:\ \  \:\ \:\ \  \:\ \:\_____  \   \
;  / /:/ / /\ \:\ \  \:\ \:\ \  \:\ \/___/:\  \   \
;  \ \:\ \//\\ \:\ \__\:\ \:\ \__\:\ \  _\_:\  \   \
;   \ \:\//_/\\_\:\________\:\________\/\_______\   \   
;    \ \:\  \//  \/________/\/________/\/_______/   /
;     \ \:\ //  / GOOS - GO Operating System       /
;      \ \://  /   c.2006 Crawwler Research       /
;       \_\/__/__________________________________/
;
; ======================================================================
; Name:	gkstart.asm
; External: gkstart.o
; Author: Andrew F. Hart
; Date:	2006.04.08
;=======================================================================
[BITS 32]
global gkstart
gkstart:
	mov esp, _sys_stack		; point the stack to new stack area
	jmp stublet

	EXTERN code,bss,end
	dd code
	dd bss
	dd end

stublet:
	mov [gs:0], WORD 0x054B;
	extern _main			; main is externally defined
	call _main			; call main (/krnl/main.c)
	mov [gs:2], WORD 0x054B;
	hlt				; main should never return

; Shutdown / Reboot code
global _reboot
_reboot:
  db 0x0EA		; machine lang for jump to ffff:0000 (reboot)
  dw 0x0000		
  dw 0xFFFF
  ; ret not required since rebooting


; GDT loading code
global _gdt_flush			; called by install_gdt (/krnl/core/gdt.c)
extern _pGDT				; _pGDT is defined in (/krnl/core/gdt.c)
_gdt_flush:
	lgdt [_pGDT]			; Load the GDT
	mov ax, 0x10			; 0x10 is offset in GDT to data segment
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov ss, ax
	mov ax, 0x18			; 0x18 is offset in GDT to vid mem segmt
	mov gs, ax
	jmp 0x08:flush			; 0x08 is offset in GDT to code segment
flush:
	ret				; returns to c code (/krnl/core/gdt.c)


; IDT loading code
global _idt_load			; called by idt_install (/krnl/core/idt.c)
extern _pIDT				; _pIDT is defined in (/krnl/core/idt.c)
_idt_load:
	lidt [_pIDT]			; Load the IDT
	ret

; ISR DEFINITIONS (for text definitions see (/krnl/core/isrs.c))
global _isr0
global _isr1
global _isr2
global _isr3
global _isr4
global _isr5
global _isr6
global _isr7
global _isr8
global _isr9
global _isr10
global _isr11
global _isr12
global _isr13
global _isr14
global _isr15
global _isr16
global _isr17
global _isr18
global _isr19
global _isr20
global _isr21
global _isr22
global _isr23
global _isr24
global _isr25
global _isr26
global _isr27
global _isr28
global _isr29
global _isr30
global _isr31

; IRQ DEFINITIONS (for text definitions see (/krnl/core/irqs.c))
global _irq0
global _irq1
global _irq2
global _irq3
global _irq4
global _irq5
global _irq6
global _irq7
global _irq8
global _irq9
global _irq10
global _irq11
global _irq12
global _irq13
global _irq14
global _irq15

; ISR0 (for definitions see (/krnl/core/isrs.c))
_isr0:
	cli
	push byte 0				; dummy error code to keep stack 
						; frame uniform
	push byte 0				; store exception number
	jmp isr_common_stub			; handle the exception

; ISR1 
_isr1:
	cli
	push byte 0
	push byte 1
	jmp isr_common_stub

; ISR2 
_isr2:
	cli
	push byte 0
	push byte 2
	jmp isr_common_stub

; ISR3 
_isr3:
	cli
	push byte 0
	push byte 3
	jmp isr_common_stub

; ISR4
_isr4:
	cli
	push byte 0
	push byte 4
	jmp isr_common_stub

; ISR5
_isr5:
	cli
	push byte 0
	push byte 5
	jmp isr_common_stub

; ISR6
_isr6:
	cli
	push byte 0
	push byte 6
	jmp isr_common_stub

; ISR7
_isr7:
	cli
	push byte 0
	push byte 7
	jmp isr_common_stub

; ISR8	
_isr8:
	cli
						; error code pushed auto.
	push byte 8
	jmp isr_common_stub

; ISR9
_isr9:
	cli
	push byte 0
	push byte 9
	jmp isr_common_stub

; ISR10
_isr10:
	cli

	push byte 10
	jmp isr_common_stub

; ISR11
_isr11:
	cli

	push byte 11
	jmp isr_common_stub

; ISR12
_isr12:
	cli

	push byte 12
	jmp isr_common_stub

; ISR13
_isr13:
	cli

	push byte 13
	jmp isr_common_stub

; ISR14
_isr14:
	cli

	push byte 14
	jmp isr_common_stub

; ISR15
_isr15:
	cli
	push byte 0
	push byte 15
	jmp isr_common_stub

; ISR16
_isr16:
	cli
	push byte 0
	push byte 16
	jmp isr_common_stub

; ISR17
_isr17:
	cli
	push byte 0
	push byte 17
	jmp isr_common_stub

; ISR18
_isr18:
	cli
	push byte 0
	push byte 18
	jmp isr_common_stub

; ISR19
_isr19:
	cli
	push byte 0
	push byte 19
	jmp isr_common_stub

; ISR20
_isr20:
	cli
	push byte 0
	push byte 20
	jmp isr_common_stub

; ISR21
_isr21:
	cli
	push byte 0
	push byte 21
	jmp isr_common_stub

; ISR22
_isr22:
	cli
	push byte 0
	push byte 22
	jmp isr_common_stub

; ISR23
_isr23:
	cli
	push byte 0
	push byte 23
	jmp isr_common_stub

; ISR24
_isr24:
	cli
	push byte 0
	push byte 24
	jmp isr_common_stub

; ISR25
_isr25:
	cli
	push byte 0
	push byte 25
	jmp isr_common_stub

; ISR26
_isr26:
	cli
	push byte 0
	push byte 26
	jmp isr_common_stub

; ISR27
_isr27:
	cli
	push byte 0
	push byte 27
	jmp isr_common_stub

; ISR28
_isr28:
	cli
	push byte 0
	push byte 28
	jmp isr_common_stub

; ISR29
_isr29:
	cli
	push byte 0
	push byte 29
	jmp isr_common_stub

; ISR30
_isr30:
	cli
	push byte 0
	push byte 30
	jmp isr_common_stub

; ISR31
_isr31:
	cli
	push byte 0
	push byte 31
	jmp isr_common_stub

; 32: IRQ0
_irq0:
	cli
	push byte 0	; dummy error code
	push byte 32	; id
	jmp irq_common_stub
; 33: IRQ1
_irq1:
	cli
	push byte 0	; dummy error code
	push byte 33	; id
	jmp irq_common_stub
; 34: IRQ2
_irq2:
	cli
	push byte 0	; dummy error code
	push byte 34	; id
	jmp irq_common_stub
; 35: IRQ3
_irq3:
	cli
	push byte 0	; dummy error code
	push byte 35	; id
	jmp irq_common_stub
; 36: IRQ4
_irq4:
	cli
	push byte 0	; dummy error code
	push byte 36	; id
	jmp irq_common_stub
; 37: IRQ5
_irq5:
	cli
	push byte 0	; dummy error code
	push byte 37	; id
	jmp irq_common_stub
; 38: IRQ6
_irq6:
	cli
	push byte 0	; dummy error code
	push byte 38	; id
	jmp irq_common_stub
; 39: IRQ7
_irq7:
	cli
	push byte 0	; dummy error code
	push byte 39	; id
	jmp irq_common_stub
; 40: IRQ8
_irq8:
	cli
	push byte 0	; dummy error code
	push byte 40	; id
	jmp irq_common_stub
; 41: IRQ9
_irq9:
	cli
	push byte 0	; dummy error code
	push byte 41	; id
	jmp irq_common_stub
; 42: IRQ10
_irq10:
	cli
	push byte 0	; dummy error code
	push byte 42	; id
	jmp irq_common_stub
; 43: IRQ11
_irq11:
	cli
	push byte 0	; dummy error code
	push byte 43	; id
	jmp irq_common_stub
; 44: IRQ12
_irq12:
	cli
	push byte 0	; dummy error code
	push byte 44	; id
	jmp irq_common_stub
; 45: IRQ13
_irq13:
	cli
	push byte 0	; dummy error code
	push byte 45	; id
	jmp irq_common_stub
; 46: IRQ14
_irq14:
	cli
	push byte 0	; dummy error code
	push byte 46	; id
	jmp irq_common_stub
; 47: IRQ15
_irq15:
	cli
	push byte 0	; dummy error code
	push byte 47	; id
	jmp irq_common_stub


extern _isr_fault_handler
isr_common_stub:
	pusha		; saves edi esi ebp ebx edx ecx eax
	push ds		; save segments
	push es
	push fs
	push gs
	mov ax, 0x10	; Load the kernel Data Segment Descriptor
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp	; Load the stack pointer
	push eax	; save the stack pointer
	mov eax, _isr_fault_handler
	call eax	
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8	; removes the pushed error code and pushed
			; ISR number
	iret		; pops CS, EIP, EFLAGS, SS, and ESP 

extern _irq_handler
irq_common_stub:
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	mov eax,esp
	push eax
	mov eax, _irq_handler
	call eax
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp,8
	iret	

extern _print_cpu
global _dump_cpu
_dump_cpu:
	pusha		; pushes edi esi ebp esp ebx edx ecx eax
	push ds		; save segments
	push es
	push fs
	push gs
	mov eax, esp	; save the stack pointer
	push eax	; push the stack pointer
	mov eax, _print_cpu
	call eax	; eip,cs,useresp,ss pushed/popped automatically
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	ret
 	

;-----------------------------------------------------------------------
; Reading and Writing to Control Registers
;
;
global _read_cr0
_read_cr0:
	mov eax, cr0
	ret

global _read_cr1
_read_cr1:
	mov eax, cr1
	ret

global _read_cr2
_read_cr2:
	mov eax, cr2
	ret

global _read_cr3
_read_cr3:
	mov eax, cr3
	ret

global _write_cr0
_write_cr0:
	push ebp
	  mov ebp, esp
	  mov eax, [ebp+8]
	  mov cr0, eax
	pop ebp
	ret
global _write_cr1
_write_cr1:
	push ebp
	  mov ebp, esp
	  mov eax, [ebp+8]
	  mov cr1, eax
	pop ebp
	ret
global _write_cr2
_write_cr2:
	push ebp
	  mov ebp, esp
	  mov eax, [ebp+8]
	  mov cr2, eax
	pop ebp
	ret
global _write_cr3
_write_cr3:
	push ebp
	  mov ebp, esp
	  mov eax, [ebp+8]
	  mov cr3, eax
	pop ebp
	ret
;-----------------------------------------------------------------------
; BSS Section. Used to store the system stack. Since
; a stack grows downwards, the size of the
; stack data is declared before declaring the identifier itself
;-----------------------------------------------------------------------
SECTION .bss
	resb 8192;			; reserves 8KBytes of memory here
_sys_stack:

