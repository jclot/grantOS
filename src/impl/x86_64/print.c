#include "print.h"
#include "io.h"
#include "../kernel/memory/memory.h"

const static size_t NUM_COLS = 80;
const static size_t NUM_ROWS = 25;
const static size_t HEADER_ROWS = 2;

struct Char
{
    uint8_t character;
    uint8_t color;
};

struct Char *buffer = (struct Char *)0xb8000;
size_t col = 0;
size_t row = HEADER_ROWS;
uint8_t color = PRINT_COLOR_WHITE | (PRINT_COLOR_BLACK << 4);

static struct Char header_backup[80 * 2]; // 2 rows × 80 cols
static int header_initialized = 0;

void clear_row(size_t target_row)
{
    struct Char empty = (struct Char){
        .character = ' ',
        .color = color,
    };

    for (size_t col = 0; col < NUM_COLS; col++)
    {
        buffer[col + NUM_COLS * target_row] = empty;
    }
}

void print_pinned_header()
{
    // Save current color
    uint8_t saved_color = color;

    // Row 0: "Welcome to GrantOS Console!"
    print_set_color(PRINT_COLOR_LIGHT_GREEN, PRINT_COLOR_BLACK);
    size_t pos = 0;
    const char *welcome = " Welcome to GrantOS Console!\n";
    for (size_t i = 0; welcome[i] != '\0'; i++)
    {
        if (welcome[i] == '\n')
            break;
        buffer[pos] = (struct Char){
            .character = (uint8_t)welcome[i],
            .color = color,
        };
        pos++;
    }

    while (pos < NUM_COLS)
    {
        buffer[pos] = (struct Char){
            .character = ' ',
            .color = color,
        };
        pos++;
    }

    // Row 1: "Type 'help' for available commands."
    print_set_color(PRINT_COLOR_LIGHT_CYAN, PRINT_COLOR_BLACK);
    pos = NUM_COLS;
    const char *help = "Type 'help' for available commands.";
    for (size_t i = 0; help[i] != '\0'; i++)
    {
        buffer[pos] = (struct Char){
            .character = (uint8_t)help[i],
            .color = color,
        };
        pos++;
    }

    while (pos < NUM_COLS * 2)
    {
        buffer[pos] = (struct Char){
            .character = ' ',
            .color = color,
        };
        pos++;
    }

    // Restore previous color
    color = saved_color;

    // Backup header
    for (size_t i = 0; i < NUM_COLS * HEADER_ROWS; i++)
    {
        header_backup[i] = buffer[i];
    }
    header_initialized = 1;
}

void preserve_header()
{
    if (header_initialized)
    {
        for (size_t i = 0; i < NUM_COLS * HEADER_ROWS; i++)
        {
            buffer[i] = header_backup[i];
        }
    }
}

void update_cursor()
{

    // Keep cursor in bounds
    if (row < HEADER_ROWS)
        row = HEADER_ROWS;
    if (row >= NUM_ROWS)
        row = NUM_ROWS - 1;
    if (col >= NUM_COLS)
        col = NUM_COLS - 1;

    size_t pos = col + NUM_COLS * row;

    // Ensure pos is within valid range
    if (pos >= NUM_COLS * NUM_ROWS)
    {
        pos = NUM_COLS * NUM_ROWS - 1;
    }

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void print_clear()
{
    for (size_t i = HEADER_ROWS; i < NUM_ROWS; i++)
    {
        clear_row(i);
    }
    col = 0;
    row = HEADER_ROWS;
    preserve_header();
    update_cursor();
}

void scroll_screen()
{
    for (size_t r = HEADER_ROWS; r < NUM_ROWS - 1; r++)
    {
        for (size_t c = 0; c < NUM_COLS; c++)
        {
            struct Char character = buffer[c + NUM_COLS * (r + 1)];
            buffer[c + NUM_COLS * r] = character;
        }
    }
    clear_row(NUM_ROWS - 1);
    preserve_header();
}

void print_newline()
{
    col = 0;

    if (row < NUM_ROWS - 1)
    {
        row++;
    }
    else
    {
        scroll_screen();
    }
}

void handle_backspace()
{
    if (col == 0 && row <= HEADER_ROWS)
    {
        return;
    }

    if (col == 0)
    {
        if (row > HEADER_ROWS)
        {
            row--;
            col = NUM_COLS - 1;
            // Scan backwards to find last non-space character
        }
    }
    else
    {
        col--;
    }

    buffer[col + NUM_COLS * row] = (struct Char){
        .character = ' ',
        .color = color,
    };
}

void handle_wrap()
{
    // Wrap text if at end of line
    if (col >= NUM_COLS)
    {
        col = 0;
        if (row < NUM_ROWS - 1)
        {
            row++;
        }
        else
        {
            // Scroll up
            scroll_screen();
        }
    }
}

void print_char(char character)
{
    if (character == '\n')
    {
        print_newline();
        update_cursor();
        return;
    }
    else if (character == '\b')
    {
        handle_backspace();
        update_cursor();
        return;
    }
    else if (character == '\t')
    {
        size_t spaces = 4 - (col % 4);
        for (size_t i = 0; i < spaces; i++)
        {
            if (col >= NUM_COLS)
            {
                handle_wrap();
            }

            buffer[col + NUM_COLS * row] = (struct Char){
                .character = ' ',
                .color = color,
            };
            col++;
        }
        update_cursor();
        return;
    }
    else if (character == '\r')
    {
        col = 0;
        update_cursor();
        return;
    }

    if (col >= NUM_COLS)
    {
        handle_wrap();
    }

    // Place the character
    buffer[col + NUM_COLS * row] = (struct Char){
        .character = (uint8_t)character,
        .color = color,
    };

    col++;

    if (col >= NUM_COLS)
    {
        handle_wrap();
    }

    update_cursor();
}

void print_str(const char *str)
{
    for (size_t i = 0; str[i] != '\0'; i++)
    {
        print_char(str[i]);
    }
}

void print_str_color(const char *str, uint8_t foreground, uint8_t background)
{
    uint8_t previous_color = color;
    print_set_color(foreground, background);
    print_str(str);
    color = previous_color;
}

void print_set_color(uint8_t foreground, uint8_t background)
{
    color = foreground + (background << 4);
}

void print_number_kb(uint64_t bytes)
{
    char buf[32];
    uint64_t kb = bytes / 1024;
    int i = 0;

    if (kb == 0)
    {
        print_str("0 KB");
        return;
    }

    do
    {
        buf[i++] = '0' + (kb % 10);
        kb /= 10;
    } while (kb > 0 && i < 31);

    while (--i >= 0)
    {
        print_char(buf[i]);
    }
    print_str(" KB");
}

void print_hex(uint32_t value)
{
    const char hex_chars[] = "0123456789ABCDEF";
    char buffer[9]; // 8 digits + null terminator
    buffer[8] = '\0';

    // Fill buffer with hex representation
    for (int i = 7; i >= 0; i--)
    {
        buffer[i] = hex_chars[value & 0xF];
        value >>= 4;
    }

    print_str(buffer);
}

void print_memory_info()
{
    memory_info_t info;
    memory_get_info(&info);

    print_str("Memory Info:\n");
    print_str("  TOTAL: ");
    print_number_kb(info.total_memory);
    print_str("\n");

    print_str("  FREE: ");
    print_number_kb(info.free_memory);
    print_str("\n");

    print_str("  USED: ");
    print_number_kb(info.used_memory);
    print_str("\n");
}