;        __  _________  _________  _________ /\
;     __/\_\/\  _____ \/\  _____ \/\   _____\  \
;    / /:/ /\:\ \___/\ \:\ \___/\ \:\  \___/_   \
;   / /:/ / _\:\ \  \:\ \:\ \  \:\ \:\_____  \   \
;  / /:/ / /\ \:\ \  \:\ \:\ \  \:\ \/___/:\  \   \
;  \ \:\ \//\\ \:\ \__\:\ \:\ \__\:\ \  _\_:\  \   \
;   \ \:\//_/\\_\:\________\:\________\/\_______\   \   
;    \ \:\  \//  \/________/\/________/\/_______/   /
;     \ \:\ //  / GOOS - GO Operating System       /
;      \ \://  /   c.2006 Andrew F. Hart          /
;       \_\/__/__________________________________/
;
; ======================================================================
; Name:	goosboot.asm
; External: GOOSBOOT.BIN
; Author: Andrew F. Hart
; Date:	2006.01.25
;=======================================================================
; This bootloader prepares the way for the inital loading of the GOOS
; kernel. What happens here should be fairly evident from the large
; number of comments I've included. For more information about 
; bootloaders, consult the following very helpful references:
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
;	nasmw -f bin GOOSBOOT.BIN GOloader2.asm
; HOW TO WRITE TO A BOOT DISK: (Read notes below first!!)
;	debug
;	n GOOSBOOT.BIN
;	l
;	w 100 0 0 1
;	q
; NOTES:
; 1) GOOSBOOT.BIN needs to be placed in the first sector of a formatted
;    floppy disk
; 2) MS DOS Debug command can do that.
; 3) LEARN ABOUT MSDOS DEBUG COMMAND BEFORE USING IT!! IT IS SERIOUSLY
;    POWERFUL AND, ESPECIALLY WITH THE 'W' COMMAND WE USE, CAN UTTERLY
;    RUIN YOUR DAY!
; 4) Verify that your compiled GOOSBOOT.BIN is exactly 512 bytes long
;    and that it ends in 55 AA. (You can use FHEXDUMP utility for this)
;

[BITS 16]			; Generate 16 Bit Code
[ORG 0x7C00]           		; Load the code into memory at 7C00
; ======================================================================
; BOOTLOADER MAIN SECTION
; ======================================================================
;Main Program
main:				; Main Program label

 mov ax,0x0000			; Set up the Data Segment register
 mov ds,ax			; Location of data is DS:Offset

;call clrscr			; clear the screen
 mov si, msgBoot		; Load up the startup message
 call message	           	; Introduce ourselves


; FLOW
; ==================================================================
; Ensure 386 or higher CPU
; Determine basic memory capabilities
; Set up a temp stack for translating between lba & chs
; Load Sector2 which will do the following:
;   Load the kernel off the floppy to >= 1MB..
;   Set up a temp GDT for the transition to protected mode
;   Make the jump to protected mode
;   point to the kernel and transfer control to start.asm which will
;      set up a proper system stack
;      set up a proper GDT
;      set up the IDT
;      call main
;
; ===================================================================

 call detect_cpu		; Detect what hardware we're running on

;Set up a Stack for use in LBA <-> CHS Translation
 cli				; Turn off interrupts
 mov ax, 0x9000			; Standard spot for the stack
 mov ss, ax			;
 mov sp, 0xFFFF			; Use the entire segment for the stack
 sti				; Restore interrupts


 ; Load the Kernel (with 2nd stage bootloader)
 ;call ReadSector		; Read the 2nd Sector off the disk and exe it 

 ;call reboot;			; Reboot the machine


;====================================================
;==*** REMOVE ME (to sector2.asm) ***================
;====================================================
  cli
  lgdt [gdtr]			; Point the gdtr to our GDT (below)
  

  ;mov si, msgGOPM		; Load up the "Going to PMode" Message
  ;call message


  mov eax, cr0			; Load Control Register 0 (for pmode switch)
  or al,0x01			; set the LSB of cr0 (pmode=1,real=0)
  mov cr0, eax			; Install our new CR0
  jmp codesel:go2_pm		; "Far Jump" to clear pipeline

