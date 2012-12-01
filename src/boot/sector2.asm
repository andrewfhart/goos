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
; Name:	sector2.asm
; External: SECTOR2.BIN
; Author: Andrew F. Hart
; Date:	2006.03.15
;=======================================================================
; This bootloader performs the loading of the GOOS kernel into memory.
; It subsequently creates and loads the GDT, switches the machine into
; 32 bit protected mode (PMODE), and finally jumps to the kernel entry 
; stub (gkstart.asm).  
; For more information about bootloaders, consult the following very 
; helpful references:
; Faulkner, Daniel R. "Hello World Boot Loader"
; 	http://www.osdever.net/tutorials/hello_btldr.php?the_id=85
; Robinson, Timothy. "Writing a Kernel in C"
;	http://www.osdever.net/tutorials/ckernel.php?the_id=10
;	http://osdev.berlios.de/ckernel.html
; Vea, Matthew. "How to Program a Bootstrap Loader"
;	http://www.omninerd.com/2005/11/05/articles/40
; Weeks, Jeff. "Writing a Bootsector", 1997.
; 	http://gaztek.sourceforge.net/osdev/boot/polyos.html
;-----------------------------------------------------------------------
; HOW TO COMPILE:
;	nasmw -f bin SECTOR2.BIN sector2.asm
; HOW TO WRITE TO A BOOT DISK: (Read notes below first!!)
;	debug
;	n SECTOR2.BIN
;	l
;	w 100 0 1 1
;	q
; NOTES:
; 1) sector2.BIN needs to be placed in the second sector of a formatted
;    floppy disk
; 2) MS DOS Debug command can do that.
; 3) LEARN ABOUT MSDOS DEBUG COMMAND BEFORE USING IT!! IT IS SERIOUSLY
;    POWERFUL AND, ESPECIALLY WITH THE 'W' COMMAND USED HERE, CAN UTTERLY
;    RUIN YOUR DAY!
; 4) Verify that the compiled GOOSBOOT.BIN is not more than 512 bytes long!
;    (use FHEXDUMP utility for this)
;

[BITS 16]			; Generate 16 bit code
[ORG 0x8000]
main:
  
  mov [bootDrv], dl		; Save the boot drive for loading the kernel

; Print Hello Message
  mov si, msgSuccess		
  call print			

; Load the Kernel into Memory
  mov [lk_KernSize],   BYTE 0x20	; Kernel is 16KB = 32 512 Byte Sectors
  mov [lk_KernStart],  BYTE 0x04	; Kernel begins on the 4th Disk Sector
  mov [lk_tempOffset], WORD 0x9000	; Kernel begins at 0x9000 in segment 0x0
  call loadKernel			; (0x0000:0x9000)


  
; Load the Global Descriptor Table
  cli
  lgdt [gdtr]			; Point the gdtr to our GDT (below)
  sti		
  mov si, msgLGDT
  call pprint

 
 
; Switch to Protected Mode
  mov si, msgGOPM
  call pprint
  cli				; clear interrupts until IDT is created
  mov eax, cr0			; load control register 0
  or  al, 0x01			; set the LSB of cr0 (1=pmode, 0=real)
  mov cr0, eax			; install the new cr0
  jmp codesel:PMode_Entry	; Flush the pipeline with a Far Jump
  

[BITS 32]
PMode_Entry:
  mov ax, datasel		; Initialize ds and es to data seg
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov ss, ax
  mov ax, videosel		; Initialize gs to video memory
  mov gs, ax
; Jump to Kernel Code (/krnl/gk_start.asm)
  jmp codesel:0x9000

  
spin: jmp spin			; Infinite loop to prevent problems


; GDT SETUP
; ======================================================================
; This GDT is only useful until we have jumped to PMode. At that point
; Kernel code (/krnl/gdt.c) creates a new GTD and IDT for use by the 
; kernel.
[BITS 16]
gdtr:				; pointer to the gdt structure
 dw gdt_end-gdt-1		; the 16 bit size of the gdt
 dd gdt 			; the 32 bit linear address of the gdt
gdt
 nullsel equ $-gdt;		; nullsel = 0 (gdt-gdt = 0)
 dd 0x00000000, 0x00000000
 codesel equ $-gdt;		; codesel = 8h
 dd 0x0000FFFF, 0x00CF9C00
 datasel equ $-gdt;		; datasel = 10h
 dd 0x0000FFFF, 0x00CF9200
 videosel equ $-gdt;		; videosel= 18h
 dd 0x80003999, 0x0000920B
gdt_end
 

