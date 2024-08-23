[bits 16]
[org 0x7c00]

; Copy the first 17 sectors of kernel image from floppy to 0x20000.
mov bx, 0x2000      ; segment location to read into
mov es, bx
mov bx, 0           ; offset to read into
mov ah, 2           ; BIOS read sector function
mov al, 17          ; number of sectors to read (18 max)
mov ch, 0           ; track to read
mov cl, 2           ; sector to read (starts at 1, skipping boot sector)
mov dh, 0           ; head to read
mov dl, 0           ; drive to read
int 0x13            ; make the BIOS call

; Copy the next 18 sectors of kernel image from floppy.
mov bx, 0x2200
mov ah, 2
mov al, 18
mov ch, 0
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

; Initialize data and stack registers.
finish_setup:
mov ax, 0x10
mov ds, ax
mov ss, ax
mov esp, 0x90000

; Jump to the kernel entry point.
jmp 0x22000

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
