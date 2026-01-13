#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H

#include "../driver_manager.h"

typedef struct
{
    char (*read_char)(void);
    int (*set_layout)(int layout);
    int (*get_status)(void);
} keyboard_ops_t;

// Keyboard driver module
int keyboard_driver_init(void);
int keyboard_driver_cleanup(void);
char keyboard_read_char(void);

int keyboard_driver_register(void);

#endif