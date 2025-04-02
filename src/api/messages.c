#include "api/messages.h"
#include "rpmc_string.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_MESSAGE_LENGTH 255
#define MAX_MESSAGES_SIZE 5

typedef struct {
    char text[MAX_MESSAGE_LENGTH];
    unsigned int length;
    Message_type type;
    Message_status status;
    long time;
} api_Message;

typedef struct {
    api_Message messages[MAX_MESSAGES_SIZE];
    unsigned int size;
} api_Message_array;

/// @brief Конвертировать буфер фиксированного размера в строку.
/// Скопировать данные в клиентский Message
void message_list_add_message(Message_list* list, const api_Message* message)
{
    Message* ptr = NULL;
    if (list->message == NULL) {
        list->message = (Message*)malloc(sizeof(Message));
        ptr = list->message;
    } else {
        list->next = malloc(sizeof(Message_list));
        list->next->message = (Message*)malloc(sizeof(Message));
        list->next->next = NULL;
    }
    ptr->text = string_create_from_const_char(message->text);
    ptr->status = message->status;
    ptr->time = message->time;
    ptr->type = message->type;
}

/// @brief Считать по MAX_MESSAGES_SIZE сообщений с сервера для двух пользователей
/// @todo Добавить вызов стаба
Message_list* rpmc_receive_messages_from_user(long session, long receiver_uuid)
{
    Message_list* head = message_list_create_empty();
    Message_list* current = head;

    unsigned int size = MAX_MESSAGES_SIZE;
    unsigned int offset = 0;

    while (size == MAX_MESSAGES_SIZE)
    {
        /// ... stub read session receiver_uuid offset
        offset += MAX_MESSAGES_SIZE;
        api_Message_array api_messages;
        for (unsigned int i = 0; i < api_messages.size; ++i) {
            message_list_add_message(current, &api_messages.messages[i]);
            current = current->next;
        }
    }
    return head;
}

Message_list* message_list_create_empty(void)
{
    Message_list* list = malloc(sizeof(Message_list));
    list->message = NULL;
    list->next = NULL;
    return list;
}

void message_list_delete(Message_list* list)
{
    Message_list* current = list;

    while (current) {
        Message_list* prev = current;
        current = current->next;
        if (prev->message) {
            message_delete(prev->message);
            free(prev->message);
        }
        free(prev);
    }
}

void message_delete(Message* message)
{
    string_deallocate(&message->text);
}
