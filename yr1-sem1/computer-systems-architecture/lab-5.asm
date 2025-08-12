bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit 
extern printf              ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll  
import printf msvcrt.dll  ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    ; ...
    a db 2, 1, 3, 3, 4, 2, 6
    la equ $ - a
    b db 4, 5, 7, 6, 2, 1
    lb equ $ - b
    r resb la + lb

; our code starts here
segment code use32 class=code
    start:
        ; ...
        mov esi, b
        mov ecx, lb
        mov edi, r
        std
        firstloop:
        lodsb ; al = [esi], esi++
        stosb ; [edi] = al
        loop firstloop
        
        mov esi, a
        mov ecx, la
        mov edi, r + lb
        cld
        secondloop:
        lodsb
        test al, 1
        jnz odd
        back:
        loop secondloop
        jmp done
        
        
        
        odd:
        stosb
        jmp back
        done:
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
