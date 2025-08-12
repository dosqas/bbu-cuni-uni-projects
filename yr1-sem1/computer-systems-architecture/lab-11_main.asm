bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit, printf
import printf msvcrt.dll            ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions
                          
%include "lab11_subprogram.asm"

section .data
    bin db '10100111b', '01100011b', '110b', '101011b'
    len equ $-bin
    format db "%d ", 0

section .text
    global start
    start:
        mov ecx, len
        mov esi, bin
    next:
        call loop_func
        push ebx
        push format
        call [printf]
        add esp, 4*2
        dec ecx
        jnz next
    done:
    
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
