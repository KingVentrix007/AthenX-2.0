#include "../include/keyboard.h"

#include "../include/console.h"
#include "../include/idt.h"
#include "../include/io_ports.h"
#include "../include/isr.h"
#include "../include/types.h"
#include "../include/string.h"
#include "../include/stdio.h"

IO_STREAM kb_buffer;

static BOOL g_caps_lock = FALSE;
static BOOL g_shift_pressed = FALSE;
char *g_ch = 0, g_scan_code = 0;

// see scan codes defined in keyboard.h for index
char g_scan_code_chars[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ',
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0
};

/**
 * get_scancode
 *
 * Parameters:
 *   
 *   
 * Return:
 *   scan code
 */
static int get_scancode() {
    int i, scancode = 0;

    // get scancode until status is on(key pressed)
    for (i = 1000; i > 0; i++) {
        // Check if scan code is ready
        if ((inportb(KEYBOARD_STATUS_PORT) & 1) == 0) continue;
        // Get the scan code
        scancode = inportb(KEYBOARD_DATA_PORT);
        break;
    }
    if (i > 0)
        return scancode;
    return 0;
}

char alternate_chars(char ch) {
    switch(ch) {
        case '`': return '~';
        case '1': return '!';
        case '2': return '@';
        case '3': return '#';
        case '4': return '$';
        case '5': return '%';
        case '6': return '^';
        case '7': return '&';
        case '8': return '*';
        case '9': return '(';
        case '0': return ')';
        case '-': return '_';
        case '=': return '+';
        case '[': return '{';
        case ']': return '}';
        case '\\': return '|';
        case ';': return ':';
        case '\'': return '\"';
        case ',': return '<';
        case '.': return '>';
        case '/': return '?';
        default: return ch;
    }
}

void keyboard_handler(REGISTERS *r) {
    int scancode;

    g_ch = 0;
    scancode = get_scancode();
    g_scan_code = scancode;
    if (scancode & 0x80) {
        // key release
    } else {
        // key down
        switch(scancode) {
            case SCAN_CODE_KEY_CAPS_LOCK:
                if (g_caps_lock == FALSE)
                    g_caps_lock = TRUE;
                else
                    g_caps_lock = FALSE;
                break;

            case SCAN_CODE_KEY_ENTER:
                g_ch = '\n';
                break;

            case SCAN_CODE_KEY_TAB:
                g_ch = '\t';
                break;

            case SCAN_CODE_KEY_LEFT_SHIFT:
                g_shift_pressed = TRUE;
                break;
            case 0x48: // Up Arrow
                g_ch = SCAN_CODE_KEY_UP;
                break;
            case 0x50: // Down Arrow
                g_ch = SCAN_CODE_KEY_DOWN;
                break;
            case 0x4D: // Right Arrow
                g_ch =  SCAN_CODE_KEY_RIGHT;
                break;
            case 0x4B: // Left Arrow
                g_ch = SCAN_CODE_KEY_LEFT;
                break;
            case 0x3B: // F1
                g_ch = "\u23A7";
                break;
            case 0x3C: // F2
                g_ch = "\u23A8";
                break;
            case 0x3D: // F3
                g_ch = "\u23A9";
                break;
            case 0x3E: // F4
                g_ch = "\u23AA";
                break;
            case 0x3F: // F5
                g_ch = "\u23AB";
                break;
            case 0x40: // F6
                g_ch = "\u23AC";
                break;
            case 0x41: // F7
                g_ch = "\u23AD";
                break;
            case 0x42: // F8
                g_ch = "\u23AE";
                break;
            case 0x43: // F9
                g_ch = "\u23AF";
                break;
            case 0x44: // F10
                g_ch = "\u23B0";
                break;
            case 0x57: // F11
                g_ch = "\u23B1";
                break;
            case 0x58: // F12
                g_ch = "\u23B2";
                break;
            default:
                g_ch = g_scan_code_chars[scancode];
                // if caps in on, covert to upper
                if (g_caps_lock) {
                    // if shift is pressed before
                    if (g_shift_pressed) {
                        // replace alternate chars
                        g_ch = alternate_chars(g_ch);
                    } else
                        g_ch = upper(g_ch);
                } else {
                    if (g_shift_pressed) {
                        if (isalpha(g_ch))
                            g_ch = upper(g_ch);
                        else 
                            // replace alternate chars
                        g_ch = alternate_chars(g_ch);
                    } else
                        g_ch = g_scan_code_chars[scancode];
                    g_shift_pressed = FALSE;
                }
                break;
        }
    }
    kb_buffer.buffer_pos++; 
    kb_buffer.buffer[kb_buffer.buffer_pos] = (int)g_ch;
    //printf("%c",kb_buffer.buffer[kb_buffer.buffer_pos]);
    
    
}

