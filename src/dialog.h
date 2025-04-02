#pragma once

#include "rpmc_string.h"

int dialog_read_action(const String_view* string_views, size_t size);
String dialog_read_string_from_console(void);
