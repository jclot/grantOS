#include "driver_manager.h"
#include "print.h"
#include "string.h" // String comparison utility

#define MAX_DRIVERS 32

static driver_module_t *registered_drivers[MAX_DRIVERS];
static int driver_count = 0;

int driver_manager_init(void)
{
    // Initialize the driver registry
    for (int i = 0; i < MAX_DRIVERS; i++)
    {
        registered_drivers[i] = NULL;
    }
    driver_count = 0;
    return 0;
}

void driver_list_all(void)
{
    if (driver_count == 0)
    {
        print_str("  No drivers loaded.\n");
        return;
    }

    for (int i = 0; i < driver_count; i++)
    {
        if (registered_drivers[i])
        {
            print_str("  - ");
            print_str(registered_drivers[i]->name);
            print_str(" (v");
            print_str(registered_drivers[i]->version);
            print_str(") - ");

            switch (registered_drivers[i]->status)
            {
            case DRIVER_STATUS_INITIALIZED:
                print_str("READY");
                break;
            case DRIVER_STATUS_LOADED:
                print_str("LOADED");
                break;
            case DRIVER_STATUS_UNLOADED:
                print_str("UNLOADED");
                break;
            case DRIVER_STATUS_ERROR:
                print_str("ERROR");
                break;
            default:
                print_str("UNKNOWN");
            }
            print_char('\n');
        }
    }
}

int driver_register(driver_module_t *driver)
{
    if (!driver || driver_count >= MAX_DRIVERS)
    {
        return -1;
    }

    // Verify if a driver with the same name is already registered
    for (int i = 0; i < driver_count; i++)
    {
        if (registered_drivers[i] &&
            str_compare(registered_drivers[i]->name, driver->name) == 0)
        {
            return -2; // Driver already registered
        }
    }

    registered_drivers[driver_count] = driver;
    driver->status = DRIVER_STATUS_LOADED;
    driver_count++;

    return 0;
}

int driver_unregister(const char *name)
{
    for (int i = 0; i < driver_count; i++)
    {
        if (registered_drivers[i] &&
            str_compare(registered_drivers[i]->name, name) == 0)
        {

            if (registered_drivers[i]->cleanup)
            {
                registered_drivers[i]->cleanup();
            }

            for (int j = i; j < driver_count - 1; j++)
            {
                registered_drivers[j] = registered_drivers[j + 1];
            }

            driver_count--;
            registered_drivers[driver_count] = NULL;
            return 0;
        }
    }
    return -1;
}

driver_module_t *driver_get(const char *name)
{
    for (int i = 0; i < driver_count; i++)
    {
        if (registered_drivers[i] &&
            str_compare(registered_drivers[i]->name, name) == 0)
        {
            return registered_drivers[i];
        }
    }
    return NULL;
}

int driver_initialize(const char *name)
{
    driver_module_t *driver = driver_get(name);
    if (!driver)
    {
        return -1;
    }

    if (driver->status != DRIVER_STATUS_LOADED)
    {
        return -2;
    }

    if (driver->init)
    {
        int result = driver->init();
        if (result == 0)
        {
            driver->status = DRIVER_STATUS_INITIALIZED;
        }
        else
        {
            driver->status = DRIVER_STATUS_ERROR;
        }
        return result;
    }

    return 0;
}

int driver_cleanup(const char *name)
{
    driver_module_t *driver = driver_get(name);
    if (!driver)
    {
        return -1;
    }

    if (driver->cleanup)
    {
        int result = driver->cleanup();
        if (result == 0)
        {
            driver->status = DRIVER_STATUS_LOADED;
        }
        return result;
    }

    return 0;
}