;
; GDT Breakdown gdt[0] (null descriptor)
;    0  0     0  0     0  0     0  0     0  0     0  0     0  0     0  0
; +--------+--------+--------+--------+--------+--------+--------+--------+
;  00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000
; +--------+--------+--------+--------+--------+--------+--------+--------+
; 63                47                31                15                0 
; GDT Breakdown gdt[1] (code descriptor)
;    0  0     C  F     9  C     0  0     0  0     0  0     F  F     F  F
; +--------+--------+--------+--------+--------+--------+--------+--------+
;  00000000 11001111 10011010 00000000 00000000 00000000 11111111 11111111
; +--------+--------+--------+--------+--------+--------+--------+--------+
; 63                47                31                15                0 
; GDT Breakdown gdt[2] (data descriptor)
;    0  0     C  F     9  2     0  0     0  0     0  0     F  F     F  F
; +--------+--------+--------+--------+--------+--------+--------+--------+
;  00000000 11001111 10010010 00000000 00000000 00000000 11111111 11111111
; +--------+--------+--------+--------+--------+--------+--------+--------+
; 63                47                31                15                0 
; GDT Breakdown gdt[3] (video descriptor)
;    0  0     0  0     9  2     0  B     8  0     0  0     3  9     9  9
; +--------+--------+--------+--------+--------+--------+--------+--------+
;  00000000 00000000 10010010 00001011 10000000 00000000 00111001 10011001
; +--------+--------+--------+--------+--------+--------+--------+--------+
; 63                47                31                15                0 

; BITS  DESCRIPTION
; 15-00 Bits 15-0 of the segment limit (FFFF = 4GB)  
; 31-16 Bits 15-0 of the base address  (0000 = 0h)
; 39-32 Bits 23-16 of the base address (00 = 0h)
; 40    Access Bit (for use with virtual memory) = 0
; 43-41 Descriptor Type
;         43: Executable (0=data segment, 1=code segment)
;         42-data: exapansion direction (0=expandup)
;         42-code: conforming
;         41-data: Read/Write: (0=RO, 1=RW)
;         41-code: Read/Execute: (0=Execute Only, 1=Read/Execute)
; 44    Descriptor Bit (0=system desc., 1=code/data desc)
; 46-45	Descriptor privilege level (0=highest, 3=lowest) (Ring0,Ring3)
; 47	Present Bit (segment present in memory (virtual mem stuff))
; 51-48 Bits 19-16 of segment limit
; 52    Reserved for the OS
; 53    Reserved, should be 0
; 54    16/32bit segment (0=16bit, 1=32bit)
; 55    Granularity (1=multiply limit by 4k)
; 63-56 Bits 31-24 of the base address (00 = 0h)


; Translate LBA to CHS
; ----------------------------------------------
; Perform a translation between logical byte address and 
; hardware cylinder/head/sector address
; The algorithm for this comes more or less directly from:
; Faulkner, Daniel. "LBA To CHS"
; 	http://www.osdever.net/tutorials/chs_lba.php?the_id=87
; 
; input: 	ax - LBA
; output:	ax - Sector
;		bx - Head
;		cx - Cylinder (Track)
;
LBACHS:
 push bx			; save original values in the stack
 push cx
 push dx			
   xor dx,dx			; zero dx
   xor cx,cx			; zero cx
   xor bx,bx			; zero bx
   mov bl, BYTE [lk_SPT]	; move SPT into place
   div bx			; Perform the divide ax/bx -> ax, dx
				; sector = (LBA % SPT)
   ;;inc dx			;; add one to the remainder
   push dx			; Save the value in dx on the stack
     xor dx,dx			; zero dx
     xor cx,cx			; zero cx
     xor bx,bx			; zero bx
     mov bl, BYTE [lk_NH]	; move NH into place
     div bx			; perform the divide ax/bx -> ax, dx
                                ; cylinder = (LBA / SPT)
				; head  = (LBA / SPT) % NH
     mov cx,ax			; Move ax to cx (Cylinder)
     mov bx,dx			; Move dx to bx (Head)
   pop ax			; Take value from stack (Sector)
 				; restore orig. dx 
   mov [lk_tempC], cx		; place cylinder value in temp storage
   mov [lk_tempH], bx		; place head value in temp storage
   mov [lk_tempS], ax		; place sector value in temp storage
 pop dx				; restore original values
 pop cx
 pop bx
ret

; Load Kernel From Disk
;------------------------------------------------
; di - read attempt counter (3 tries per sector)
; cx - sector counter (how many remain to be read)
; 
; kernel loaded to 0x0000:0x9000 (0x09000 linear)
;
loadKernel:
  mov si, msgLoadKern
  call pprint
  xor ax, ax			; clear ax,bx,and cx
  xor bx, bx
  xor cx, cx
  mov al, BYTE [lk_KernStart]	; load the kernel start sector into ax
  mov bx, WORD [lk_tempOffset]	; load the offset sector into bx
  mov cl, BYTE [lk_KernSize]	; load kernel sector size into counter
