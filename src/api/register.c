#include "api/register.h"

const char* register_status_to_const_char(Register_status status)
{
    const char* statuses[4] = {
        "ok",
        "Name already exists",
        "Incorrect format",
        "Incorrect format"
    };
    return statuses[status];
}

Register_status rpmc_register(const String* name, const String* password)
{
    if (string_is_equal_to_const_char(name, "exists"))
        return rs_name_already_exists;

    if (string_is_equal_to_const_char(name, "incorrect"))
        return rs_name_incorrect_format;

    if (string_is_equal_to_const_char(password, "123"))
        return rs_password_incorrect_format;

    return rs_ok;
}
