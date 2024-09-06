#ifndef KEYBOARD_H
#define KEYBOARD_H

#define SC_LEFT_ALT 0x0011
#define SC_RIGHT_ALT 0xe011
#define SC_LEFT_SHIFT 0x0012
#define SC_RIGHT_SHIFT 0x0059

#define SC_TAB 0x000d

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

void keyboard_init(void);

#endif