bits 32
go2_pm:
  mov ax, datasel		; Initialize ds and es to data seg
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov ss, ax
  mov ax, videosel		; Initialize gs to video memory
  mov gs, ax
  mov word [gs:0],0x641		; Display a white A in protected mode

  jmp $

; GDT SETUP
; ======================================================================
; This GDT is only useful until we have jumped to PMode. At that point
; Kernel code (/krnl/gdt.c) creates a new GTD and IDT for use by the 
; kernel.
bits 16
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
;===================================================
;==*** END REMOVE ME ***============================
;===================================================

 

 ; Switch to PMODE (gdt setup, etc)



 ; Jump to Stage 2




	
;----------------------------------------------------------------------
;(Should never be run)
;; jmp $			; Put it into a continuous loop to stop it from
				; running off into the memory and executing any
				; leftover junk it might find there

; ====================================================================
; PROCEDURES SECTION
; ====================================================================
; This section contains all of the procedures we will need to make 
; the main program short and readable and to reduce the overall 
; code size to fit within our 512 byte (510 actually, considering the
; boot signature eats up 2 bytes) constraints.
;
; Proc: clrscr
; -------------------------------
; clears the screen
clrscr :
mov ax,0x0600 	; Fn 06 of int 10h,scroll window up,if al = 0 clrscr
mov cx,0x0000 	; Clear window from 0,0
mov dx,0x174F 	; to 23,79 174f
mov bh,0 	; fill with colour 0
int 0x10 	; call bios interrupt 10h
ret

; Proc: message
; -------------------------------
; prints one of our string constants to the screen. This procedure
; expects that the address of the string to print will be located
; at ds:si. loadsb loads the contents of the address pointed to by 
; ds:si into al and increments si by one. In this manner, 
; characters are outputted until a 0 is encountered.
message:
; Register Setup for Interrupt call
mov ah, 0x0E		; 0E = Write text in teletype mode
mov bh, 0x00		; Video page number
mov bl, 0x01		; Normal text attributes (white on blck)

.nextchar		; label for multi-char msgs
 lodsb			; Load byte at ds:si into al
 or al,al		; test if the character was 0 (end)
 jz .done
 int 0x10		; Call the BIOS video interrupt
 jmp .nextchar		; Go back for the next character
.done			; Internal label for when we're done
 ret			; Return to caller


;message:
;	mov ah, 0x0E		; put character
;	mov bh, 0x00		; video page number
;        mov bl, 0x07		; white on black
;	jmp short message_2
;message_1:
;	int 10h			
;message_2:
;	loadsb			; load byte at ds:si into al
;	or al,al		; test if character is 0 (end)
;	jne message_1
;	ret


; Proc: waitforkey
; -------------------------------
; waits until the user presses a key. This makes use of the BIOS 
; interrupt 0x16 which halts the program until a key with a scancode
; is pressed. (NOTE: The scancode of the keypress is stored in ah on
; return and al contains the ascii character or 0 if the scancode is
; for a special function key. -- none of this is used here)
waitforkey:
 mov ah,0x00		; 00 = Wait for keypress and read char
 int 0x16		; Call the BIOS
 ret
		
; Proc: reboot
; -------------------------------
; Informs the user of the impending reboot, waits for user to press
; any key (allow user to remove system disk from floppy), and then
; reboots the machine.
reboot:
 mov si, msgReboot	; Load up the reboot message
 call message		; Tell the user we're about to reboot
 call waitforkey	; Wait for the user to press a key
 db 0x0EA		; machine lang for jump to ffff:0000 (reboot)
 dw 0x0000		
 dw 0xFFFF
 ; ret not required since rebooting
				

