#pragma once

#include <stddef.h>

typedef struct {
	char* ptr;
	size_t size;
} String;

String string_create_empty(void);
void string_deallocate(String *string);

int string_is_equal_to_const_char(const String *string, const char *buffer);

/// @brief Считать пользовательский ввод из консоли, скопировать
/// в строку. Мб можно будет потом использовать один буфер и 
/// сделать структуру вроде std::string_view
String read_string_from_console(void);
