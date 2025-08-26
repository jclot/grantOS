#include "keyboard.h"
#include "io.h"

static int shift_pressed = 0;
static int caps_lock_on = 0;

int isalpha(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

static char scancode_to_ascii(uint8_t scancode)
{
    static const char keyboard_map[] = {
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
        '\t', 's', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
        'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '};

    static const char keyboard_shift_map[] = {
        0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
        '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
        0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|',
        'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' '};

    if (scancode >= sizeof(keyboard_map))
    {
        return 0;
    }

    char base = keyboard_map[scancode];
    char shifted = keyboard_shift_map[scancode];

    if (isalpha(base))
    {
        int upper = shift_pressed ^ caps_lock_on;
        return upper ? shifted : base;
    }
    else
    {
        return shift_pressed ? shifted : base;
    }
}

void keyboard_handler(uint8_t scancode)
{

    if (scancode == 0x2A || scancode == 0x36)
    {
        shift_pressed = 1;
        return;
    }

    if (scancode == 0xAA || scancode == 0xB6)
    {
        shift_pressed = 0;
        return;
    }

    if (scancode == 0x3A)
    {
        caps_lock_on = !caps_lock_on;
        return;
    }

    if (scancode & 0x80)
    {
        return;
    }
}

char keyboard_read_char()
{
    uint8_t status = inb(0x64);
    if (status & 0x01)
    {
        uint8_t scancode = inb(0x60);
        if (!(scancode & 0x80))
        {
            return scancode_to_ascii(scancode);
        }
    }
    return 0;
}
