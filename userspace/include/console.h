#ifndef CONSOLE_H
#define CONSOLE_H

#include <types.h>

struct key_event {
    u16 scancode;
    bool is_release;
};

enum vga_color {
    VGA_COLOR_BLACK,
    VGA_COLOR_BLUE,
    VGA_COLOR_GREEN,
    VGA_COLOR_CYAN,
    VGA_COLOR_RED,
    VGA_COLOR_MAGENTA,
    VGA_COLOR_BROWN,
    VGA_COLOR_WHITE,
    VGA_COLOR_GRAY,
    VGA_COLOR_LIGHT_BLUE,
    VGA_COLOR_LIGHT_GREEN,
    VGA_COLOR_LIGHT_CYAN,
    VGA_COLOR_LIGHT_RED,
    VGA_COLOR_LIGHT_MAGENTA,
    VGA_COLOR_YELLOW,
    VGA_COLOR_BRIGHT_WHITE,
};

#define SC_ENTER 0x005a
#define SC_TAB 0x000d
#define SC_SPACE 0x0029
#define SC_BACKSPACE 0x0066

#define SC_LEFT_CTRL 0x0014
#define SC_RIGHT_CTRL 0xe014

#define SC_LEFT_ALT 0x0011
#define SC_RIGHT_ALT 0xe011

#define SC_LEFT_SHIFT 0x0012
#define SC_RIGHT_SHIFT 0x0059

#define SC_F1 0x0005
#define SC_F2 0x0006
#define SC_F3 0x0004
#define SC_F4 0x000c
#define SC_F5 0x0003
#define SC_F6 0x000b
#define SC_F7 0x0083
#define SC_F8 0x000a
#define SC_F9 0x0001
#define SC_F10 0x0009
#define SC_F11 0x0078
#define SC_F12 0x0007

#define SC_UP 0xe075
#define SC_DOWN 0xe072
#define SC_LEFT 0xe06b
#define SC_RIGHT 0xe074

#define SC_HOME 0xe06c
#define SC_END 0xe069
#define SC_PAGEUP 0xe07d
#define SC_PAGEDOWN 0xe07a
#define SC_INSERT 0xe070
#define SC_DELETE 0xe071

#define SC_A 0x001c
#define SC_B 0x0032
#define SC_C 0x0021
#define SC_D 0x0023
#define SC_E 0x0024
#define SC_F 0X002b
#define SC_G 0x0034
#define SC_H 0x0033
#define SC_I 0x0043
#define SC_J 0x003b
#define SC_K 0x0042
#define SC_L 0x004b
#define SC_M 0x003a
#define SC_N 0x0031
#define SC_O 0x0044
#define SC_P 0x004d
#define SC_Q 0x0015
#define SC_R 0x002d
#define SC_S 0x001b
#define SC_T 0x002c
#define SC_U 0x003c
#define SC_V 0x002a
#define SC_W 0x001d
#define SC_X 0x0022
#define SC_Y 0x0035
#define SC_Z 0x001a

#define SC_0 0x0045
#define SC_1 0x0016
#define SC_2 0x001e
#define SC_3 0x0026
#define SC_4 0x0025
#define SC_5 0x002e
#define SC_6 0x0036
#define SC_7 0x003d
#define SC_8 0x003e
#define SC_9 0x0046

#define SC_COMMA 0x0041
#define SC_PERIOD 0x0049
#define SC_SLASH 0x004a
#define SC_SEMICOLON 0x004c
#define SC_APOSTROPHE 0x0052
#define SC_LEFT_BRACKET 0x0054
#define SC_RIGHT_BRACKET 0x005b
#define SC_BACKSLASH 0x005d
#define SC_HYPHEN 0x004e
#define SC_EQUAL 0x0055
#define SC_BACKTICK 0x000e

#define SC_NUMPAD_ENTER 0xe05a
#define SC_NUMPAD_SLASH 0xe04a
#define SC_NUMPAD_ASTERISK 0x007c
#define SC_NUMPAD_HYPHEN 0x007b
#define SC_NUMPAD_PLUS 0x0079

#endif
