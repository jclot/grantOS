#ifndef DRIVER_MANAGER_H
#define DRIVER_MANAGER_H

#include <stdint.h>
#include <stddef.h>

typedef enum
{
    DRIVER_STATUS_UNLOADED = 0,
    DRIVER_STATUS_LOADED = 1,
    DRIVER_STATUS_INITIALIZED = 2,
    DRIVER_STATUS_ERROR = -1
} driver_status_t;

typedef struct
{
    const char *name; // Name of the driver
    const char *version; // Version of the driver
    uint32_t type; // Type of the driver (e.g., 0 for character device, 1 for block device)
    driver_status_t status; // Current status of the driver

    int (*init)(void); // Pointer to the driver's initialization function
    int (*cleanup)(void); // Pointer to the driver's cleanup function
    int (*suspend)(void); // Pointer to the driver's suspend function
    int (*resume)(void); // Pointer to the driver's resume function

    void *driver_ops; // Pointer to driver-specific operations structure
} driver_module_t;

#define DRIVER_TYPE_INPUT 0x0001
#define DRIVER_TYPE_STORAGE 0x0002
#define DRIVER_TYPE_NETWORK 0x0004
#define DRIVER_TYPE_GRAPHICS 0x0008

int driver_manager_init();
int driver_register(driver_module_t *driver);
int driver_unregister(const char *name);
driver_module_t *driver_get(const char *name);
int driver_initialize(const char *name);
int driver_cleanup(const char *name);
void driver_list_all();

#endif // DRIVER_MANAGER_H