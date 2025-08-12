bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit
extern printf               ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll
import printf msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    ; ...
    a db 3
    b db 5
    c db 2
    d dw 4
    rez dd 0
    mesaj db "Rezultatul la (a*2)+2*(b-3)-d-2*c este: %d"

; our code starts here
segment code use32 class=code
    start:
        ; ...
        mov al, byte [a] ; al = a
        mov bl, 2 ; bl = 2
        mul bl ; ax = a*2
        mov dx, ax ; dx = a*2
        mov al, byte [b] ; al = b
        sub al, 3 ; al = b-3
        mul bl ; ax = 2*(b-3)
        add dx, ax ; dx = (a*2)+2*(b-3)
        sub dx, word [d] ; dx = (a*2)+2*(b-3)-d
        mov al, byte [c] ; al = c
        mul bl ; ax = 2*c
        sub dx, ax ; dx = (a*2)+2*(b-3)-d-2*c
        mov [rez], dx ; rez = (a*2)+2*(b-3)-d-2*c
        
        
        push dword [rez]
        push mesaj
        call [printf]
    
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
