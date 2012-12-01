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
; kernel. It sets up a temporary stack, verifies the processor is at least
; a 386 or greater, enables the A20 line for extended memory access and
; finally loads the second stage bootloader into memory.Control jumps 
; to the second stage after it has been loaded. For more information about 
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
;	nasmw -f bin GOOSBOOT.BIN goosboot.asm
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
;    POWERFUL AND, ESPECIALLY WITH THE 'W' COMMAND USED HERE, CAN UTTERLY
;    RUIN YOUR DAY!
; 4) Verify that the compiled GOOSBOOT.BIN is exactly 512 bytes long
;    and that it ends in 55 AA. (use FHEXDUMP utility for this)
;
;------------------------------------------------------------------------
; 2006.06.12 - added resetfloppy function

[BITS 16]
[ORG 0x7C00]				

mov [bootDrv], dl		; store the id of the bootdrive 0x00=floppy,0x80=hd
mov ax, 0x0000			; set up the data segment register
mov ds, ax
	
	

; Print Hello Message
  mov si, msgStart
  call print

; Set up Stack
  cli				; disable interrupts
  mov ax, 0x1D00		; the stack will be at  0x1D00:0200
  mov ss, ax			; set the stack pointer
  mov sp, 0x200			; 512 (200h) Byte Stack
  sti				; enable interrupts

; Verify 386 or higher Processor
  call verify386

; Enable A20 Line to access memory above 1MB
  call enableA20

; Load the Second Stage of the Bootloader
  mov si, msgLoad2nd
  call pprint
  call loadStage2
  mov dl, [bootDrv]		; Save the boot drive for stage 2

  jmp 0x0000:0x8000		; jump to stage 2
  


 

; loop indefinitely
 
 jmp $				; infinite loop

; Print a message
;-----------------------------------------------
print:
  mov ah, 0x0E
  mov bx, 0x0007
.nxt
  lodsb
  or al,al
  jz .end
  int 0x10
  jmp .nxt
.end
ret

; Print with Prefix (msgPrefix)
;-----------------------------------------------
pprint:
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
ret

; Verify 386
;------------------------------------------------
; Determine what kind of hardware we are running on: 8088/8086, 286,
; or 386+ (which is the only kind that we'll allow).
; The algorithm for this comes more or less directly from:
; Weeks, Jeff. "Writing a Bootsector", 1997
; 	http://gaztek.sourceforge.net/osdev/boot/polyos.html
; 
verify386:
  mov si, msg386		 
  call pprint		 

  ;Test if 8088/8086 (flag bits 12-15 set)
  pushf			; save original value of the flags
    xor ah,ah		; zero ah
  popf			; With bits 12-15 clear

  pushf			; read flags back into ax
  pop ax		
  and ah,0x0F0		; check if bits 12-15 are set
  cmp ah,0x0F0
  je .no386		; no 386 detected (found 8088/8086)

  ;Test if 286 (flag bits 12-15 clear)
  mov ah,0x0F0		; set bits 12-15
  push ax		; copy ax onto the flags
  popf

  pushf			; copy the flags into ax
  pop ax		
  and ah, 0F0h		; check if bits 12-15 are clear
  jz .no386		; no 386 detected (found 80286)

  mov si, msgSuccess	; processor is a 386 or higher
  call print

  ret

  .no386
  mov si, msgFailed	; processor does not meet kernel's needs
  call print
  call reboot

; Enable A20 Line
;------------------------------------------------
enableA20:
  mov si, msgA20
  call pprint
  cli
  
  call kbwaitc		; wait for keyboard controller to clear (bit 1 unset = port open)
  mov al, 0xD1		; code for write to output port
  out 0x64,al		; 0x64 is keyboard controller port
  
  call kbwaitc
  mov al, 0xDF		; code for enable A20
  out 0x60,al		; 0x60 is port to write data to
  
  call kbwaitc
kbwait:
  or  ax, ax		; filler (anything to waste time)
  out 0xE0, ax
  loop kbwait		; loop to waste time
  
  mov al, 0xD0		; code for read output port
  out 0x64, al
  call kbwaitf
  in al, 0x60		; read data from controller
  test al, 2		; test A20 gate bit
  jnz .a20_on
  mov si, msgFailed	; A20 not enabled
  call print
  call reboot
.a20_on:
  mov si, msgSuccess	; A20 enabled
  call print  
ret

; Wait for keyboard to be full
;------------------------------------------------
kbwaitf:
  xor cx, cx
  in  al, 0x64		; get keyboard status
  test al, 1		; if bit zero [00000001] is clear
  jz kbwaitf		;    wait some more
ret

; Wait for keyboard to clear
;------------------------------------------------
kbwaitc:
  xor al,al
  in al, 0x64		; get keyboard status
  test al, 2		; if bit one [00000010] is set,
  jnz kbwaitc		;   wait some more
ret

; Load Stage Two Boot Code
;------------------------------------------------
loadStage2:
  call resetfloppy	;
  mov ah, 0x02		; code for read disk sectors
  mov al, 0x01		; number of sectors to read
  mov ch, 0x00		; track/cylinder number
  mov cl, 0x02		; sector number
  mov dh, 0x00		; head number
  mov dl, [bootDrv]	; drive number
  mov bx, 0x0000	; destination segment (0x0000)
  mov es, bx
  mov bx, 0x8000	; offset within dest segment (32768 Bytes)
  mov di, 4		; Try to read 3 times (predecremented)
.read
  dec di		; predecrement counter
  jz .fail
  int 0x13		; Diskette BIOS Services Interrupt
  jc .read		; on error try again
  jmp .end		; sector read successfully
.fail
  mov si, msgFailed	; error reading sector
  call resetfloppy	;
  call print
  call reboot
.end
  call resetfloppy	;
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


; Reset Floppy Drive
;--------------------------------------------------
; Resets the floppy drive
resetfloppy:
  mov ax, 0		; code for reset drive
  mov dl, [bootDrv]	; which drive
  int 13
  jc resetfloppy
ret


; DATA SECTION
;--------------------------------------------------  
 bootDrv    db 0
 msgPrefix  db '[:> ',0
 msgStart   db 'GOOS Bootloader [ver 0.3] Started...',13,10,0
 msgReboot  db 'Press any key to reboot now...',13,10,0
 msg386     db 'Verifying Processor is 386 or Higher...',0
 msgA20     db 'Enabling A20 Gate...',0
 msgLoad2nd db 'Loading 2nd Stage Bootloader...',0

 msgSuccess db 'Success!',13,10,0
 msgFailed  db 'Failed!',13,10,0

 times 510-($-$$) db 0		; pad with zeroes
 dw 0xAA55			; end with boot signature
