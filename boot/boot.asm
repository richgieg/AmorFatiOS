[bits 16]
[org 0x7c00]

; Enable address line 20 (A20) via BIOS.
; Allows access to memory addresses that have bit 20 set (odd megabytes).
mov ax, 0x2401
int 0x15

; Populate the memory map at 0x10000 for the kernel's memory manager.
mov ax, 0x1000
mov es, ax
xor edi, edi
xor ebx, ebx
mov edx, 0x534d4150

populate_map_entry:
    mov eax, 0xe820
    mov ecx, 24
    int 0x15
    add di, 24
    test ebx, ebx
    jnz populate_map_entry

; Write an entry of all zeroes as a marker.
xor al, al
mov cx, 24
rep stosb

; Use 80x25 VGA text mode.
mov ax, 0x0003
int 0x10

; Uncomment to change font to allow for 80x50.
; mov ax, 0x1112
; mov bl, 0
; int 0x10

; Copy the first 18 sectors of kernel image from floppy to 0x20000.
mov bx, 0x2000      ; segment location to read into
mov es, bx
mov bx, 0           ; offset to read into
mov ah, 2           ; BIOS read sector function
mov al, 18          ; number of sectors to read (18 max)
mov ch, 1           ; track to read (1 = start at 36 sectors in)
mov cl, 1           ; sector to read (starts at 1)
mov dh, 0           ; head to read
mov dl, 0           ; drive to read
int 0x13            ; make the BIOS call

; Copy the next 18 sectors of kernel image from floppy.
mov bx, 0x2400
mov ah, 2
mov al, 18
mov ch, 1
mov cl, 1
mov dh, 1
mov dl, 0
int 0x13

; Copy the next 18 sectors of kernel image from floppy.
mov bx, 0x4800
mov ah, 2
mov al, 18
mov ch, 2
mov cl, 1
mov dh, 0
mov dl, 0
int 0x13

; Copy the next 18 sectors of kernel image from floppy.
mov bx, 0x6c00
mov ah, 2
mov al, 18
mov ch, 2
mov cl, 1
mov dh, 1
mov dl, 0
int 0x13

; Copy the next 18 sectors of kernel image from floppy.
mov bx, 0x9000
mov ah, 2
mov al, 18
mov ch, 3
mov cl, 1
mov dh, 0
mov dl, 0
int 0x13

; Copy the first 18 sectors of userspace image from floppy to 0x90000.
mov bx, 0x9000      ; segment location to read into
mov es, bx
mov bx, 0           ; offset to read into
mov ah, 2           ; BIOS read sector function
mov al, 18          ; number of sectors to read (18 max)
mov ch, 16          ; track to read (16 = start at 576 sectors in)
mov cl, 1           ; sector to read (starts at 1)
mov dh, 0           ; head to read
mov dl, 0           ; drive to read
int 0x13            ; make the BIOS call

; Copy the next 18 sectors of userspace image from floppy.
mov bx, 0xb400
mov ah, 2
mov al, 18
mov ch, 16
mov cl, 1
mov dh, 1
mov dl, 0
int 0x13

; Disable interrupts.
cli

; Load the global descriptor table.
xor ax, ax
mov ds, ax
lgdt [gdt_desc]

; Enable protected mode.
mov eax, cr0
or eax, 1
mov cr0, eax

; Switch to protected mode via jump to code segment.
jmp 0x08:finish_setup

[bits 32]

finish_setup:
    
    ; Initialize data and stack registers.
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    ; Jump to the kernel entry point.
    jmp 0x21000

; Define the global descriptor table.
gdt:
gdt_null:
    dq 0
gdt_code:
    dw 0xffff
    dw 0
    db 0
    db 0b10011010
    db 0b11001111
    db 0
gdt_data:
    dw 0xffff
    dw 0
    db 0
    db 0b10010010
    db 0b11001111
    db 0
gdt_end:

gdt_desc:
    dw gdt_end - gdt - 1
    dd gdt

; Pad with zeroes as needed up to the 510-byte mark.
times 510-($-$$) db 0

; Add boot signature.
dw 0xaa55
