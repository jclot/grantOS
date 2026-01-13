#include "keyboard_driver.h"
#include "io.h"

static int shift_pressed = 0;
static int caps_lock_on = 0;
static int driver_initialized = 0;

static int isalpha(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

static const char keyboard_map[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '};

static const char keyboard_shift_map[] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' '};

static char scancode_to_ascii(uint8_t scancode)
{
    if (scancode >= sizeof(keyboard_map))
    {
        return 0;
    }

    char base = keyboard_map[scancode];
    char shifted = keyboard_shift_map[scancode];

    if (isalpha(base))
    {
        int should_uppercase = shift_pressed ^ caps_lock_on;
        return should_uppercase ? shifted : base;
    }
    else
    {
        return shift_pressed ? shifted : base;
    }
}

static int handle_special_keys(uint8_t scancode)
{
    if (scancode == 0x2A || scancode == 0x36)
    {
        shift_pressed = 1;
        return 1;
    }

    if (scancode == 0xAA || scancode == 0xB6)
    {
        shift_pressed = 0;
        return 1;
    }

    if (scancode == 0x3A)
    {
        caps_lock_on = !caps_lock_on;
        return 1;
    }

    if (scancode & 0x80)
    {
        return 1;
    }

    return 0;
}

static char keyboard_read_char_impl(void)
{
    if (!driver_initialized)
    {
        return 0;
    }

    uint8_t status = inb(0x64);
    if (status & 0x01)
    {
        uint8_t scancode = inb(0x60);

        if (handle_special_keys(scancode))
        {
            return 0;
        }

        return scancode_to_ascii(scancode); // If no special key, convert to ASCII
    }
    return 0;
}

static int keyboard_set_layout(int layout)
{
    (void)layout;
    return 0;
}

static int keyboard_get_status(void)
{
    return driver_initialized ? 1 : 0;
}

static keyboard_ops_t keyboard_operations = {
    .read_char = keyboard_read_char_impl,
    .set_layout = keyboard_set_layout,
    .get_status = keyboard_get_status};

int keyboard_driver_init(void)
{
    shift_pressed = 0;
    caps_lock_on = 0;
    driver_initialized = 1;
    return 0;
}

int keyboard_driver_cleanup(void)
{
    driver_initialized = 0;
    shift_pressed = 0;
    caps_lock_on = 0;
    return 0;
}

char keyboard_read_char(void)
{
    return keyboard_read_char_impl();
}

static driver_module_t keyboard_driver_module = {
    .name = "keyboard",
    .version = "1.0.0",
    .type = DRIVER_TYPE_INPUT,
    .status = DRIVER_STATUS_UNLOADED,
    .init = keyboard_driver_init,
    .cleanup = keyboard_driver_cleanup,
    .suspend = NULL, // Not necessary for keyboard
    .resume = NULL,  // Not necessary for keyboard
    .driver_ops = &keyboard_operations};

int keyboard_driver_register(void)
{
    return driver_register(&keyboard_driver_module);
}