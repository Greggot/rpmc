#include "chat.h"
#include "api/messages.h"
#include "dialog.h"
#include "rpmc_string.h"
#include "session.h"
#include "terminal.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

/// @todo Получить имя пользователя по user_id
static void output_chat(Message_list* head, long user_id)
{
    Message_list* current = head;
    while (current) {
        const char* sender = current->message->status == ms_sender ? COLOR_CHAT_OWN_NAME "(you)" : COLOR_USER_NAME "user";
        if (current->message->type == mt_text) {
            printf("%s: " COLOR_CLEAN "%s\n", sender, current->message->text.ptr);
        } else {
            printf("%s: " COLOR_CHAT_FILE "[%s]\n" COLOR_CLEAN, sender, current->message->text.ptr);
        }
        current = current->next;
    }
    message_list_delete(head);
}

static void output_files(Message_list* head)
{
    Message_list* current = head;
    while (current) {
        if (current->message->type == mt_file) {
            const char* sender = current->message->status == ms_sender ? COLOR_CHAT_OWN_NAME "(you)" : COLOR_USER_NAME "user";
            printf("%s: " COLOR_CHAT_FILE "[%s]\n" COLOR_CLEAN, sender, current->message->text.ptr);
        }
        current = current->next;
    }
    message_list_delete(head);
}

static void output_files_only(long user_id)
{
    system("clear");
    printf(COLOR_CHAT_HEADER "Media from " COLOR_USER_NAME "%zu\n" COLOR_CLEAN, user_id);
    Message_list_iterator list = rpmc_receive_messages_from_user(user_session_id(), user_id);
    if (list.head == NULL) {
        printf(COLOR_OFFER "There is no files yet\n" COLOR_CLEAN);
    } else {
        output_files(list.head);
    }
}

static void output_full_history(long user_id)
{
    system("clear");
    printf(COLOR_CHAT_HEADER "Chat with " COLOR_USER_NAME "%zu\n" COLOR_CLEAN, user_id);
    Message_list_iterator list = rpmc_receive_messages_from_user(user_session_id(), user_id);
    if (list.head == NULL) {
        printf(COLOR_OFFER "There is no messages yet\n" COLOR_CLEAN);
    } else {
        output_chat(list.head, user_id);
    }
}

typedef enum {
    user_files_exit,
    user_files_download
} User_files_dialog;

void terminal_user_files_dialog(long user_id)
{
    output_files_only(user_id);
    const String_view string_views[] = {
        string_view_create_from_char("back"),
        string_view_create_from_char("download"),
    };
    User_files_dialog action = dialog_read_action(string_views, sizeof(string_views) / sizeof(String_view));
    while (action) {
        action = dialog_read_action(string_views, sizeof(string_views) / sizeof(String_view));
    }
}

typedef enum {
    user_chat_exit,
    user_chat_new_message,
    user_chat_last_messages,
    user_chat_history,
    user_chat_files
} User_chat_dialog;

/// @todo Поток, который будет обновлять историю сообщений
/// @todo Показывать N последних сообщений в переписке
/// @todo Навигация с использованием getch
void terminal_user_chat_dialog(long user_id)
{
    output_full_history(user_id);

    const String_view string_views[] = {
        string_view_create_from_char("back"),
        string_view_create_from_char("new"),
        string_view_create_from_char("last"),
        string_view_create_from_char("history"),
        string_view_create_from_char("files"),
    };

    User_chat_dialog action = dialog_read_action(string_views, sizeof(string_views) / sizeof(String_view));
    while (action) {
        switch (action)
        {
            /// @todo Send message API
            case user_chat_new_message: {
                printf("\nEnter text:\n" COLOR_OFFER);
                String message = dialog_read_string_from_console();
                printf(COLOR_CLEAN "\n");
                string_deallocate(&message);
            } break;

            case user_chat_files:
                terminal_user_files_dialog(user_id);
                break;

            default:
                break;
        }
        
        output_full_history(user_id);
        action = dialog_read_action(string_views, sizeof(string_views) / sizeof(String_view));
    }
    user_session_delete();
}
