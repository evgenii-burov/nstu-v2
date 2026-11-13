.386
.model flat, stdcall

EXTERN  GetStdHandle@4: PROC 
EXTERN  WriteConsoleA@20: PROC 
EXTERN  CharToOemA@8: PROC 
EXTERN  ReadConsoleA@20: PROC 
EXTERN  ExitProcess@4: PROC

.data

dOut dword ?
dIn dword ?

nWritten dword ?
nRead dword ?

msg byte "Введите число: ", 0
msgLen = $ - msg - 1

inputBuf byte 256 dup(?)

outputBuf byte 256 dup(?)

num dword ?

.code
main PROC
	push offset msg
	push offset msg
	call CharToOemA@8

	push -10
	call GetStdHandle@4
	mov dIn, eax
	
	push -11
	call GetStdHandle@4
	mov dOut, eax

	push 0
	push offset nWritten
	push msgLen
	push offset msg
	push dOut
	call WriteConsoleA@20

	push 0 
	push offset nRead
	push 256
	push offset inputBuf
	push dIn
	call ReadConsoleA@20

	mov esi, offset inputBuf
	mov ebx, 0
	cld
convert:
	lodsb
	mov ebx, al+'0'



	push 0
	push offset nWritten
	push nRead
	push offset inputBuf
	push dOut
	call WriteConsoleA@20

	push 0
	call ExitProcess@4

main endp
end main