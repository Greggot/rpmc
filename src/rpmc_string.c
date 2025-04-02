#include "rpmc_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

String string_create_empty(void)
{
    String string = { .ptr = NULL, .size = 0 };
    return string;
}

String string_create_from_const_char(const char* buffer)
{
    String string = { 
        .size = strlen(buffer) 
    };
    string.ptr = (char*)malloc(string.size + 1);
    memcpy(string.ptr, buffer, string.size);
    string.ptr[string.size] = 0;
    return string;
}

int string_is_equal_to_const_char(const String* string, const char* buffer)
{
    const size_t buffer_size = strlen(buffer);
    if (buffer_size != string->size)
        return 0;

    char* string_ptr = string->ptr;
    for (size_t i = 0; i < buffer_size; ++i, ++buffer, ++string_ptr)
        if (*buffer != *string_ptr)
            return 0;
    return 1;
}

void string_deallocate(String* string)
{
    free(string->ptr);
    string->size = 0;
    string->ptr = NULL;
}

String_view string_view_create_from_char(const char* ptr)
{
    String_view string_view = {
        .ptr = ptr,
        .size = strlen(ptr)
    };
    return string_view;
}

int string_view_is_equal_to_string_view(const String_view* right, const String_view* left)
{
    if (left->size != right->size) {
        return 0;
    }
    const char* lptr = left->ptr;
    const char* rptr = right->ptr;
    for (size_t i = 0; i < left->size; ++i, ++lptr, ++rptr)
        if (*lptr != *rptr)
            return 0;
    return 1;
}
