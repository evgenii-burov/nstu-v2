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

    msgPos    BYTE "Введите номер позиции: ", 0
    msgPosLen = $ - msgPos - 1

    msgSub    BYTE "Введите длину подстроки: ", 0
    msgSubLen = $ - msgSub - 1

    numBuf    BYTE 16 dup(0)          ; буфер для ввода числа
    numSize   = 16

    startPos  DWORD ?                 ; номер позиции (число)
    subLen    DWORD ?                 ; длина подстроки (число)

    strLen    DWORD ?                 ; длина введённой строки без CR LF

    msgRes    BYTE "Подстрока: ", 0
    msgResLen = $ - msgRes - 1

    msgErr    BYTE "Ошибка: подстрока выходит за границы строки", 13, 10, 0
    msgErrLen = $ - msgErr - 1

    crlf      BYTE 13, 10             ; перевод строки (только ASCII, не перекодируем)
    subBuf    BYTE 256 dup(0)         ; буфер для подстроки

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
    mov  eax, nRead
    sub  eax, 2                 ; отбрасываем 0Dh 0Ah
    mov  strLen, eax

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

    ; --- перекодировка обоих сообщений в OEM ---
    push offset msgPos
    push offset msgPos
    call CharToOemA@8
    push offset msgSub
    push offset msgSub
    call CharToOemA@8

    ; --- запрос и ввод номера позиции ---
    push 0
    push offset nWritten
    push msgPosLen
    push offset msgPos
    push dOut
    call WriteConsoleA@20

    push 0
    push offset nRead
    push numSize
    push offset numBuf
    push dIn
    call ReadConsoleA@20

    mov  esi, offset numBuf
    call StrToInt
    mov  startPos, eax

    ; --- запрос и ввод длины подстроки ---
    push 0
    push offset nWritten
    push msgSubLen
    push offset msgSub
    push dOut
    call WriteConsoleA@20

    push 0
    push offset nRead
    push numSize
    push offset numBuf
    push dIn
    call ReadConsoleA@20

    mov  esi, offset numBuf
    call StrToInt
    mov  subLen, eax

	    ; --- перекодировка сообщений ---
    push offset msgRes
    push offset msgRes
    call CharToOemA@8
    push offset msgErr
    push offset msgErr
    call CharToOemA@8

    ; --- проверка: 1 <= startPos, 1 <= subLen, startPos+subLen-1 <= strLen ---
    mov  eax, startPos
    cmp  eax, 1
    jb   inputErr
    mov  ecx, subLen
    cmp  ecx, 1
    jb   inputErr
    add  eax, ecx
    dec  eax                    ; номер последнего символа подстроки
    cmp  eax, strLen
    ja   inputErr

    ; --- вырезание подстроки ---
    cld
    mov  esi, offset buffer
    add  esi, startPos
    dec  esi                    ; позиции с 1, смещения с 0
    mov  edi, offset subBuf
    mov  ecx, subLen
    rep  movsb                  ; копируем ECX байт из [ESI] в [EDI]
    ; --- приведение подстроки к верхнему регистру ---
    cld
    mov  esi, offset subBuf     ; читаем из subBuf
    mov  edi, esi               ; и пишем обратно в subBuf
    mov  ecx, subLen
upper:
    lodsb                       ; AL = [ESI], ESI++
    cmp  al, 'a'
    jb   store                  ; меньше 'a' - не трогаем
    cmp  al, 'z'
    jbe  shift20                ; латиница a..z
    cmp  al, 0A0h
    jb   store
    cmp  al, 0AFh
    jbe  shift20                ; русские а..п (A0h..AFh)
    cmp  al, 0E0h
    jb   store
    cmp  al, 0EFh
    jbe  shift50                ; русские р..я (E0h..EFh)
    cmp  al, 0F1h
    jne  store
    dec  al                     ; ё (F1h) -> Ё (F0h)
    jmp  store
shift20:
    sub  al, 20h
    jmp  store
shift50:
    sub  al, 50h
store:
    stosb                       ; [EDI] = AL, EDI++
    loop upper                  ; ECX--, повтор пока ECX <> 0

    ; --- вывод результата ---
    push 0
    push offset nWritten
    push msgResLen
    push offset msgRes
    push dOut
    call WriteConsoleA@20

    push 0
    push offset nWritten
    push subLen
    push offset subBuf
    push dOut
    call WriteConsoleA@20

    push 0
    push offset nWritten
    push 2
    push offset crlf
    push dOut
    call WriteConsoleA@20
    jmp  finish

inputErr:
    push 0
    push offset nWritten
    push msgErrLen
    push offset msgErr
    push dOut
    call WriteConsoleA@20

finish:
    push 0                      ; код возврата
    call ExitProcess@4
main ENDP

; Перевод строки десятичных цифр в число
; Вход:  ESI - адрес строки
; Выход: EAX - число (разбор идёт до первого символа, не являющегося цифрой)
StrToInt PROC
    push ebx
    push ecx
    xor  ebx, ebx           ; EBX - накопитель результата
    cld                     ; обход строки вперёд
conv:
    lodsb                   ; AL = [ESI], ESI = ESI + 1
    sub  al, '0'            ; код символа -> значение цифры
    cmp  al, 9
    ja   done               ; беззнаковое сравнение: не цифра -> конец
    movzx ecx, al
    imul ebx, ebx, 10       ; результат = результат * 10 + цифра
    add  ebx, ecx
    jmp  conv
done:
    mov  eax, ebx
    pop  ecx
    pop  ebx
    ret
StrToInt ENDP

END main