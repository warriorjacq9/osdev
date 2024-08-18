#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <kernel/idt.h>

#define CMD_LED         0xED // Set LEDs
#define CMD_ECHO        0xEE // Echo
#define CMD_SCAN_SET    0xF0 // Get / Set scancode set
#define CMD_IDENTIFY    0xF2 // Identify keyboard type
#define CMD_REPEATDELAY 0xF3 // Typematic repeat and delay
#define CMD_SCAN_ENABLE 0xF4 // Enable scanning
#define CMD_SCAN_DISABLE 0xF5// Disable scanning
#define CMD_DEF_PARAMS  0xF6 // Set params to default
#define CMD_AUTOREPEAT  0xF7 // Set all keys to autorepeat (Scan code set 3 only)
#define CMD_MAKE_REL    0xF8 // Set all keys to make/release (Scan code set 3 only)
#define CMD_MAKE_ONLY   0xF9 // Set all keys to make only (Scan code set 3 only)
#define CMD_TYPE_ALL    0xFA // Set all keys to all types (Scan code st 3 only)
#define CMD_AUTOREP_KEY 0xFB // Set key to autorepeat (Scan code 3 only)
#define CMD_MAKEREL_KEY 0xFC // Set key to make/release (Scan code set 3 only)
#define CMD_MAKE_KEY    0xFD // Set key to make only (Scan code set 3 only)
#define CMD_RESEND      0xFE // Resend last sent byte
#define CMD_TEST        0xFF // Restart and start a self-test

#define RES_ERR         0x00 // Key detection or buffer overflow error
#define RES_TEST_PASS   0xAA // Self test passed
#define RES_ECHO        0xEE // Response to CMD_ECHO
#define RES_ACK         0xFA // Command acknowledged
#define RES_TEST_FAIL   0xFC // Self test failed or mouse transmit error
#define RES_FAIL        0xFD // Internal failure
#define RES_RESEND      0xFE // Repeat last command sent
#define RES_ERR_        0xFF // Key detection or buffer overflow error

#define KEY_ESCAPE      0x01
#define KEY_1           0x02
#define KEY_2           0x03
#define KEY_3           0x04
#define KEY_4           0x05
#define KEY_5           0x06
#define KEY_6           0x07
#define KEY_7           0x08
#define KEY_8           0x09
#define KEY_9           0x0A
#define KEY_0           0x0B
#define KEY_MINUS       0x0C
#define KEY_EQUALS      0x0D
#define KEY_BKSPACE     0x0E
#define KEY_TAB         0x0F
#define KEY_Q           0x10
#define KEY_W           0x11
#define KEY_E           0x12
#define KEY_R           0x13
#define KEY_T           0x14
#define KEY_Y           0x15
#define KEY_U           0x16
#define KEY_I           0x17
#define KEY_O           0x18
#define KEY_P           0x19
#define KEY_LEFT_BRKET  0x1A
#define KEY_RIGHT_BRKET 0x1B
#define KEY_ENTER       0x1C
#define KEY_LEFT_CTRL   0x1D
#define KEY_A           0x1E
#define KEY_S           0x1F
#define KEY_D           0x20
#define KEY_F           0x21
#define KEY_G           0x22
#define KEY_H           0x23
#define KEY_J           0x24
#define KEY_K           0x25
#define KEY_L           0x26
#define KEY_SEMICOLON   0x27
#define KEY_SINGLEQUOTE 0x28
#define KEY_BACKTICK    0x29
#define KEY_LEFT_SHIFT  0x2A
#define KEY_BACKSLASH   0x2B
#define KEY_Z           0x2C
#define KEY_X           0x2D
#define KEY_C           0x2E
#define KEY_V           0x2F
#define KEY_B           0x30
#define KEY_N           0x31
#define KEY_M           0x32
#define KEY_COMMA       0x33
#define KEY_PERIOD      0x34
#define KEY_SLASH       0x35
#define KEY_RIGHT_SHIFT 0x36
#define KEY_NUM_ASTERISK 0x37
#define KEY_LEFT_ALT    0x38
#define KEY_SPACE       0x39
#define KEY_CAPSLOCK    0x3A
#define KEY_F1          0x3B
#define KEY_F2          0x3C
#define KEY_F3          0x3D
#define KEY_F4          0x3E
#define KEY_F5          0x3F
#define KEY_F6          0x40
#define KEY_F7          0x41
#define KEY_F8          0x42
#define KEY_F9          0x43
#define KEY_F10         0x44
#define KEY_NUMLOCK     0x45
#define KEY_SCROLLOCK   0x46
#define KEY_NUM_7       0x47
#define KEY_NUM_8       0x48
#define KEY_NUM_9       0x49
#define KEY_NUM_MINUS   0x4A
#define KEY_NUM_4       0x4B
#define KEY_NUM_5       0x4C
#define KEY_NUM_6       0x4D
#define KEY_NUM_PLUS    0x4E
#define KEY_NUM_1       0x4F
#define KEY_NUM_2       0x50
#define KEY_NUM_3       0x51
#define KEY_NUM_0       0x52
#define KEY_NUM_PERIOD  0x53

