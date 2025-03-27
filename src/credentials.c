#include "credentials.h"
#include "api/register.h"
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
        printf("Enter password:\n");
        credentials->password = read_string_from_console();
    }
}

void credentials_read_full(Credentials* credentials)
{
    if (credentials->name.size)
        string_deallocate(&credentials->name);

    while (!credentials->name.size)
    {
        printf("Enter name:\n");
        credentials->name = read_string_from_console();
    }
    credentials_read_password(credentials);
}

Credentials terminal_register(void)
{
    Credentials credentials = credentials_create_empty();
    printf("Registration credentials.\n");
    credentials_read_full(&credentials);

    Register_status status = rs_name_already_exists;
    while (status)
    {
        status = rpmc_register(&credentials.name, &credentials.password);

        switch (status) {
            case rs_name_already_exists:
            case rs_name_incorrect_format:
                printf("\nLogin: %s\n", register_status_to_const_char(status));
                credentials_read_full(&credentials);
                break;

            case rs_password_incorrect_format:
                printf("\nPassword: %s\n", register_status_to_const_char(status));
                credentials_read_password(&credentials);
                break;

            default:
                break;
        }
    }
    printf("Registration successful\n");
    return credentials;
}
