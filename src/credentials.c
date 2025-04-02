#include "credentials.h"
#include "api/register.h"
#include "terminal.h"
#include <stdio.h>

Credentials credentials_create_empty(void)
{
    Credentials credentials = {
        .name = string_create_empty(),
        .password = string_create_empty()
    };
    return credentials;
}

void credentials_delete(Credentials* credentials)
{
    string_deallocate(&credentials->name);
    string_deallocate(&credentials->password);
}

void credentials_read_password(Credentials* credentials)
{
    if (credentials->password.size)
        string_deallocate(&credentials->password);

    while (!credentials->password.size)
    {
        printf(COLOR_OFFER "Enter password:\n" COLOR_CLEAN);
        credentials->password = read_string_from_console();
    }
}

void credentials_read_full(Credentials* credentials)
{
    if (credentials->name.size)
        string_deallocate(&credentials->name);

    while (!credentials->name.size)
    {
        printf(COLOR_OFFER "Enter name:\n" COLOR_CLEAN);
        credentials->name = read_string_from_console();
    }
    credentials_read_password(credentials);
}

Credentials terminal_register(void)
{
    Credentials credentials = credentials_create_empty();
    credentials_read_full(&credentials);

    Register_status status = rs_name_already_exists;
    while (status)
    {
        status = rpmc_register(&credentials.name, &credentials.password);

        switch (status) {
            case rs_name_already_exists:
            case rs_name_incorrect_format:
                printf(COLOR_ERROR "\nLogin: %s\n" COLOR_CLEAN, register_status_to_const_char(status));
                credentials_read_full(&credentials);
                break;

            case rs_password_incorrect_format:
                printf(COLOR_ERROR "\nPassword: %s\n" COLOR_CLEAN, register_status_to_const_char(status));
                credentials_read_password(&credentials);
                break;

            default:
                break;
        }
    }
    printf(COLOR_SUCCESS "\nRegistration successful\n" COLOR_CLEAN);
    return credentials;
}
