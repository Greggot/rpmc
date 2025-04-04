#include "contacts.h"
#include "api/users.h"
#include "chat.h"
#include "dialog.h"
#include "rpmc_string.h"
#include "session.h"
#include "terminal.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	User_list_iterator users;
	int selection;
} Selectable_user_list;

static Selectable_user_list user_list;

static void user_list_init(void) {
	user_list.selection = -1;
	user_list.users.head = NULL;
	user_list.users.current = NULL;
	user_list.users.size = 0;
}

static void user_list_clear(void) {
	if(user_list.users.head != NULL) 
		user_list_delete(user_list.users.head);
}

static void user_list_set_new(User_list_iterator users) 
{
	user_list_clear();
	user_list.selection = 0;
	user_list.users.head = users.head;
	user_list.users.current = users.current;
	user_list.users.size = users.size;
}

static void output_users(User_list* head)
{
    User_list* current = user_list.users.head;
	int selection = 0;
    while (current) {
		if(selection++ == user_list.selection)
			printf(">> ");
        if (current->user->status == us_online) {
            printf(COLOR_USER_ONLINE "O " COLOR_CLEAN COLOR_USER_NAME "[%s]\n" COLOR_CLEAN, current->user->name);
        } else {
            printf(COLOR_USER_OFFLINE "O " COLOR_CLEAN COLOR_USER_NAME "[%s]\n" COLOR_CLEAN, current->user->name);
        }
        current = current->next;
    }
}

static void output_online_users(User_list* head)
{
    User_list* current = user_list.users.head;
	int selection = 0;
    while (current) {
		if(selection++ == user_list.selection)
			printf(">> ");
        if (current->user->status == us_online) {
            printf(COLOR_USER_NAME "[%s]\n" COLOR_CLEAN, current->user->name);
        }
        current = current->next;
    }
}

static void output_friends_list(void)
{
    system("clear");
    printf(COLOR_CHAT_HEADER "FRIEND list\n" COLOR_CLEAN);
    user_list_set_new(rpmc_receive_users(user_session_id(), ruf_friends));
	
    if (user_list.users.head == NULL) {
        printf(COLOR_OFFER "There are no friends yet\n" COLOR_CLEAN);
    } else {
        output_users(user_list.users.head);
    }
}

static void output_online_list(void)
{
    system("clear");
    printf(COLOR_CHAT_HEADER "Users ONLINE\n" COLOR_CLEAN);
	user_list_set_new(rpmc_receive_users(user_session_id(), ruf_friends));
	
    if (user_list.users.head == NULL) {
        printf(COLOR_OFFER "...\n" COLOR_CLEAN);
    } else {
        output_online_users(user_list.users.head);
    }
}

static void output_current_list(void)
{
    system("clear");
    printf(COLOR_CHAT_HEADER "FRIEND list\n" COLOR_CLEAN);
	
    if (user_list.users.head == NULL) {
        printf(COLOR_OFFER "There are no friends yet\n" COLOR_CLEAN);
    } else {
        output_users(user_list.users.head);
    }
}

static void output_current_online(void)
{
    system("clear");
    printf(COLOR_CHAT_HEADER "Users ONLINE\n" COLOR_CLEAN);
	
    if (user_list.users.head == NULL) {
        printf(COLOR_OFFER "There are no friends yet\n" COLOR_CLEAN);
    } else {
        output_online_users(user_list.users.head);
    }
}

static void enter_chat(void)
{
	User_list* current = user_list.users.head;
	int i = user_list.selection;
	while(current && (i-- > 0)) {
		current = current->next;
	}
	if(current != NULL)
		terminal_user_chat_dialog(current->user);
}

typedef enum {
    contacts_online_exit,
    contacts_online_enter_chat,
	contacts_online_up,
	contacts_online_down
} Contacts_online_dialog;

void terminal_online_list(void)
{
    output_online_list();
    const String_view string_views[] = {
        string_view_create_from_char("back"),
        string_view_create_from_char("chat"),
        string_view_create_from_char("u"),
        string_view_create_from_char("d"),
    };

    Contacts_online_dialog action = dialog_read_action(string_views, sizeof(string_views) / sizeof(String_view));
    while (action) {
        switch (action)
        {
            case contacts_online_enter_chat:
                enter_chat();
                output_current_online();
                break;
				
			case contacts_online_down:
				if(user_list.selection < user_list.users.size - 1)
					++user_list.selection;
				output_current_online();
				break;
				
			case contacts_online_up:
				if(user_list.selection > 0)
					--user_list.selection;
				output_current_online();
				break;

            default:
                output_current_online();
                break;
        }
        action = dialog_read_action(string_views, sizeof(string_views) / sizeof(String_view));
    }
}

typedef enum {
    contacts_exit,
    contacts_list_all,
    contacts_list_online,
    contacts_list_friends,
    contacts_enter_chat,
	contacts_up,
	contacts_down
} Contacts_dialog;

void terminal_contacts_list(void)
{
	user_list_init();
    output_friends_list();
    const String_view string_views[] = {
        string_view_create_from_char("back"),
        string_view_create_from_char("all"),
        string_view_create_from_char("online"),
        string_view_create_from_char("friends"),
        string_view_create_from_char("chat"),
        string_view_create_from_char("u"),
        string_view_create_from_char("d"),
    };

    Contacts_dialog action = dialog_read_action(string_views, sizeof(string_views) / sizeof(String_view));
    while (action) {
        switch (action)
        {
			case contacts_list_friends:
                output_friends_list();
                break;
            case contacts_list_online:
                terminal_online_list();
				output_current_list();
                break;

            case contacts_enter_chat:
                enter_chat();
                output_current_list();
                break;
				
			case contacts_down:
				if(user_list.selection < user_list.users.size - 1)
					++user_list.selection;
				output_current_list();
				break;
				
			case contacts_up:
				if(user_list.selection > 0)
					--user_list.selection;
				output_current_list();
				break;

            default:
                output_current_list();
                break;
        }

        action = dialog_read_action(string_views, sizeof(string_views) / sizeof(String_view));
    }
	user_list_clear();
}
