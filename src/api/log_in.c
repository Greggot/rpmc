#include "log_in.h"

Log_in_token rpmc_log_in(const String* name, const String* password, const String* device)
{
    Log_in_token token = {
        .id = 0,
        .log_in_status = lis_ok
    };

    if(name->size == 0) {
        token.log_in_status = lis_incorrect_login;
    }
    if(password->size == 0) {
        token.log_in_status = lis_incorrect_password;
    }
    if(device->size == 0) {
        token.log_in_status = lis_incorrect_device;
    }
    return token;
}

const char* log_in_status_to_const_char(Log_in_status status)
{
    const char* statuses[4] = {
        "Ok",
        "Incorrect login",
        "Incorrect password",
        "Incorrect device"
    };
    return statuses[status];
}
