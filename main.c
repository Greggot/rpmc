#include "credentials.h"
#include "rpmc_string.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    action_log_out,
    action_log_in,
    action_register,
    action_send_message,
    action_display_users,
    action_display_user_messages,
    action_help,
    action_unknown
} Action;

/// @todo Сделать string_views статическими или передавать из main 
/// в качестве параметра
Action read_action(void)
{
    const String_view string_views[] = {
        string_view_create_from_char("exit"),
        string_view_create_from_char("login"),
        string_view_create_from_char("register"),
        string_view_create_from_char("send_message"),
        string_view_create_from_char("users"),
        string_view_create_from_char("messages"),
        string_view_create_from_char("help")
    };

    char buffer[128];
    fgets(buffer, 128, stdin);

    for (size_t i = 0; i < sizeof(string_views) / sizeof(String_view); ++i)
        if (string_view_is_equal_to_const_char(&string_views[i], buffer))
            return i;
    return action_unknown;
}

int main(void)
{
    Action action = read_action();
    while (action) {
        switch (action)
        {
            case action_register: {
                Credentials credentials = terminal_register();
                credentials_delete(&credentials);
            } break;

            default:
                break;
        }
        action = read_action();
    }
    return 0;
}
