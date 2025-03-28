#pragma once

#include "rpmc_string.h"

typedef enum {
	lis_ok = 0,
	lis_incorrect_login = 1,
	lis_incorrect_password = 2,
	lis_incorrect_device = 3 
} Log_in_status;

const char* log_in_status_to_const_char(Log_in_status status);

typedef struct {
	Log_in_status log_in_status;
	long id;
} Log_in_token;

/// @todo Добавить вызов стаба
Log_in_token rpmc_log_in(const String* name, const String* password, const String* device);
