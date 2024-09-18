#include "mnarrays.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



// Initialize array with specified capacity.
void mnarray_init_with_capacity(mnarray *arr, size_t capacity) {
    arr->data = malloc(capacity);
    if (arr->data == NULL) {
        perror("Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    arr->capacity = capacity;
    arr->count = 0;
}

// Initialize array with default capacity.
void mnarray_init(mnarray *arr) {
    mnarray_init_with_capacity(arr, ARRAY_INIT_CAP);
}

// Add an item to the array.
size_t mnarray_add(mnarray *arr, void *item, size_t item_size) {
    size_t result = arr->count;
    if (arr->count + item_size > arr->capacity) {
        arr->capacity = (arr->capacity + item_size) * 2;
        void *new_data = realloc(arr->data, arr->capacity);
        if (new_data == NULL) {
            perror("Memory reallocation failed.");
            exit(EXIT_FAILURE);
        }
        arr->data = new_data;
    }
    memcpy(arr->data + arr->count, item, item_size);
    arr->count += item_size;
    return result;
}

// Free memory used by the array.
void mnarray_finalize(mnarray *arr) {
    free(arr->data);
    arr->data = NULL;
    arr->capacity = 0;
    arr->count = 0;
}

// Add an integer to the array.
size_t mnarray_add_int(mnarray *arr, int item) {
    return mnarray_add(arr, &item, sizeof(int));
}

// Add a C-style string to the array.
size_t mnarray_add_cstr(mnarray *arr, const char *item) {
    return mnarray_add(arr, (void*)item, strlen(item) + 1);
}

// Add a boolean value to the array.
size_t mnarray_add_bool(mnarray *arr, char item) {
    return mnarray_add(arr, &item, 1);
}








void mncstr_array_init(mncstr_array* arr) {
    arr->capacity = ARRAY_INIT_CAP;
    arr->count = 0;
    arr->data = (char**)malloc(arr->capacity * sizeof(char*));
}

void mncstr_array_add_with_size(mncstr_array* arr, const char* buff,size_t buff_size) {
    if (arr->count == arr->capacity) {
        arr->capacity *= 2;
        arr->data = (char**)realloc(arr->data, arr->capacity * sizeof(char*));
    }
    char* s= malloc(buff_size);
    memcpy(s,buff,buff_size);
    arr->data[arr->count++]=s;
}

void mncstr_array_add(mncstr_array* arr, const char* str) {
    if (arr->count == arr->capacity) {
        arr->capacity *= 2;
        arr->data = (char**)realloc(arr->data, arr->capacity * sizeof(char*));
    }
    arr->data[arr->count++] = strdup(str);
}

void mncstr_array_finalize(mncstr_array* arr) {
    for (int i = 0; i < arr->count; i++) {
        free(arr->data[i]);
    }
    free(arr->data);
    arr->data = NULL;
    arr->capacity = 0;
    arr->count = 0;
}
