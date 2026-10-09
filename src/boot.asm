[BITS 16]

%assign SECTORS (N + 511) / 512

cli
xor ax, ax ; ax = 0
mov ss, ax ; stack segment = 0
mov ds, ax
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
  jz setup_pm
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


setup_pm:
  lgdt [gdt_descriptor]
  cld

  mov eax, cr0
  or eax, 0x1
  mov cr0, eax

  jmp 0x8:next


[BITS 32]
next:

mov ax, 0x10
mov ds, ax
mov ss, ax
mov es, ax
mov fs, ax
mov gs, ax

[EXTERN kernel_entry]
call kernel_entry


[GLOBAL infloop]
infloop:
  jmp infloop

align 8

gdt_start:
null_descriptor:
  dq 0x0

code_descriptor:
  dw 0xFFFF
  dw 0x0
  db 0x0
  db 10011010b
  db 11001111b
  db 0x0

data_descriptor:
  dw 0xFFFF
  dw 0x0
  db 0x0
  db 10010010b
  db 11001111b
  db 0x0

gdt_end:
gdt_descriptor:
  dw gdt_end - gdt_start - 1
  dd gdt_start

times 510-($-$$) db 0
dw 0xAA55
