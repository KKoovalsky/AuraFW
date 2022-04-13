/**
 * @file    custom_alloc.c
 * @brief   Custom malloc() and free() implementations which wrap FreeRTOS allocation functions.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include <stddef.h>

void* pvPortMalloc(size_t xSize);
void vPortFree(void* pv);

void* malloc(size_t size)
{
    return pvPortMalloc(size);
}

void free(void* ptr)
{
    vPortFree(ptr);
}

