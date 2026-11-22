.386
.model flat, c

.code

CalcFunc PROC
    push ebp
    mov  ebp, esp
    push ebx

    mov  ebx, [ebp+16]          ; EBX = ok

    ; --- проверка области определения: x != 1 ---
    ; Стек FPU: пуст
    fld  qword ptr [ebp+8]      ; ST(0) = x
    fld1                        ; ST(0) = 1.0, ST(1) = x
    fucompp                     ; pop обоих
                                ; Стек FPU: пуст
    fnstsw ax
    test ah, 40h                ; x == 1.0 ?
    jnz  undefined

    ; --- числитель: x + 2*x^2 ---
    ; Стек FPU: пуст
    fld  qword ptr [ebp+8]      ; ST(0) = x
    fld  st(0)                  ; ST(0) = x, ST(1) = x
    fmul st(0), st(0)           ; ST(0) = x^2, ST(1) = x
    fadd st(0), st(0)           ; ST(0) = 2*x^2, ST(1) = x
    fadd st(0), st(1)           ; ST(0) = x + 2*x^2, ST(1) = x
                                ; Стек FPU: ST(0)=числитель, ST(1)=x

    ; --- знаменатель: x - 1 ---
    fld1                        ; ST(0)=1, ST(1)=числитель, ST(2)=x
    fsubr st(0), st(2)          ; ST(0)=x-1, ST(1)=числитель, ST(2)=x
                                ; Стек FPU: ST(0)=x-1, ST(1)=числитель, ST(2)=x

    ; --- деление ---
    fdivp st(1), st(0)          ; ST(0)=числитель/(x-1), ST(1)=x
                                ; Стек FPU: ST(0)=результат, ST(1)=x

    fstp st(1)                  ; ST(0)=результат
                                ; Стек FPU: ST(0)=результат

    ; --- нормализация -0.0 -> +0.0 ---
    ; Стек FPU: ST(0)=результат
    sub  esp, 8
    fst  qword ptr [esp]        ; сохранили, ST(0) на месте
                                ; Стек FPU: ST(0)=результат
    fldz                        ; ST(0)=0.0, ST(1)=результат
    fucompp                     ; pop обоих
                                ; Стек FPU: пуст
    fnstsw ax
    test ah, 40h
    jz   restore_res
    fldz                        ; ST(0)=+0.0
                                ; Стек FPU: ST(0)=+0.0
    add  esp, 8
    jmp  set_ok

restore_res:
    fld  qword ptr [esp]        ; ST(0)=результат
                                ; Стек FPU: ST(0)=результат
    add  esp, 8

set_ok:
    mov  dword ptr [ebx], 1
    jmp  quit

undefined:
    ; Стек FPU: пуст
    fldz                        ; ST(0)=+0.0
                                ; Стек FPU: ST(0)=+0.0
    mov  dword ptr [ebx], 0

quit:
    pop  ebx
    pop  ebp
    ret
CalcFunc ENDP

END