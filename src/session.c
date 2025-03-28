#include "session.h"
#include "api/log_in.h"
#include "rpmc_string.h"
#include <stdio.h>

static long session_id = 0;

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
        printf("Enter password:\n");
        credentials->password = read_string_from_console();
    }
}

static void log_in_credentials_read_full(Log_in_credentials* credentials)
{
    if (credentials->login.size)
        string_deallocate(&credentials->login);

    while (!credentials->login.size)
    {
        printf("Enter name:\n");
        credentials->login = read_string_from_console();
    }
    log_in_credentials_read_password(credentials);
}

void terminal_log_in(void)
{
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
    printf("Logged in as \"%s\"\n", log_in.login.ptr);
}
