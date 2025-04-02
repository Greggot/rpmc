#include "dialog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int dialog_read_action(const String_view* string_views, size_t size)
{
    char buffer[128];
    fgets(buffer, 128, stdin);
    /// Чтобы не считать длину строкти каждый вызов сравнения
    String_view buffer_view = string_view_create_from_char(buffer);
    --buffer_view.size;

    for (size_t i = 0; i < size; ++i)
        if (string_view_is_equal_to_string_view(&string_views[i], &buffer_view))
            return i;
    return -1;
}

static void string_copy_from_buffer(String* string, const char* buffer)
{
    string->size = strlen(buffer) - 1;
    if (string->size == 0)
        return;
    string->ptr = (char*)malloc(string->size + 1);
    memcpy(string->ptr, buffer, string->size);
    string->ptr[string->size] = 0;
}

String dialog_read_string_from_console(void)
{
    static char buffer[128];
    String string = string_create_empty();
    if (fgets(buffer, 128, stdin) == NULL)
        return string;
    string_copy_from_buffer(&string, buffer);
    return string;
}
