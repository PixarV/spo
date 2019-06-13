global _start

%include "macro.inc"

%define pc r15
%define w r14
%define rstack r13

section .text          

%include "choice.inc"
%include "interpreter.inc"

section .bss

; return stack end-----;
resq 1023              ;
rstack_start: resq 1   ;
; return stack start---;

input_buf: resb 1024   ; buffer to read textual words into
user_dict:  resq 65536 ; data for words


user_mem: resq 65536   ; global data for user

state: resq 1          ; changes to 1 if compiling, 0 by default

section .data 
last_word: dq _lw      ; stores a pointer to the last word in dictionary
here: dq user_dict     ; current position in words memory; 
dp: dq user_mem        ; current global data pointer 

section .rodata
msg_no_such_word: db ": no such word", 10, 0

section .text
next:                  ; inner interpreter, fetches next word to execute
    mov w, pc
    add pc, 8
    mov w, [w]
    jmp [w]

_start: 
    mov rstack, rstack_start
    mov [stack_start], rsp
    mov pc, forth_init

