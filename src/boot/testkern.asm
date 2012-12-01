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
; Name:	sector2.asm
; External: SECTOR2.BIN
; Author: Andrew F. Hart
; Date:	2006.01.25
;=======================================================================
[BITS 32]			; Generate 16 bit code


  
  mov [gs:0x0], WORD 0x044B	; K
  mov [gs:0x2], WORD 0x0445	; E
  mov [gs:0x4], WORD 0x0452	; R
  mov [gs:0x6], WORD 0x044E	; N
  mov [gs:0x8], WORD 0x0445	; E
  mov [gs:0xA], WORD 0x044C	; L
  mov [gs:0xC], WORD 0x0420	;
  mov [gs:0xE], WORD 0x044E	; N
  mov [gs:0x10], WORD 0x044F	; O
  mov [gs:0x12], WORD 0x0454	; T
  mov [gs:0x14], WORD 0x0420	;
  mov [gs:0x16], WORD 0x0446	; F
  mov [gs:0x18], WORD 0x044F	; O
  mov [gs:0x1A], WORD 0x0455	; U
  mov [gs:0x1C], WORD 0x044E	; N
  mov [gs:0x1E], WORD 0x0444	; D
  mov [gs:0x20], WORD 0x0421	; !
  
  jmp $

  times 16384-($-$$) db 0	; pad to 16KB with 0's to flush out old krnl junk
