#include "api/users.h"
#include <stdlib.h>
#include <string.h>

typedef struct  {
	User users[5];
	unsigned int size;
} User_array;

/// @brief Конвертировать буфер фиксированного размера в строку.
/// Скопировать данные в клиентский Message
static void user_list_add_user(User_list_iterator* list, const User* user)
{
    if (list->current == NULL) {
        list->head = malloc(sizeof(User_list));
        list->head->user = (User*)malloc(sizeof(User));
        list->head->next = NULL;
        list->current = list->head;
    } else {
        User_list* prev = list->current;
        prev->next = malloc(sizeof(User_list));
        list->current = prev->next;
        list->current->user = (User*)malloc(sizeof(User));
        list->current->next = NULL;
    }

    User* ptr = list->current->user;
    ptr->name_length = user->name_length;
    ptr->status = user->status;
    ptr->uuid = user->uuid;
    memcpy(ptr->name, user->name, ptr->name_length);
    ptr->name[ptr->name_length] = 0;
}

static User bogus_user(const char* name, User_status status)
{
    static long uuid_gen = 0;
    User user = {
        .name_length = strlen(name),
        .status = status,
        .uuid = ++uuid_gen
    };
    memcpy(user.name, name, user.name_length);
    user.name[user.name_length] = 0;
    return user;
}

static User_list_iterator bogus_user_list(void)
{
    User_list_iterator list = {
        .current = NULL,
        .head = NULL
    };

    User users[] = {
        bogus_user("Death", us_online),
        bogus_user("War", us_online),
        bogus_user("Hunger", us_offline),
        bogus_user("Plague", us_online),
        bogus_user("XakeP_2001", us_offline)
    };

    for (size_t i = 0; i < sizeof(users) / sizeof(User); ++i) {
        user_list_add_user(&list, &users[i]);
    }
    return list;
}

void user_list_delete(User_list* list)
{
    User_list* current = list;

    while (current) {
        User_list* prev = current;
        current = current->next;
        if (prev->user) {
            free(prev->user);
        }
        free(prev);
    }
}

User_list_iterator rpmc_receive_users(long session, Receive_users_filter filter) 
{ 
    return bogus_user_list();
}
