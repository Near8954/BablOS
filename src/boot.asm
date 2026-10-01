[BITS 16]

%assign SECTORS (N + 511) / 512

cli
xor ax, ax ; ax = 0
mov ss, ax ; stack segment = 0
mov sp, 0x7C00 ; stack pointer

mov si, SECTORS
; [es:bx] = [0:0x7e00] - destination
mov es, ax
mov bx, 0x7E00

mov ch, 0 ; cylinder [0; 79]
mov dh, 0 ; head [0; 1]
mov cl, 1 ; sector [0; 17]

; order: s h c

.loop:
  cmp cl, 18 ; sectors [0; 17]
  jge .change_head ; main part of cycle
  inc cl

  mov ah, 0x2
  mov al, 1

  int 0x13
  jc .disk_error ; check carry flag

  ; offset
  mov ax, es
  add ax, 0x0020 ; 512 byte
  mov es, ax ; move base


  dec si
  jz infloop
  jmp .loop

.change_cylinder:
  inc ch
  mov dh, 0
  mov cl, 0
  jmp .loop


.change_head:
  cmp dh, 1
  jge .change_cylinder
  inc dh
  mov cl, 0
  jmp .loop

; print on screen E symbol
.disk_error:
  mov ah, 0x0E
  mov al, 'E'
  int 0x10
  jmp .disk_error

infloop:
  jmp infloop

times 510-($-$$) db 0
dw 0xAA55
