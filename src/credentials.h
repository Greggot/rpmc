#pragma once

#include "rpmc_string.h"

typedef struct {
	String name;
	String password;
} Credentials;

Credentials credentials_create_empty(void);
void credentials_delete(Credentials* credentials);

void credentials_read_password(Credentials* credentials);
void credentials_read_full(Credentials* credentials);

/// @brief Считать регистрацию из консоли
/// @todo Проверка логина и пароля на корректный формат
Credentials terminal_register(void);
