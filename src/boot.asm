[BITS 16]
[ORG 0x7C00]

%assign SECTORS (N + 511) / 512

cli
xor ax, ax
mov ss, ax
mov sp, 0x7C00
mov ds, ax
sti

mov si, SECTORS

 xor ax, ax
mov es, ax
mov bx, 0x7E00

mov ch, 0 ; cylinder
mov dh, 0 ; head
mov cl, 1 ; sector

.loop:
  cmp cl, 18
  jge .change_head ; main part of cycle
  inc cl

  mov ah, 0x2
  mov al, 1
  mov bx, 0x7E00

  int 0x13
  jc .disk_error
  ; offset
  mov ax, es
  add ax, 0x0020 
  mov es, ax


  dec si
  jz loop
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


.disk_error:
  mov ah, 0x0E
  mov al, 'E'
  int 0x10
  jmp .disk_error

loop:
  jmp loop

times 510-($-$$) db 0
dw 0xAA55