#define KEY_F11         0x57
#define KEY_F12         0x58

#define KEY_REL_ESCAPE  0x81
#define KEY_REL_1       0x82
#define KEY_REL_2       0x83
#define KEY_REL_3       0x84
#define KEY_REL_4       0x85
#define KEY_REL_5       0x86
#define KEY_REL_6       0x87
#define KEY_REL_7       0x88
#define KEY_REL_8       0x89
#define KEY_REL_9       0x8A
#define KEY_REL_0       0x8B
#define KEY_REL_MINUS   0x8C
#define KEY_REL_EQUALS  0x8D
#define KEY_REL_BKSPACE 0x8E
#define KEY_REL_TAB     0x8F
#define KEY_REL_Q       0x90
#define KEY_REL_W       0x91
#define KEY_REL_E       0x92
#define KEY_REL_R       0x93
#define KEY_REL_T       0x94
#define KEY_REL_Y       0x95
#define KEY_REL_U       0x96
#define KEY_REL_I       0x97
#define KEY_REL_O       0x98
#define KEY_REL_P       0x99
#define KEY_REL_LEFT_BRKET 0x9A
#define KEY_REL_RIGHT_BRKET 0x9B
#define KEY_REL_ENTER   0x9C
#define KEY_REL_LEFT_CTRL 0x9D
#define KEY_REL_A       0x9E
#define KEY_REL_S       0x9F
#define KEY_REL_D       0xA0
#define KEY_REL_F       0xA1
#define KEY_REL_G       0xA2
#define KEY_REL_H       0xA3
#define KEY_REL_J       0xA4
#define KEY_REL_K       0xA5
#define KEY_REL_L       0xA6
#define KEY_REL_SEMICOLON 0xA7
#define KEY_REL_SINGLE_QUOTE 0xA8
#define KEY_REL_BACKTICK 0xA9
#define KEY_REL_LEFT_SHIFT 0xAA
#define KEY_REL_BACKSLASH 0xAB
#define KEY_REL_Z       0xAC
#define KEY_REL_X       0xAD
#define KEY_REL_C       0xAE
#define KEY_REL_V       0xAF
#define KEY_REL_B       0xB0
#define KEY_REL_N       0xB1
#define KEY_REL_M       0xB2
#define KEY_REL_COMMA   0xB3
#define KEY_REL_PERIOD  0xB4
#define KEY_REL_SLASH   0xB5
#define KEY_REL_RIGHT_SHIFT 0xB6
#define KEY_REL_NUM_ASTERISK 0xB7
#define KEY_REL_LEFT_ALT 0xB8
#define KEY_REL_SPACE   0xB9
#define KEY_REL_CAPSLOCK 0xBA
#define KEY_REL_F1      0xBB
#define KEY_REL_F2      0xBC
#define KEY_REL_F3      0xBD
#define KEY_REL_F4      0xBE
#define KEY_REL_F5      0xBF
#define KEY_REL_F6      0xC0
#define KEY_REL_F7      0xC1
#define KEY_REL_F8      0xC2
#define KEY_REL_F9      0xC3
#define KEY_REL_F10     0xC4
#define KEY_REL_NUMLOCK 0xC5
#define KEY_REL_SCROLLOCK 0xC6
#define KEY_REL_NUM_7   0xC7
#define KEY_REL_NUM_8   0xC8
#define KEY_REL_NUM_9   0xC9
#define KEY_REL_NUM_MINUS 0xCA
#define KEY_REL_NUM_4   0xCB
#define KEY_REL_NUM_5   0xCC
#define KEY_REL_NUM_6   0xCD
#define KEY_REL_NUM_PLUS 0xCE
#define KEY_REL_NUM_1   0xCF
#define KEY_REL_NUM_2   0xD0
#define KEY_REL_NUM_3   0xD1
#define KEY_REL_NUM_0   0xD2
#define KEY_REL_NUM_PERIOD 0xD3

