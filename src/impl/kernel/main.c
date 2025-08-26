#include "print.h"
#include "keyboard.h"
#include "string.h"

#define MAX_INPUT_LEN 256

void kernel_main()
{
    print_clear();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
    print_str_color(" Welcome to GrantOS Console!\n", PRINT_COLOR_LIGHT_GREEN, PRINT_COLOR_BLACK);
    print_str("> ");

    char input_buffer[MAX_INPUT_LEN];
    size_t input_index = 0;

    while (1)
    {
        char c = keyboard_read_char();
        if (c != 0)
        {
            if (c == '\n')
            {
                print_char('\n');
                input_buffer[input_index] = '\0';
                print_str("Command: ");
                if (str_compare(input_buffer, "clear") == 0)
                {
                    print_clear();
                    print_char('\n');
                    print_str("> ");
                }
                else if (str_compare(input_buffer, "help") == 0)
                {
                    print_str("Available commands:\n");
                    print_str("  clear - Clear the screen\n");
                    print_str("  help  - Show this help message\n");
                    print_str("> ");
                }
                else
                {
                    print_str(input_buffer);
                    print_str("\n> ");
                }
                input_index = 0;
            }
            else if (c == '\b')
            {
                if (input_index > 0)
                {
                    input_index--;
                    print_char('\b');
                }
            }
            else
            {
                if (input_index < MAX_INPUT_LEN - 1)
                {
                    input_buffer[input_index++] = c;
                    print_char(c);
                }
            }
        }
    }
}
