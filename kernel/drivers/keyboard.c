#include <drivers/keyboard.h>
#include <stdint.h>
#include <kernel/idt.h>
#include <kernel/tty.h>
#include <stdio.h>

int capslk = 0;
int caps = 0;
int shift = 0;

void print_letter(uint8_t scancode)
{
    switch (scancode) {
        case 0x0:
            printf("ERROR");
            break;
        case 0x1:
            printf("ESC");
            break;
        case 0x2:
            printf(shift ? "!" : "1");
            break;
        case 0x3:
            printf(shift ? "@" : "2");
            break;
        case 0x4:
            printf(shift ? "#" : "3");
            break;
        case 0x5:
            printf(shift ? "$" : "4");
            break;
        case 0x6:
            printf(shift ? "%" : "5");
            break;
        case 0x7:
            printf(shift ? "^" : "6");
            break;
        case 0x8:
            printf(shift ? "&" : "7");
            break;
        case 0x9:
            printf(shift ? "*" : "8");
            break;
        case 0x0A:
            printf(shift ? "(" : "9");
            break;
        case 0x0B:
            printf(shift ? ")" : "0");
            break;
        case 0x0C:
            printf(shift ? "_" : "-");
            break;
        case 0x0D:
            printf(shift ? "+" : "=");
            break;
        case 0x0E:
            terminal_backspace();
            break;
        case 0x0F:
            printf("    ");
            break;
        case 0x10:
            printf(caps ? "Q" : "q");
            break;
        case 0x11:
            printf(caps ? "W" : "w");
            break;
        case 0x12:
            printf(caps ? "E" : "e");
            break;
        case 0x13:
            printf(caps ? "R" : "r");
            break;
        case 0x14:
            printf(caps ? "T" : "t");
            break;
        case 0x15:
            printf(caps ? "Y" : "y");
            break;
        case 0x16:
            printf(caps ? "U" : "u");
            break;
        case 0x17:
            printf(caps ? "I" : "i");
            break;
        case 0x18:
            printf(caps ? "O" : "o");
            break;
        case 0x19:
            printf(caps ? "P" : "p");
            break;
		case 0x1A:
			printf(shift ? "{" : "[");
			break;
		case 0x1B:
			printf(shift ? "}" : "]");
			break;
		case 0x1C:
			printf("\n");
			break;
		case 0x1D:
			printf(""); // LCtrl
			break;
		case 0x1E:
			printf(caps ? "A" : "a");
			break;
		case 0x1F:
			printf(caps ? "S" : "s");
			break;
        case 0x20:
            printf(caps ? "D" : "d");
            break;
        case 0x21:
            printf(caps ? "F" : "f");
            break;
        case 0x22:
            printf(caps ? "G" : "g");
            break;
        case 0x23:
            printf(caps ? "H" : "h");
            break;
        case 0x24:
            printf(caps ? "J" : "j");
            break;
        case 0x25:
            printf(caps ? "K" : "k");
            break;
        case 0x26:
            printf(caps ? "L" : "l");
            break;
        case 0x27:
            printf(shift ? ":" : ";");
            break;
        case 0x28:
            printf(shift ? "\"" : "'");
            break;
        case 0x29:
            printf(shift ? "~" : "`");
            break;
		case 0x2B:
			printf(shift ? "|" : "\\");
			break;
		case 0x2C:
			printf(caps ? "Z" : "z");
			break;
		case 0x2D:
			printf(caps ? "X" : "x");
			break;
		case 0x2E:
			printf(caps ? "C" : "c");
			break;
		case 0x2F:
			printf(caps ? "V" : "v");
			break;
        case 0x30:
            printf(caps ? "B" : "b");
            break;
        case 0x31:
            printf(caps ? "N" : "n");
            break;
        case 0x32:
            printf(caps ? "M" : "m");
            break;
        case 0x33:
            printf(shift ? "<" : ",");
            break;
        case 0x34:
            printf(shift ? ">" : ".");
            break;
        case 0x35:
            printf(shift ? "?" : "/");
            break;
        case 0x36:
            printf(""); // RShift
            break;
        case 0x37:
            printf("*");
            break;
        case 0x38:
            printf(""); //LAlt
            break;
        case 0x39:
            printf(" ");
            break;
 		case 0x3a:
			printf(""); // CapsLk
			break;
		case 0x3b:
			printf(""); // F1
            printf("%d", get_row());
			break;
		case 0x3c:
			printf(""); // F2
            printf("%d", get_col());
			break;
		case 0x3d:
			printf(""); // F3
			break;
		case 0x3e:
			printf(""); // F4
			break;
		case 0x3f:
			printf(""); // F5
			break;
		case 0x40:
			printf(""); // F6
			break;
		case 0x41:
			printf(""); // F7
			break;
		case 0x42:
			printf(""); // F8
			break;
		case 0x43:
			printf(""); // F9
			break;
		case 0x44:
			printf(""); // F10
			break;
		case 0x45:
			printf(""); // NumLK
			break;
		case 0x46:
			printf(""); // ScLk
			break;
		case 0x47:
			printf("7");
			break;
		case 0x48:
			printf("8");
			break;
		case 0x49:
			printf("9");
			break;
		case 0x4a:
			printf("-");
			break;
		case 0x4b:
			printf("4");
			break;
		case 0x4c:
			printf("5");
			break;
		case 0x4d:
			printf("6");
			break;
		case 0x4e:
			printf("+");
			break;
		case 0x4f:
			printf("1");
			break;
		case 0x50:
			printf("2");
			break;
		case 0x51:
			printf("3");
			break;
		case 0x52:
			printf("0");
			break;
		case 0x53:
			printf(".");
			break;
		case 0x54:
			printf("");
			break;
		case 0x55:
			printf("");
			break;
		case 0x56:
			printf("");
			break;
		case 0x57:
			printf(""); // F11
			break;
        case 0x58:
            printf(""); // F12
            break;

        default:
            /* 'keuyp' event corresponds to the 'keydown' + 0x80 
             * it may still be a scancode we haven't implemented yet, or
             * maybe a control/escape sequence */
            
            break;
    }
}


void keyboard_handler(registers_t r)
{
    /* The PIC leaves us the scancode in port 0x60 */
    uint8_t scancode = inb(0x60);
    if (scancode == KEY_CAPSLOCK) capslk = capslk ? 0 : 1;
    if (scancode == KEY_LEFT_SHIFT || scancode == KEY_RIGHT_SHIFT) {shift = 1; caps = 1;}
    if (scancode == KEY_REL_LEFT_SHIFT || scancode == KEY_REL_RIGHT_SHIFT) {shift = 0; caps = capslk ? 1 : 0;}
    caps = shift | capslk;
    print_letter(scancode);
}