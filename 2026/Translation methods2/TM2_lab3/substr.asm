.386
.model flat, c

.code

; int Substr(const char* src, char* dst, int start, int len)
; [ebp+8]  - src    (РёСЃС…РѕРґРЅР°СЏ СЃС‚СЂРѕРєР°, РѕРєР°РЅС‡РёРІР°РµС‚СЃСЏ РЅСѓР»С‘Рј)
; [ebp+12] - dst    (Р±СѓС„РµСЂ РїРѕРґ СЂРµР·СѓР»СЊС‚Р°С‚)
; [ebp+16] - start  (РїРѕР·РёС†РёСЏ, СЃС‡РёС‚Р°СЏ СЃ 1)
; [ebp+20] - len    (РґР»РёРЅР° РїРѕРґСЃС‚СЂРѕРєРё)
; Р’РѕР·РІСЂР°С‚ РІ EAX: 0 - СѓСЃРїРµС…, 1 - РѕС€РёР±РєР° РіСЂР°РЅРёС†
SubstrUpper PROC
    push ebp
    mov  ebp, esp
    push esi                    ; cdecl: ESI, EDI, EBX РЅСѓР¶РЅРѕ СЃРѕС…СЂР°РЅСЏС‚СЊ
    push edi
    push ebx

    ; --- РґР»РёРЅР° РёСЃС…РѕРґРЅРѕР№ СЃС‚СЂРѕРєРё (repne scasb) ---
    cld
    mov  edi, [ebp+8]
    xor  al, al                 ; РёС‰РµРј РЅСѓР»РµРІРѕР№ Р±Р°Р№С‚
    mov  ecx, 0FFFFFFFFh        ; РјР°РєСЃРёРјР°Р»СЊРЅС‹Р№ СЃС‡С‘С‚С‡РёРє
    repne scasb                 ; СЃРєР°РЅРёСЂСѓРµРј, РїРѕРєР° [EDI] <> AL
    not  ecx
    dec  ecx                    ; ECX = РґР»РёРЅР° СЃС‚СЂРѕРєРё Р±РµР· РЅСѓР»СЏ
    mov  ebx, ecx               ; EBX = strLen

    ; --- РїСЂРѕРІРµСЂРєР° РіСЂР°РЅРёС† ---
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
    inc  edx                    ; EDX = СЃРєРѕР»СЊРєРѕ СЃРёРјРІРѕР»РѕРІ РѕСЃС‚Р°Р»РѕСЃСЊ СЃ РїРѕР·РёС†РёРё start
    cmp  ecx, edx
    jg   bad                    ; len Р±РѕР»СЊС€Рµ РѕСЃС‚Р°С‚РєР° СЃС‚СЂРѕРєРё

    ; --- РІС‹СЂРµР·Р°РЅРёРµ РїРѕРґСЃС‚СЂРѕРєРё (rep movsb) ---
    mov  esi, [ebp+8]
    add  esi, [ebp+16]
    dec  esi                    ; РїРѕР·РёС†РёРё СЃ 1, СЃРјРµС‰РµРЅРёСЏ СЃ 0
    mov  edi, [ebp+12]
    mov  ecx, [ebp+20]
    rep  movsb
    mov  byte ptr [edi], 0      ; Р·Р°РІРµСЂС€Р°СЋС‰РёР№ РЅРѕР»СЊ РґР»СЏ РІС‹РІРѕРґР° С‡РµСЂРµР· cout

    ; --- РїСЂРёРІРµРґРµРЅРёРµ Рє РІРµСЂС…РЅРµРјСѓ СЂРµРіРёСЃС‚СЂСѓ (lodsb / stosb), РєРѕРґРёСЂРѕРІРєР° cp866 ---
    mov  esi, [ebp+12]
    mov  edi, esi
    mov  ecx, [ebp+20]
upLoop:
    lodsb
    cmp  al, 'a'
    jb   upStore
    cmp  al, 'z'
    jbe  shift20                ; Р»Р°С‚РёРЅРёС†Р° a..z
    cmp  al, 0A0h
    jb   upStore
    cmp  al, 0AFh
    jbe  shift20                ; РєРёСЂРёР»Р»РёС†Р° Р°..Рї
    cmp  al, 0E0h
    jb   upStore
    cmp  al, 0EFh
    jbe  shift50                ; РєРёСЂРёР»Р»РёС†Р° СЂ..СЏ
    cmp  al, 0F1h
    jne  upStore
    dec  al                     ; С‘ -> РЃ
    jmp  upStore
shift20:
    sub  al, 20h
    jmp  upStore
shift50:
    sub  al, 50h
upStore:
    stosb
    loop upLoop

    xor  eax, eax               ; СѓСЃРїРµС…
    jmp  quit
bad:
    mov  eax, 1                 ; РѕС€РёР±РєР°
quit:
    pop  ebx
    pop  edi
    pop  esi
    pop  ebp
    ret                         ; cdecl: СЃС‚РµРє РѕС‡РёС‰Р°РµС‚ РІС‹Р·С‹РІР°СЋС‰РёР№ (C++)
SubstrUpper ENDP

END