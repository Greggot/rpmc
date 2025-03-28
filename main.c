#include "credentials.h"
#include "rpmc_string.h"
#include "session.h"
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

Action read_action(const String_view* string_views, size_t size)
{
    char buffer[128];
    fgets(buffer, 128, stdin);
    /// Чтобы не считать длину строкти каждый вызов сравнения
    String_view buffer_view = string_view_create_from_char(buffer);
    --buffer_view.size;

    for (size_t i = 0; i < size; ++i)
        if (string_view_is_equal_to_string_view(&string_views[i], &buffer_view))
            return i;
    return action_unknown;
}

int main(void)
{
    /// Строковые константы с заранее посчитанной длиной,
    const String_view string_views[] = {
        string_view_create_from_char("exit"),
        string_view_create_from_char("login"),
        string_view_create_from_char("register"),
        string_view_create_from_char("send_message"),
        string_view_create_from_char("users"),
        string_view_create_from_char("messages"),
        string_view_create_from_char("help")
    };
    const size_t size = sizeof(string_views) / sizeof(String_view);
    Action action = read_action(string_views, size);

    while (action) {
        switch (action)
        {
            case action_log_in:
                terminal_log_in();
                break;

            case action_register: {
                Credentials credentials = terminal_register();
                credentials_delete(&credentials);
            } break;

            default:
                break;
        }
        action = read_action(string_views, size);
    }
    return 0;
}
