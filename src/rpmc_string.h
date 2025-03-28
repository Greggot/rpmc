#pragma once

#include <stddef.h>

typedef struct {
	char* ptr;
	size_t size;
} String;

typedef struct {
	const char* ptr;
	size_t size;
} String_view;

String string_create_empty(void);
String string_create_from_const_char(const char*);
void string_deallocate(String *string);

String_view string_view_create_from_char(const char*);

int string_is_equal_to_const_char(const String *string, const char *buffer);
int string_view_is_equal_to_string_view(const String_view*, const String_view*);

/// @brief Считать пользовательский ввод из консоли, скопировать
/// в строку. Мб можно будет потом использовать один буфер и 
/// сделать структуру вроде std::string_view
String read_string_from_console(void);
