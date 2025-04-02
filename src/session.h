#pragma once

#include "rpmc_string.h"

long user_session_id(void);
void user_sesion_id_set(long);

void user_session_delete(void);
String user_session_name(void);

void log_out(void);
void terminal_log_in(void);
