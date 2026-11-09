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

    prompt    BYTE "Р’РІРµРґРёС‚Рµ СЃС‚СЂРѕРєСѓ: ", 0
    promptLen = $ - prompt - 1        ; РґР»РёРЅР° Р±РµР· Р·Р°РІРµСЂС€Р°СЋС‰РµРіРѕ РЅСѓР»СЏ

    buffer    BYTE 256 dup(0)         ; Р±СѓС„РµСЂ РґР»СЏ РІРІРѕРґРёРјРѕР№ СЃС‚СЂРѕРєРё
    bufSize   = 256

.code

main PROC
    push -10 
    call GetStdHandle@4 
    mov  dIn, eax 
    push -11 
    call GetStdHandle@4 
    mov  dOut, eax

        ; --- РїРµСЂРµРєРѕРґРёСЂРѕРІРєР° РїСЂРёРіР»Р°С€РµРЅРёСЏ РІ РєРѕРґРёСЂРѕРІРєСѓ РєРѕРЅСЃРѕР»Рё (OEM) ---
    push offset prompt          ; pDst (РєСѓРґР°) - РїСЂРµРѕР±СЂР°Р·СѓРµРј "РЅР° РјРµСЃС‚Рµ"
    push offset prompt          ; pSrc (РѕС‚РєСѓРґР°)
    call CharToOemA@8

    ; --- РІС‹РІРѕРґ РїСЂРёРіР»Р°С€РµРЅРёСЏ ---
    push 0                      ; lpReserved
    push offset nWritten        ; СЃРєРѕР»СЊРєРѕ СЃРёРјРІРѕР»РѕРІ Р·Р°РїРёСЃР°РЅРѕ
    push promptLen              ; СЃРєРѕР»СЊРєРѕ СЃРёРјРІРѕР»РѕРІ РІС‹РІРѕРґРёС‚СЊ
    push offset prompt          ; Р°РґСЂРµСЃ СЃС‚СЂРѕРєРё
    push dOut                   ; РґРµСЃРєСЂРёРїС‚РѕСЂ РІС‹РІРѕРґР°
    call WriteConsoleA@20

    ; --- РІРІРѕРґ СЃС‚СЂРѕРєРё ---
    push 0                      ; pInputControl
    push offset nRead           ; СЃРєРѕР»СЊРєРѕ СЃРёРјРІРѕР»РѕРІ РїСЂРѕС‡РёС‚Р°РЅРѕ
    push bufSize                ; СЂР°Р·РјРµСЂ Р±СѓС„РµСЂР°
    push offset buffer          ; Р°РґСЂРµСЃ Р±СѓС„РµСЂР°
    push dIn                    ; РґРµСЃРєСЂРёРїС‚РѕСЂ РІРІРѕРґР°
    call ReadConsoleA@20

	call ExitProcess@4
main ENDP
END main