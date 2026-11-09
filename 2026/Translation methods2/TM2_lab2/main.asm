.386
.model flat, stdcall

extern GetStdHandle@4:PROC 
extern ReadConsoleA@20:PROC 
extern WriteConsoleA@20:PROC 
extern ExitProcess@4:PROC 
EXTERN CharToOemA@8: PROC

.data
	dIn      DWORD ? 
    dOut     DWORD ? 
    nRead    DWORD ? 
    nWritten DWORD ?

    prompt    BYTE "Введите строку: ", 0
    promptLen = $ - prompt - 1        ; длина без завершающего нуля

    buffer    BYTE 256 dup(0)         ; буфер для вводимой строки
    bufSize   = 256

    msgOut    BYTE "Вы ввели: ", 0
    msgOutLen = $ - msgOut - 1

.code

main PROC
    push -10 
    call GetStdHandle@4 
    mov  dIn, eax 
    push -11 
    call GetStdHandle@4 
    mov  dOut, eax

    ; --- перекодировка приглашения в кодировку консоли (OEM) ---
    push offset prompt          ; pDst (куда) - преобразуем "на месте"
    push offset prompt          ; pSrc (откуда)
    call CharToOemA@8

    ; --- вывод приглашения ---
    push 0                      ; lpReserved
    push offset nWritten        ; сколько символов записано
    push promptLen              ; сколько символов выводить
    push offset prompt          ; адрес строки
    push dOut                   ; дескриптор вывода
    call WriteConsoleA@20

    ; --- ввод строки ---
    push 0                      ; pInputControl
    push offset nRead           ; сколько символов прочитано
    push bufSize                ; размер буфера
    push offset buffer          ; адрес буфера
    push dIn                    ; дескриптор ввода
    call ReadConsoleA@20

    ; --- перекодировка сообщения "Вы ввели: " в OEM ---
    push offset msgOut          ; pDst
    push offset msgOut          ; pSrc
    call CharToOemA@8

    ; --- вывод сообщения ---
    push 0
    push offset nWritten
    push msgOutLen
    push offset msgOut
    push dOut
    call WriteConsoleA@20

    ; --- вывод введённой строки ---
    push 0
    push offset nWritten
    push nRead                  ; выводим ровно столько символов, сколько прочитали
    push offset buffer
    push dOut
    call WriteConsoleA@20

	call ExitProcess@4
main ENDP
END main