void keyboard_init() {
    // for (size_t i = 0; i < IO_BUFFER_SIZE; i++)
    // {
    //     kb_buffer.buffer[i] = 0;

    // }
    kb_buffer.buffer_pos = 1;
    kb_buffer.buffer[0] = '\0';
    
    isr_register_interrupt_handler(IRQ_BASE + 1, keyboard_handler);
}




// a blocking character read
char* kb_getchar() {
    int count = 0;
    //printf_("HERE");
    char *c;
    
    while(g_ch <= 0)
    {
        count++;
        if(count >= 100)
        {
           return '\0'; 
        }
    };
    c = g_ch;
    g_ch = 0;
    g_scan_code = 0;
    return c;
}

char kb_get_scancode() {
    char code;
    int count = 0;
    while(g_scan_code <= 0)
    {
        count++;
        if(count >= 100)
        {
            return -1;
        }
    };
    code = g_scan_code;
    g_ch = 0;
    g_scan_code = 0;
    return code;
}


int get_input(int mode)
{
    int key = kb_get_scancode();
    int count = 0;
    while(key <= 0)
    {
        count++;
        if(count >= 100 && mode == 1)
        {
            return -1;
        }
    }
    return key;
}

char *inter_key(int mode)
{
    int key = get_input(key);
    if(key == -1) return -1;
    switch (key) {
        case 0x48: // Up Arrow
            return "\u2191";
        case 0x50: // Down Arrow
            return "\u2193";
        case 0x4D: // Right Arrow
            return "\u2192";
        case 0x4B: // Left Arrow
            return "\u2190";
        case 0x3B: // F1
            return "\u23A7";
        case 0x3C: // F2
            return "\u23A8";
        case 0x3D: // F3
            return "\u23A9";
        case 0x3E: // F4
            return "\u23AA";
        case 0x3F: // F5
            return "\u23AB";
        case 0x40: // F6
            return "\u23AC";
        case 0x41: // F7
            return "\u23AD";
        case 0x42: // F8
            return "\u23AE";
        case 0x43: // F9
            return "\u23AF";
        case 0x44: // F10
            return "\u23B0";
        case 0x57: // F11
            return "\u23B1";
        case 0x58: // F12
            return "\u23B2";
        default:
                char out = g_scan_code_chars[key];
                // if caps in on, covert to upper
                if (g_caps_lock) {
                    // if shift is pressed before
                    if (g_shift_pressed) {
                        // replace alternate chars
                        out = alternate_chars(out);
                    } else
                        out = upper(out);
                } else {
                    if (g_shift_pressed) {
                        if (isalpha(out))
                            out = upper(out);
                        else 
                            // replace alternate chars
                        out = alternate_chars(out);
                    } else
                        g_ch = g_scan_code_chars[key];
                    g_shift_pressed = FALSE;
                }
                break;
            
                // if caps in on, covert to upper
                
        
    }

}
int get_char()
{
    
    int chr = kb_buffer.buffer[kb_buffer.buffer_pos];
    if(chr == '\0')
    {
        return EOF;
    }
    kb_buffer.buffer[kb_buffer.buffer_pos];
    if(kb_buffer.buffer_pos > 0)
    {
        kb_buffer.buffer_pos--;
    }
    
    return chr;
}

int unget_char(int c)
{
    kb_buffer.buffer_pos++;
    if(kb_buffer.buffer_pos >= IO_BUFFER_SIZE)
    {
        //printf("EOF");
        return -10;
    }
    kb_buffer.buffer[kb_buffer.buffer_pos] = c;
    if(kb_buffer.buffer[kb_buffer.buffer_pos] == c)
    {
        return c;
    }
    else
    {
        //printf("EOF 3");
        return EOF;
    }

}
