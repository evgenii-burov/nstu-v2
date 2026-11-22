.386
.model flat, c

.code

; int Substr(const char* src, char* dst, int start, int len)
; [ebp+8]  - src    (исходная строка, оканчивается нулём)
; [ebp+12] - dst    (буфер под результат)
; [ebp+16] - start  (позиция, считая с 1)
; [ebp+20] - len    (длина подстроки)
; Возврат в EAX: 0 - успех, 1 - ошибка границ
SubstrUpper PROC
    push ebp
    mov  ebp, esp
    push esi                    ; cdecl: ESI, EDI, EBX нужно сохранять
    push edi
    push ebx

    ; --- длина исходной строки (repne scasb) ---
    cld
    mov  edi, [ebp+8]
    xor  al, al                 ; ищем нулевой байт
    mov  ecx, 0FFFFFFFFh        ; максимальный счётчик
    repne scasb                 ; сканируем, пока [EDI] <> AL
    not  ecx
    dec  ecx                    ; ECX = длина строки без нуля
    mov  ebx, ecx               ; EBX = strLen

    ; --- проверка границ ---
    mov  eax, [ebp+16]          ; start
    cmp  eax, 1
    jl   bad                    ; start < 1
    mov  ecx, [ebp+20]          ; len
    cmp  ecx, 1
    jl   bad                    ; len < 1
    cmp  eax, ebx
    jg   bad                    ; start > strLen
    mov  edx, ebx
    sub  edx, eax
    inc  edx                    ; EDX = сколько символов осталось с позиции start
    cmp  ecx, edx
    jg   bad                    ; len больше остатка строки

    ; --- вырезание подстроки (rep movsb) ---
    mov  esi, [ebp+8]
    add  esi, [ebp+16]
    dec  esi                    ; позиции с 1, смещения с 0
    mov  edi, [ebp+12]
    mov  ecx, [ebp+20]
    rep  movsb
    mov  byte ptr [edi], 0      ; завершающий ноль для вывода через cout

    ; --- приведение к верхнему регистру (lodsb / stosb), кодировка cp866 ---
    mov  esi, [ebp+12]
    mov  edi, esi
    mov  ecx, [ebp+20]
upLoop:
    lodsb
    cmp  al, 'a'
    jb   upStore
    cmp  al, 'z'
    jbe  shift20                ; латиница a..z
    cmp  al, 0A0h
    jb   upStore
    cmp  al, 0AFh
    jbe  shift20                ; кириллица а..п
    cmp  al, 0E0h
    jb   upStore
    cmp  al, 0EFh
    jbe  shift50                ; кириллица р..я
    cmp  al, 0F1h
    jne  upStore
    dec  al                     ; ё -> Ё
    jmp  upStore
shift20:
    sub  al, 20h
    jmp  upStore
shift50:
    sub  al, 50h
upStore:
    stosb
    loop upLoop

    xor  eax, eax               ; успех
    jmp  quit
bad:
    mov  eax, 1                 ; ошибка
quit:
    pop  ebx
    pop  edi
    pop  esi
    pop  ebp
    ret                         ; cdecl: стек очищает вызывающий (C++)
SubstrUpper ENDP

END