#include "contacts.h"
#include "api/users.h"
#include "chat.h"
#include "dialog.h"
#include "rpmc_string.h"
#include "session.h"
#include "terminal.h"
#include <stdio.h>
#include <stdlib.h>

static void output_users(User_list* head)
{
    User_list* current = head;
    while (current) {
        if (current->user->status == us_online) {
            printf(COLOR_USER_ONLINE "O " COLOR_CLEAN COLOR_USER_NAME "[%s]\n" COLOR_CLEAN, current->user->name);
        } else {
            printf(COLOR_USER_OFFLINE "O " COLOR_CLEAN COLOR_USER_NAME "[%s]\n" COLOR_CLEAN, current->user->name);
        }
        current = current->next;
    }
    user_list_delete(head);
}

static void output_online_users(User_list* head)
{
    User_list* current = head;
    while (current) {
        if (current->user->status == us_online) {
            printf(COLOR_USER_NAME "[%s]\n" COLOR_CLEAN, current->user->name);
        }
        current = current->next;
    }
    user_list_delete(head);
}

static void output_friends_list(void)
{
    system("clear");
    printf(COLOR_CHAT_HEADER "FRIEND list\n" COLOR_CLEAN);
    User_list_iterator list = rpmc_receive_users(user_session_id(), ruf_friends);
    if (list.head == NULL) {
        printf(COLOR_OFFER "There are no friends yet\n" COLOR_CLEAN);
    } else {
        output_users(list.head);
    }
}

static void output_online_list(void)
{
    system("clear");
    printf(COLOR_CHAT_HEADER "Users ONLINE\n" COLOR_CLEAN);
    User_list_iterator list = rpmc_receive_users(user_session_id(), ruf_friends);
    if (list.head == NULL) {
        printf(COLOR_OFFER "...\n" COLOR_CLEAN);
    } else {
        output_online_users(list.head);
    }
}

static void enter_chat(int i)
{
    User_list_iterator list = rpmc_receive_users(user_session_id(), ruf_friends);
    terminal_user_chat_dialog(list.head->user);
}

typedef enum {
    contacts_exit,
    contacts_list_all,
    contacts_list_online,
    contacts_list_friends,
    contacts_enter_chat
} Contacts_dialog;

void terminal_contacts_list(void)
{
    output_friends_list();
    const String_view string_views[] = {
        string_view_create_from_char("back"),
        string_view_create_from_char("all"),
        string_view_create_from_char("online"),
        string_view_create_from_char("friends"),
        string_view_create_from_char("chat"),
    };

    Contacts_dialog action = dialog_read_action(string_views, sizeof(string_views) / sizeof(String_view));
    while (action) {
        switch (action)
        {
            case contacts_list_online:
                output_online_list();
                break;

            case contacts_enter_chat:
                enter_chat(0);
                output_friends_list();
                break;

            default:
                output_friends_list();
                break;
        }

        action = dialog_read_action(string_views, sizeof(string_views) / sizeof(String_view));
    }
    user_session_delete();
}
