#include "contacts.h"
#include "credentials.h"
#include "dialog.h"
#include "rpmc_string.h"
#include "session.h"
#include "terminal.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    main_dialog_log_out,
    main_dialog_log_in,
    main_dialog_register,
    main_dialog_users,
    main_dialog_help
} Main_dialog;

/// @brief Перехватывает событие пользовательского прерывания программы как в Python
void ctrl_c_handler(int sig)
{
    signal(sig, SIG_IGN);
    printf("\rUse \"exit\" to terminate session\n");
    signal(SIGINT, ctrl_c_handler);
}

/// @todo Получение имени пользователя по session
void main_page_print(void)
{
    system("clear");
    const String username = user_session_name();
    if (username.ptr) {
        printf("RPMChat main (" COLOR_USER_NAME "%s" COLOR_CLEAN ")\n", username.ptr);
    } else {
        printf("RPMChat main ( )\n");
    }
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
        string_view_create_from_char("help")
    };

    main_page_print();
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

            case main_dialog_users:
                terminal_contacts_list();
                break;

            default:
                break;
        }
        main_page_print();
        action = dialog_read_action(string_views, size);
    }
    log_out();

    return 0;
}
