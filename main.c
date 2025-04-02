#include "chat.h"
#include "credentials.h"
#include "dialog.h"
#include "rpmc_string.h"
#include "session.h"
#include <signal.h>
#include <stdio.h>

typedef enum {
    main_dialog_log_out,
    main_dialog_log_in,
    main_dialog_register,
    main_dialog_users,
    main_dialog_test_chat,
    main_dialog_help
} Main_dialog;

/// @brief Перехватывает событие пользовательского прерывания программы как в Python
void ctrl_c_handler(int sig)
{
    signal(sig, SIG_IGN);
    printf("\rUse \"exit\" to terminate session\n");
    signal(SIGINT, ctrl_c_handler);
}

int main(void)
{
    // signal(SIGINT, ctrl_c_handler);

    /// Строковые константы с заранее посчитанной длиной,
    const String_view string_views[] = {
        string_view_create_from_char("exit"),
        string_view_create_from_char("login"),
        string_view_create_from_char("register"),
        string_view_create_from_char("users"),
        string_view_create_from_char("chat"),
        string_view_create_from_char("help")
    };
    const size_t size = sizeof(string_views) / sizeof(String_view);
    Main_dialog action = dialog_read_action(string_views, size);

    while (action) {
        switch (action)
        {
            case main_dialog_log_in:
                terminal_log_in();
                break;

            case main_dialog_register:
                terminal_register();
                break;

            /// @todo Убрать, когда будет готов диалог со списком пользователей,
            /// из которого уже можно будет открывать чат с кем-либо
            case main_dialog_test_chat:
                terminal_user_chat_dialog(1);
                break;

            default:
                break;
        }
        action = dialog_read_action(string_views, size);
    }
    log_out();

    return 0;
}