#define KEY_REL_F11     0xD7
#define KEY_REL_F12     0xD8

#define KEY_EXT         0xE0 // Extended keycode, one of two bytes

#define KEY_EXT_PREV    0x10
#define KEY_EXT_NEXT    0x19
#define KEY_EXT_NUM_ENTER 0x1C
#define KEY_EXT_RIGHT_CTRL 0x1D
#define KEY_EXT_MUTE    0x20
#define KEY_EXT_CALC    0x21
#define KEY_EXT_PLAY    0x22
#define KEY_EXT_STOP    0x24
#define KEY_EXT_VOL_DOWN 0x2E
#define KEY_EXT_VOL_UP  0x30
#define KEY_EXT_WEB     0x32
#define KEY_EXT_NUM_SLASH 0x35
#define KEY_EXT_ALTGR   0x38
#define KEY_EXT_HOME    0x47
#define KEY_EXT_UP      0x48
#define KEY_EXT_PAGEUP  0x49
#define KEY_EXT_LEFT    0x4B
#define KEY_EXT_RIGHT   0x4D
#define KEY_EXT_END     0x4F
#define KEY_EXT_DOWN    0x50
#define KEY_EXT_PAGEDOWN 0x51
#define KEY_EXT_INSERT  0x52
#define KEY_EXT_DEL     0x53
#define KEY_EXT_LEFT_WIN 0x5B
#define KEY_EXT_RIGHT_WIN 0x5C
#define KEY_EXT_APPS    0x5D
#define KEY_EXT_POWER   0x5E
#define KEY_EXT_SLEEP   0x5F
#define KEY_EXT_WAKE    0x63
#define KEY_EXT_SEARCH  0x65
#define KEY_EXT_FAVORITES 0x66
#define KET_EXT_REFRESH 0x67
#define KEY_EXT_WWW_STOP 0x68
#define KEY_EXT_FORWARD 0x69
#define KEY_EXT_BACK    0x6A
#define KEY_EXT_COMPUTER 0x6B
#define KEY_EXT_EMAIL   0x6C
#define KEY_EXT_MEDIA   0x6D

#define KEY_EXT_REL_PREV    0x90
#define KEY_EXT_REL_NEXT    0x99
#define KEY_EXT_REL_NUM_ENTER 0x9C
#define KEY_EXT_REL_RIGHT_CTRL 0x9D
#define KEY_EXT_REL_MUTE    0xA0
#define KEY_EXT_REL_CALC    0xA1
#define KEY_EXT_REL_PLAY    0xA2
#define KEY_EXT_REL_STOP    0xA4
#define KEY_EXT_REL_VOL_DOWN 0xAE
#define KEY_EXT_REL_VOL_UP  0xB0
#define KEY_EXT_REL_WEB     0xB2
#define KEY_EXT_REL_NUM_SLASH 0xB5
#define KEY_EXT_REL_ALTGR   0xB8
#define KEY_EXT_REL_HOME    0xC7
#define KEY_EXT_REL_UP      0xC8
#define KEY_EXT_REL_PAGEUP  0xC9
#define KEY_EXT_REL_LEFT    0xCB
#define KEY_EXT_REL_RIGHT   0xCD
#define KEY_EXT_REL_END     0xCF
#define KEY_EXT_REL_DOWN    0xD0
#define KEY_EXT_REL_PAGEDOWN 0xD1
#define KEY_EXT_REL_INSERT  0xD2
#define KEY_EXT_REL_DEL     0xD3
#define KEY_EXT_REL_LEFT_WIN 0xDB
#define KEY_EXT_REL_RIGHT_WIN 0xDC
#define KEY_EXT_REL_APPS    0xDD
#define KEY_EXT_REL_POWER   0xDE
#define KEY_EXT_REL_SLEEP   0xDF
#define KEY_EXT_REL_WAKE    0xE3
#define KEY_EXT_REL_SEARCH  0xE5
#define KEY_EXT_REL_FAVORITES 0xE6
#define KET_EXT_REL_REFRESH 0xE7
#define KEY_EXT_REL_WWW_STOP 0xE8
#define KEY_EXT_REL_FORWARD 0xE9
#define KEY_EXT_REL_BACK    0xEA
#define KEY_EXT_REL_COMPUTER 0xEB
#define KEY_EXT_REL_EMAIL   0xEC
#define KEY_EXT_REL_MEDIA   0xED

void keyboard_handler(registers_t r);
#endif