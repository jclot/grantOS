#include "print.h"
#include "drivers/keyboard.h"
#include "drivers/driver_manager.h"
#include "drivers/keyboard/keyboard_driver.h"
#include "string.h"
#include "memory/memory.h"
#include "io.h"

#define MAX_INPUT_LEN 256
#define MEMORY_SIZE (16 * 1024 * 1024)
#define PROMPT "> "

// Safely reset input buffer and index
static inline void reset_input(char *buffer, size_t *index)
{
    buffer[0] = '\0';
    *index = 0;
}

// Handle command processing
static void process_command(const char *input_buffer)
{
    if (!input_buffer)
    {
        print_str("Error: Invalid command\n");
        return;
    }

    if (input_buffer[0] == '\0')
    {
        return;
    }

    // print_str("Command: ");
    // print_str(input_buffer);
    // print_char('\n');

    if (str_compare(input_buffer, "clear") == 0)
    {
        print_clear();
    }
    else if (str_compare(input_buffer, "help") == 0)
    {
        print_str_color("Available commands:\n", PRINT_COLOR_LIGHT_GREEN, PRINT_COLOR_BLACK);
        print_str_color("  clear   - Clear the screen\n", PRINT_COLOR_LIGHT_GREEN, PRINT_COLOR_BLACK);
        print_str_color("  help    - Show this help message\n", PRINT_COLOR_LIGHT_GREEN, PRINT_COLOR_BLACK);
        print_str_color("  meminfo - Show memory information\n", PRINT_COLOR_LIGHT_GREEN, PRINT_COLOR_BLACK);
        print_str_color("  version - Show OS version\n", PRINT_COLOR_LIGHT_GREEN, PRINT_COLOR_BLACK);
        print_str_color("  drivers   - List loaded drivers\n", PRINT_COLOR_LIGHT_GREEN, PRINT_COLOR_BLACK);
        print_str_color("  drvinfo   - Show driver information\n", PRINT_COLOR_LIGHT_GREEN, PRINT_COLOR_BLACK);
        print_str_color("  exit    - Exit the console\n", PRINT_COLOR_LIGHT_GREEN, PRINT_COLOR_BLACK);
    }
    else if (str_compare(input_buffer, "meminfo") == 0)
    {
        print_memory_info();
    }
    else if (str_compare(input_buffer, "version") == 0)
    {
        print_str("GrantOS Version 1.0.0\n");
    }
    else if (str_compare(input_buffer, "drivers") == 0)
    {
        print_str_color("Loaded Drivers:\n", PRINT_COLOR_LIGHT_CYAN, PRINT_COLOR_BLACK);
        driver_list_all();
    }
    else if (str_compare(input_buffer, "drvinfo") == 0)
    {
        // Show detailed info about the keyboard driver as an example
        driver_module_t *kbd_driver = driver_get("keyboard");
        if (kbd_driver)
        {
            print_str_color("Keyboard Driver Information:\n", PRINT_COLOR_LIGHT_CYAN, PRINT_COLOR_BLACK);
            print_str("  Name: ");
            print_str(kbd_driver->name);
            print_char('\n');
            print_str("  Version: ");
            print_str(kbd_driver->version);
            print_char('\n');
            print_str("  Type: INPUT (0x");
            print_hex(kbd_driver->type);
            print_str(")\n");
            print_str("  Status: ");

            switch (kbd_driver->status)
            {
            case DRIVER_STATUS_UNLOADED:
                print_str_color("UNLOADED", PRINT_COLOR_LIGHT_RED, PRINT_COLOR_BLACK);
                break;
            case DRIVER_STATUS_LOADED:
                print_str_color("LOADED", PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
                break;
            case DRIVER_STATUS_INITIALIZED:
                print_str_color("INITIALIZED", PRINT_COLOR_LIGHT_GREEN, PRINT_COLOR_BLACK);
                break;
            case DRIVER_STATUS_ERROR:
                print_str_color("ERROR", PRINT_COLOR_LIGHT_RED, PRINT_COLOR_BLACK);
                break;
            default:
                print_str("UNKNOWN");
            }
            print_char('\n');

            if (kbd_driver->driver_ops)
            {
                keyboard_ops_t *ops = (keyboard_ops_t *)kbd_driver->driver_ops;
                if (ops->get_status)
                {
                    int kbd_status = ops->get_status();
                    print_str("  Hardware Status: ");
                    if (kbd_status)
                    {
                        print_str_color("READY", PRINT_COLOR_LIGHT_GREEN, PRINT_COLOR_BLACK);
                    }
                    else
                    {
                        print_str_color("NOT READY", PRINT_COLOR_LIGHT_RED, PRINT_COLOR_BLACK);
                    }
                    print_char('\n');
                }
            }
        }
        else
        {
            print_str_color("Keyboard driver not found!\n", PRINT_COLOR_LIGHT_RED, PRINT_COLOR_BLACK);
        }
    }
    else if (str_compare(input_buffer, "06/10/2006") == 0)
    {
        print_str("I \x03 you!\n");
    }
    else if (str_compare(input_buffer, "exit") == 0)
    {
        // Cleanup and exit
        driver_cleanup("keyboard");

        print_str("Exiting console...\n");
        print_str("System will shutdown in QEMU...\n");
        qemu_shutdown();
    }
    else
    {
        print_str("Unknown command: '");
        print_str(input_buffer);
        print_str("'. Type 'help' for available commands.\n");
    }
}

static void show_system_startup_info(void)
{
    print_str_color("\nGrantOS Kernel Starting...\n", PRINT_COLOR_LIGHT_CYAN, PRINT_COLOR_BLACK);
    print_str("Initializing memory system... ");
    memory_init(MEMORY_SIZE);
    print_str_color("[OK]\n", PRINT_COLOR_LIGHT_GREEN, PRINT_COLOR_BLACK);

    print_str("Initializing driver manager... ");
    if (driver_manager_init() == 0)
    {
        print_str_color("[OK]\n", PRINT_COLOR_LIGHT_GREEN, PRINT_COLOR_BLACK);
    }
    else
    {
        print_str_color("[FAILED]\n", PRINT_COLOR_LIGHT_RED, PRINT_COLOR_BLACK);
        return;
    }

    print_str("Registering keyboard driver... ");
    if (keyboard_driver_register() == 0)
    {
        print_str_color("[OK]\n", PRINT_COLOR_LIGHT_GREEN, PRINT_COLOR_BLACK);
    }
    else
    {
        print_str_color("[FAILED]\n", PRINT_COLOR_LIGHT_RED, PRINT_COLOR_BLACK);
        return;
    }

    print_str("Initializing keyboard driver... ");
    if (driver_initialize("keyboard") == 0)
    {
        print_str_color("[OK]\n", PRINT_COLOR_LIGHT_GREEN, PRINT_COLOR_BLACK);
    }
    else
    {
        print_str_color("[FAILED]\n", PRINT_COLOR_LIGHT_RED, PRINT_COLOR_BLACK);
        return;
    }

    print_str_color("System initialization complete!\n\n", PRINT_COLOR_LIGHT_GREEN, PRINT_COLOR_BLACK);
}

void kernel_main()
{
    print_pinned_header();
    show_system_startup_info();
    print_str(PROMPT);

    char input_buffer[MAX_INPUT_LEN];
    size_t input_index = 0;
    int running = 1;

    reset_input(input_buffer, &input_index);

    while (running)
    {
        char c = keyboard_read_char();

        if (c != 0)
        {
            switch (c)
            {
            case '\n':
                print_char('\n');
                // Null-terminate the string safely
                input_buffer[input_index] = '\0';

                // Process the command
                process_command(input_buffer);

                // Reset for next command
                reset_input(input_buffer, &input_index);
                print_str(PROMPT);
                break;

            case '\b':
                // Handle backspace - only if there's something to delete
                if (input_index > 0)
                {
                    input_index--;
                    input_buffer[input_index] = '\0'; // Clear the character
                    print_char('\b');
                }
                break;

            case '\t':
                // Ignore tab for now (could implement auto-completion later)
                break;

            case '\0':
                // Ignore null characters
                break;

            default:
                // Regular character input
                // buffer overflow protection
                if (input_index < MAX_INPUT_LEN - 1)
                {
                    if (c >= 32 && c <= 126)
                    { // Printable ASCII range
                        input_buffer[input_index] = c;
                        input_index++;
                        input_buffer[input_index] = '\0'; // Keep string null-terminated
                        print_char(c);
                    }
                }
                else
                {
                    print_str_color("\n[Buffer full! Press Enter to execute or Backspace to edit]\n",
                                    PRINT_COLOR_LIGHT_RED, PRINT_COLOR_BLACK);
                    print_str(PROMPT);
                    print_str(input_buffer);
                }
                break;
            }
        }

        // Small delay to prevent excessive CPU usage in tight loop
        // This is a simple busy-wait, in a real OS you'd use proper scheduling
        for (volatile int i = 0; i < 1000; i++)
            ;
    }

    print_str_color("\nConsole exited. System halting...\n", PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);

    // Infinite halt loop for system shutdown
    while (1)
    {
        asm volatile("cli; hlt" : : : "memory");
    }
}