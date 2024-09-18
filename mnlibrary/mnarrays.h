#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char *data;
    size_t capacity;
    size_t count;
} mnarray;
#define ARRAY_INIT_CAP 1000

void mnarray_init(mnarray *arr);
void mnarray_init_with_capacity(mnarray *arr, size_t capacity);
void mnarray_finalize(mnarray *arr);
size_t mnarray_add(mnarray *arr, void *item, size_t item_size);
size_t mnarray_add_int(mnarray *arr, int item);
size_t mnarray_add_cstr(mnarray *arr, const char *item);
size_t mnarray_add_bool(mnarray *arr, char item);

typedef struct {
    char **data;
    int capacity;
    int count;
} mncstr_array;

void mncstr_array_init(mncstr_array *arr);
void mncstr_array_add_with_size(mncstr_array *arr, const char *buff,
                                size_t buff_size);
void mncstr_array_add(mncstr_array *arr, const char *str);
void mncstr_array_finalize(mncstr_array *arr);

#ifdef __cplusplus
}
#endif
