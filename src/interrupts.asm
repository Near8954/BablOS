[BITS 32]


%macro ISR_NOERR 1
    global isr%1

isr%1:
    push dword 0       ; фиктивный error_code
    push dword %1      ; номер вектора
    jmp save_context
%endmacro

%macro ISR_ERR 1
    global isr%1

isr%1:
    push dword %1      ; номер вектора
    jmp save_context
%endmacro


%assign i 0

%rep 256

    %if i = 8 || i = 10 || i = 11 || i = 12 || i = 13 || i = 14 || i = 17
        ISR_ERR i
    %else
        ISR_NOERR i
    %endif

    %assign i i + 1

%endrep

[GLOBAL isr_stub_table]
isr_stub_table:
%assign i 0
%rep 256
    dd isr %+ i
    %assign i i + 1
%endrep

[global idt_load]
idt_load:
  mov eax, [esp + 4]
  lidt [eax]
  ret

[EXTERN universal_handler]
[GLOBAL save_context]
save_context:
  push ds
  push es
  push fs
  push gs
  pusha

  cld

  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax

  mov ebx, esp

  and esp, -16
  sub esp, 12
  push ebx

  call universal_handler

[GLOBAL exp1]
exp1:
  mov eax, 1
  xor edx, edx
  mov ebx, 0
  div ebx
  ret

