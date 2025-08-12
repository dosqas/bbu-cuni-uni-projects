bits 32 ; assembling for the 32 bits architecture

; declare the EntryPoint (a label defining the very first instruction of the program)
global start        

; declare external functions needed by our program
extern exit    
extern printf           ; tell nasm that exit exists even if we won't be defining it
import exit msvcrt.dll
import printf msvcrt.dll    ; exit is a function that ends the calling process. It is defined in msvcrt.dll
                          ; msvcrt.dll contains exit, printf and all the other important C-runtime specific functions

; our data is declared here (the variables needed by our program)
segment data use32 class=data
    ; ...
    a db 5
    b db 5
    c db 5
    e dd 8
    x dq 125

    mesaj db "Rezultatul la x-(a*b*25+c*3)/(a+b)+e este: %d"

; our code starts here
segment code use32 class=code
    start:
        ; ...
        mov al, byte [a] ; al = a
        mov bl, byte [b] ; bl = b
        imul bl ; ax = a*b
        mov bx, 25 ; bx = 25
        imul bx ; dx:ax = a*b*25
        push dx
        push ax
        pop eax ; eax = a*b*25
        mov ecx, eax ; ecx = a*b*25
        mov al, byte [c] ; al = c
        mov bl, 3 ; bl = 3
        imul bl ; ax = c*3
        CWDE; eax = c*3
        add ecx, eax ; ecx = a*b*25+c*3  
        mov al, byte [a] ; al = a
        mov bl, byte [b] ; bl = b
        add al, bl ; bl = a+b
        CBW ; ax = a+b
        mov bx, ax ; bx = a+b
        mov eax, ecx ; eax = a*b*25+c*3   
        idiv bx ; ax = (a*b*25+c*3)/(a+b)
        CWDE; eax = (a*b*25+c*3)/(a+b)
        mov ecx, eax
        mov eax, dword [x]
        mov edx, dword [x+4] ; edx:eax = x
        sub eax, ecx ; edx:eax = x-(a*b*25+c*3)/(a+b)
        
        
        mov ebx, dword [e] ; ebx = e
        add eax, ebx ; eax = x-(a*b*25+c*3)/(a+b)+e
        push edx
        push eax
        push mesaj
        call[printf]
        ; exit(0)
        push    dword 0      ; push the parameter for exit onto the stack
        call    [exit]       ; call exit to terminate the program
