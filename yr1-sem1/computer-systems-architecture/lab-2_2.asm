bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit
extern printf                  ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll
import printf msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    ; ...
    a db 134
    b db 7
    c db 15
    d db 5
    mesaj db "Rezultatul la a-b-d+2+c+(10-b) este: %d", 0
    rez db 0

; our code starts here
segment code use32 class=code
    start:
        ; ...
        mov al, byte [a] ; al = a
        sub al, byte [b] ; al = a-b
        sub al, byte [d] ; al = a-b-d
        add al, byte [c] ; al = a-b-d+c
        add al, 2 ; al = a-b-d+c-2
        mov bl, 10 ; bl = 10
        sub bl, byte [b] ; bl = 10-b
        add al, bl; al = a-b-d+c+(10-b)
        mov [rez], al; rez = a-b-d+c+(10-b)
        push dword [rez];
        push mesaj
        call[printf]
        
        
    
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