; Proc: detect_cpu
; -------------------------------
; Determine what kind of hardware we are running on: 8088/8086, 286,
; or 386+ (which is the only kind that we'll allow).
; The algorithm for this comes more or less directly from:
; Weeks, Jeff. "Writing a Bootsector", 1997
; 	http://gaztek.sourceforge.net/osdev/boot/polyos.html
; ------------------------------
detect_cpu:
 mov si, msgProc	; Load up the checking processor message
 call message		; Tell the user we're checking for a compatible proc

 ;Test if 8088/8086 (flag bits 12-15 set)
 pushf			; Save original value of the flags
   xor ah,ah		; ah = 0
 popf			; With bits 12-15 clear

 pushf			; Read flags back into ax
 pop ax		
 and ah,0x0F0		; Check if bits 12-15 are set
 cmp ah,0x0F0
 je .no386		; No 386 detected (found 8088/8086)

 ;Test if 286 (flag bits 12-15 clear)
 mov ah,0x0F0		; Set bits 12-15
 push ax		; Copy ax onto the flags
 popf

 pushf			; Copy the flags into ax
 pop ax		
 and ah, 0F0h		; Check if bits 12-15 are clear
 jz .no386		; No 386 detected (found 80286)

 mov si, msgFound386	; Load up the '386 processor found' message
 call message		; Tell the user their processor is good!

 ret

 .no386
 mov si, msgNeed386	; Load up the 'need 386 processor' message
 call message
 call reboot
 ;ret not necessary since rebooting
 

; Proc: LBACHS
; -------------------------------
; Perform a translation between logical byte address and 
; hardware cylinder/head/sector address
; The algorithm for this comes more or less directly from:
; Faulkner, Daniel. "LBA To CHS"
; 	http://www.osdever.net/tutorials/chs_lba.php?the_id=87
; ------------------------------
; input: 	ax - LBA
; output:	ax - Sector
;		bx - Head
;		cx - Cylinder (Track)
LBACHS:
 push dx			; Save the value in dx in the stack
   xor dx,dx			; zero dx
   mov bx,[SectorsPerTrack]	; move SPT into place
   div bx			; Perform the divide ax/bx -> ax, dx
   inc dx			; add one to the remainder
   push dx			; Save the value in dx on the stack
     xor dx,dx			; zero dx
     mov bx,[NumHeads]		; move NH into place
     div bx			; perform the divide ax/bx -> ax, dx
     
     mov cx,ax			; Move ax to cx (Cylinder)
     mov bx,dx			; Move bx to dx (Head)
   pop ax			; Take value from stack (Sector)
 pop dx				; restore orig. dx 
 ret

ReadSector:
 mov ah, 0x02			; Read Disk Sectors
 mov al, 0x01			; Read 1 Sector (512b)
 mov ch, 0x00			;  Track 0
 mov cl, 0x02			;  Sector 2
 mov dh, 0x00			;  Head 0
 mov dl, [bootdrv]		;  Drive
 mov bx, 0x7c00			;  Segment 0x2000
 mov es, bx			;
 mov bx, 0x0200			;  Start of segment (0 0ffset)
.readsector
 int 0x13			; Make the BIOS call
 jc .readsector			; If there was an error, try again
  
;mov ax, 0x2000			; Set the data segment register
;mov ds, ax			; to point to the loaded location in memory
 jmp 0x7C00:0x0200		; Jump to the code
 





; ====================================================================
; DATA SECTION
; ====================================================================
; This section contains all of the data (variables and string constants)
; that the bootloader needs to operate. Variables are declared by
; providing a variable name, a variable size (db = declare byte (8bit), 
; dw =; declare word (16bit), and then the initial value or ? for 
; indeterminate. Strings should be declared as seen below with the
; content of the string followed by 13 (ascii cr), 10, (ascii lf), and
; 0 (ascii nul).
;
bootdrv		db 	0
SectorsPerTrack	db	18
NumHeads	db	2
msgBoot		db 	'GO Bootloader 0.2 by crawwl0r',13,10,0
msgReboot	db 	'Press any key to reboot now...',13,10,0

; used for identification of the processor
msgProc		db 	'Searching for 386+ processor: ',0
msgNeed386	db	'Sorry...  386 or higher required!',13,10,0
msgFound386	db 	'Success!',13,10,0


; ====================================================================
; END OF FILE PADDING AND BOOT SIGNATURE
; ====================================================================
; The bootsector must be exactly 512 bytes long and must end with
; bytes 510 and 511 (0 indexed) being 55 and AA respectively. As 
; a result, we pad the unused portion of our 512 bytes (determined
; by taking the difference between 510 and the amount of space our
; program and data have taken up) with zeros. Finally, we write the
; boot signature (55AA) into the final two bytes. (NOTE: dw 0xAA55
; will produce the desired result, most likely owing to the endianness
; of the intel x86 instruction set architecture.)
;
times 510-($-$$)  db 0		; Fill the rest of the sector with zeros
dw 0xAA55			; Boot Signature (0101010110101010)
