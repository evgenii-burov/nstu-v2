.386
.model flat, stdcall

extern GetStdHandle@4:PROC
extern ReadConsoleA@20:PROC
extern WriteConsoleA@20:PROC
extern ExitProcess@4:PROC

Coef_A EQU 4
Coef_B EQU 19
Coef_C EQU -2

.data
    msgIn    BYTE "Введите число в 16-ричной системе: "
    msgOut   BYTE "Число в 10-ричной системе: "
    msgPolyO BYTE "Значение многочлена в 8-ричной системе: "
    msgPolyD BYTE "Значение многочлена в 10-ричной системе: "
    msgErr   BYTE "Ошибка ввода! Нужно 16-ричное число от -5A84 до 5A80 (допустим знак минус).", 13, 10
    crlf     BYTE 13, 10
    inBuf    BYTE 32 DUP(0)
    outBuf   BYTE 16 DUP(0)

.data?
    hIn      DWORD ?
    hOut     DWORD ?
    nRead    DWORD ?
    nWritten DWORD ?

.code
; mult(x, y) -> EAX, умножение сложением
mult PROC
    push ebp
    mov  ebp, esp
    push ebx
    push edi

    mov  eax, [ebp+8]
    mov  ebx, [ebp+12]
    xor  edi, edi               ; признак отрицательного результата
    test eax, eax
    jns  m_x
    neg  eax
    xor  edi, 1
m_x:
    test ebx, ebx
    jns  m_y
    neg  ebx
    xor  edi, 1
m_y:
    cmp  eax, ebx
    jbe  m_order
    xchg eax, ebx               ; EAX - меньший модуль, EBX - больший
m_order:
    mov  ecx, eax               ; счётчик цикла
    xor  eax, eax               ; накопитель
    test ecx, ecx
    jz   m_sign
m_add:
    add  eax, ebx
    dec  ecx
    jnz  m_add
m_sign:
    test edi, edi
    jz   m_end
    neg  eax
m_end:
    pop  edi
    pop  ebx
    pop  ebp
    ret  8
mult ENDP

; PrintNum(число, основание, знаковое)
PrintNum PROC
    push ebp
    mov  ebp, esp
    push ebx
    push esi
    push edi

    mov  eax, [ebp+8]
    mov  ebx, [ebp+12]
    xor  edi, edi
    cmp  DWORD PTR [ebp+16], 0
    je   convert
    test eax, eax
    jns  convert
    neg  eax
    mov  edi, 1
convert:
    mov  esi, OFFSET outBuf + 16
digit_loop:
    xor  edx, edx
    div  ebx
    add  dl, '0'
    dec  esi
    mov  [esi], dl
    test eax, eax
    jnz  digit_loop
    test edi, edi
    jz   print
    dec  esi
    mov  BYTE PTR [esi], '-'
print:
    mov  edi, OFFSET outBuf + 16
    sub  edi, esi

    push 0
    push OFFSET nWritten
    push edi
    push esi
    push hOut
    call WriteConsoleA@20

    pop  edi
    pop  esi
    pop  ebx
    pop  ebp
    ret  12
PrintNum ENDP

main PROC
    push -10
    call GetStdHandle@4
    mov  hIn, eax
    push -11
    call GetStdHandle@4
    mov  hOut, eax

input_again:
    push 0
    push OFFSET nWritten
    push SIZEOF msgIn
    push OFFSET msgIn
    push hOut
    call WriteConsoleA@20

    push 0
    push OFFSET nRead
    push SIZEOF inBuf
    push OFFSET inBuf
    push hIn
    call ReadConsoleA@20

    ; ---------- 16-ричная строка (со знаком) -> число (EBX) с проверкой ----------
    ; допустимый диапазон: -5A84h .. 5A80h (при нём многочлен помещается в int32)
    mov  esi, OFFSET inBuf
    xor  ebx, ebx
    xor  edi, edi               ; признак минуса
    xor  ecx, ecx               ; число введённых цифр
    cmp  BYTE PTR [esi], '-'
    jne  hex_loop
    mov  edi, 1
    inc  esi
hex_loop:
    movzx eax, BYTE PTR [esi]
    cmp  al, '0'
    jb   hex_end
    cmp  al, '9'
    jbe  hex_digit
    or   al, 20h
    cmp  al, 'a'
    jb   hex_end
    cmp  al, 'f'
    ja   hex_end
    sub  al, 'a' - 10
    jmp  hex_add
hex_digit:
    sub  al, '0'
hex_add:
    shl  ebx, 4
    or   ebx, eax
    inc  ecx
    cmp  ebx, 5A84h             ; уже вышли за максимум по модулю -> ошибка
    ja   bad_input              ; (заодно нет переполнения EBX)
    inc  esi
    jmp  hex_loop
hex_end:
    test ecx, ecx               ; нет ни одной цифры
    jz   bad_input
    mov  al, [esi]              ; после числа допустим только конец строки
    cmp  al, 13
    je   hex_done
    cmp  al, 10
    je   hex_done
    test al, al
    jnz  bad_input
hex_done:
    test edi, edi
    jz   check_pos
    neg  ebx                    ; минимум -5A84h уже проверен в цикле
    jmp  calc
check_pos:
    cmp  ebx, 5A80h
    jbe  calc

bad_input:
    push 0
    push OFFSET nWritten
    push SIZEOF msgErr
    push OFFSET msgErr
    push hOut
    call WriteConsoleA@20
    jmp  input_again

    ; ---------- многочлен: (Coef_A*x + Coef_B)*x + Coef_C -> ESI ----------
calc:
    push Coef_A
    push ebx
    call mult
    add  eax, Coef_B
    push eax
    push ebx
    call mult
    add  eax, Coef_C
    mov  esi, eax

    ; ---------- вывод числа ----------
    push 0
    push OFFSET nWritten
    push SIZEOF msgOut
    push OFFSET msgOut
    push hOut
    call WriteConsoleA@20

    push 1
    push 10
    push ebx
    call PrintNum

    push 0
    push OFFSET nWritten
    push SIZEOF crlf
    push OFFSET crlf
    push hOut
    call WriteConsoleA@20

    ; ---------- многочлен в 8-ричной ----------
    push 0
    push OFFSET nWritten
    push SIZEOF msgPolyO
    push OFFSET msgPolyO
    push hOut
    call WriteConsoleA@20

    push 1
    push 8
    push esi
    call PrintNum

    push 0
    push OFFSET nWritten
    push SIZEOF crlf
    push OFFSET crlf
    push hOut
    call WriteConsoleA@20

    ; ---------- многочлен в 10-ричной ----------
    push 0
    push OFFSET nWritten
    push SIZEOF msgPolyD
    push OFFSET msgPolyD
    push hOut
    call WriteConsoleA@20

    push 1
    push 10
    push esi
    call PrintNum

    push 0
    push OFFSET nWritten
    push SIZEOF crlf
    push OFFSET crlf
    push hOut
    call WriteConsoleA@20

    push 0
    call ExitProcess@4
main ENDP
END main
