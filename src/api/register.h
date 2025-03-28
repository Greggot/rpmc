#pragma once

#include "rpmc_string.h"

typedef enum {
	rs_ok = 0,
	rs_name_already_exists = 1,
	rs_name_incorrect_format = 2,
	rs_password_incorrect_format = 3
} Register_status;

/// @brief Перевод ошибки в читаемый для пользователя вид
const char* register_status_to_const_char(Register_status status);

/// @todo Добавить вызов стаба
Register_status rpmc_register(const String* name, const String* password);
