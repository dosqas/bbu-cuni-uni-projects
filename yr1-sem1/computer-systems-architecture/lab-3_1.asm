bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit     
extern printf          ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll
import printf msvcrt.dll     ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    ; ...
    a db 8
    b dw 4
    c dd 5
    d dq 10
    rez dd 0
    mesaj db "Rezultatul la a+b-c+(d-a) este: %d", 0

; our code starts here
segment code use32 class=code
    start:
        ; ...
        mov al, byte [a] ; al = a
        mov ah, 0 ; ax = a
        mov bx, word [b] ; bx = b
        add ax, bx ; ax = a+b
        push 0
        push ax
        pop eax ; eax = a+b
        mov ebx, dword [c] ; ebx = c
        sub eax, ebx ; eax = a+b-c
        mov ecx, eax ; ecx = a+b-c
        mov ebx, dword [d] 
        mov edx, dword [d+4] ; edx:ebx = d
        mov al, byte [a] ; al = a
        mov ah, 0 ; ax = a
        push 0 
        push ax
        pop eax ; eax = a
        sub ebx, eax ; edx:ebx = d-a
        add ebx, ecx ; edx:ebx = (d-a)+a+b-c
        push edx
        push ebx
        
        push mesaj
        call [printf]
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