.reads
  push ax			; store the lba value of the sect. to read
    call LBACHS			; Translate sector into C/H/S
    push ax			; sector
    push bx			; head
    push cx			; cylinder
      mov ah, 0x02		; code for read disk sectors
      mov al, 0x01		; number of sectors to read
      mov ch, [lk_tempC]		; track/cylinder number
      mov cl, [lk_tempS]		; sector number
      mov dh, [lk_tempH]		; head number
      mov dl, [bootDrv]		; drive number
      mov bx, 0x0000		; destination segment (0x0000)
      mov es, bx
      mov bx, [lk_tempOffset]	; offset within dest segment (512 Bytes)
      mov di, 4			; Try to read 3 times (predecremented)
     .read
      dec di			; predecrement counter
      jz .fail
      int 0x13			; Diskette BIOS Services Interrupt
      jc .read			; on error try again
   .success			; If Successful, Queue Next Sector  
    pop cx
    pop bx
    pop ax
    mov si, msgProgressDot	; print a dot to show progress
    call print
  pop ax
  inc ax			; increment sector to load
  add bx, 0x0200		; increment offset by 512 Bytes and
  mov [lk_tempOffset], bx	; store new offset
  dec cx			; decrement number left to load
  jz .end			; If no sectors remain, go to end	
  jmp .reads			; Otherwise, go read the next sector
.fail
  pop cx
  pop bx
  pop ax
  mov si, msgFailed		; error reading sector
  call print
  call reboot
.end 
  mov si, msgSuccess
  call print
ret


; Reboot
;------------------------------------------------
; Informs the user of the impending reboot, waits for user to press
; any key (allow user to remove system disk from floppy), and then
; reboots the machine.
;
reboot:
  mov si, msgReboot	; Load up the reboot message
  call print		; Tell the user we're about to reboot
  call waitforkey	; Wait for the user to press a key
  db 0x0EA		; machine lang for jump to ffff:0000 (reboot)
  dw 0x0000		
  dw 0xFFFF
  ; ret not required since rebooting

; Wait For Keypress
;------------------------------------------------
; waits until the user presses a key. This makes use of the BIOS 
; interrupt 0x16 which halts the program until a key with a scancode
; is pressed. (NOTE: The scancode of the keypress is stored in ah on
; return and al contains the ascii character or 0 if the scancode is
; for a special function key. -- none of this is used here)
;
waitforkey:
  mov ah,0x00		; code for Wait for keypress and read char
  int 0x16		; Keyboard Bios Services Interrupt
ret 

; Print a message
;-----------------------------------------------
print:
  push ax
  push bx
    mov ah, 0x0E
    mov bx, 0x0007
  .nxt
    lodsb
    or al,al
    jz .end
    int 0x10
    jmp .nxt
  .end
  pop bx
  pop ax
ret

; Print with Prefix (msgPrefix)
;-----------------------------------------------
pprint:
  push ax
  push bx
    mov ah, 0x0E
    mov bx, 0x0007
    push si
    mov si, msgPrefix
    .nxt2 
      lodsb
      or al,al
      jz .endp
      int 0x10
      jmp .nxt2
      .endp
    pop si
  .nxt3
    lodsb
    or al,al
    jz .end
    int 0x10
    jmp .nxt3
  .end:
  pop bx
  pop ax
ret


; Data Section
; ================================================
bootDrv		db	0	; Boot drive (0x0=floppy,0x80=hd)
lk_KernSize	db	0	; Size of kernel in sectors
lk_KernStart	db	0	; First Sector (LBA) of kernel on disk
lk_tempC	db	0	; Cylinder
lk_tempH	db	0	; Head
lk_tempS	db	0	; Sector
lk_tempOffset	dw	0	; Offset into segment
lk_SPT		db	18	; Sectors Per Track
lk_NH		db	2	; Number of Disk Heads
msgPrefix  	db      '[2:> ',0
msgProgressDot	db	'.',0
msgLoadKern	db	'Loading Kernel',0
msgLGDT		db	'Installed Temporary GDT.',13,10,0
msgGOPM		db	'Going to Protected Mode...',0
msgReboot	db 	'Press any key to reboot now...',13,10,0

msgSuccess 	db 	'Success!',13,10,0
msgFailed  	db 	'Failed!',13,10,0



times 512-($-$$) db 0
