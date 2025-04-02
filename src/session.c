#include "session.h"
#include "api/log_in.h"
#include "api/log_out.h"
#include "dialog.h"
#include "rpmc_string.h"
#include "terminal.h"
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>

static long session_id = 0;
static String user_name = {
    .ptr = NULL,
    .size = 0
};

long user_session_id(void)
{
    return session_id;
}

void user_sesion_id_set(long new_id)
{
    session_id = new_id;
}

typedef struct {
    String login;
    String password;
    String device;
} Log_in_credentials;

/// @todo implement
static void log_in_credentials_get_device(Log_in_credentials* credentials)
{
    credentials->device = string_create_from_const_char("device");
}

static void log_in_credentials_read_password(Log_in_credentials* credentials)
{
    if (credentials->password.size)
        string_deallocate(&credentials->password);

    while (!credentials->password.size)
    {
        printf(COLOR_OFFER "Enter password:\n" COLOR_CLEAN);
        credentials->password = dialog_read_string_from_console();
    }
}

static void log_in_credentials_read_full(Log_in_credentials* credentials)
{
    if (credentials->login.size)
        string_deallocate(&credentials->login);

    while (!credentials->login.size)
    {
        printf(COLOR_OFFER "Enter name:\n" COLOR_CLEAN);
        credentials->login = dialog_read_string_from_console();
    }
    log_in_credentials_read_password(credentials);
}

void terminal_log_in(void)
{
    system("clear");
    Log_in_credentials log_in = {
        .login = string_create_empty(),
        .password = string_create_empty(),
        .device = string_create_empty()
    };
    log_in_credentials_get_device(&log_in);
    log_in_credentials_read_full(&log_in);

    Log_in_token token = {
        .id = 0,
        .log_in_status = lis_incorrect_login
    };

    while (token.log_in_status) {
        token = rpmc_log_in(&log_in.login, &log_in.password, &log_in.device);
        switch (token.log_in_status)
        {
            case lis_ok:
                user_sesion_id_set(token.id);
                break;

            case lis_incorrect_login:
                log_in_credentials_read_full(&log_in);
                break;

            case lis_incorrect_password:
                log_in_credentials_read_password(&log_in);
                break;

            case lis_incorrect_device:
                printf("There is active session already\n");
                break;
        }
    }
    printf("Logged in as: \"" COLOR_USER_NAME "%s" COLOR_CLEAN "\"\n", log_in.login.ptr);
    user_name = log_in.login;
    string_deallocate(&log_in.password); 
    string_deallocate(&log_in.device);
}

void log_out(void)
{
    long session = user_session_id();
    if (!session) {
        printf(COLOR_WARNING "Log out with no session\n" COLOR_CLEAN);
        return;
    }
    printf("Log out %zu\n", session);
    rpmc_log_out(session);
    user_sesion_id_set(0);
}

void user_session_delete(void)
{
    string_deallocate(&user_name);
}

String user_session_name(void)
{
    return user_name;
}
