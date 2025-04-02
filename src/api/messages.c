#include "api/messages.h"
#include "rpmc_string.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

static api_Message bogus_message(Message_status status, const char* text);
static Message_list_iterator bogus_message_history(void);

/// @brief Конвертировать буфер фиксированного размера в строку.
/// Скопировать данные в клиентский Message
void message_list_add_message(Message_list_iterator* list, const api_Message* message)
{
    if (list->current == NULL) {
        list->head = malloc(sizeof(Message_list));
        list->head->message = (Message*)malloc(sizeof(Message));
        list->head->next = NULL;
        list->current = list->head;
    } else {
        Message_list* prev = list->current;
        prev->next = malloc(sizeof(Message_list));
        list->current = prev->next;
        list->current->message = (Message*)malloc(sizeof(Message));
        list->current->next = NULL;
    }

    Message* ptr = list->current->message;
    ptr->text = string_create_from_const_char(message->text);
    ptr->status = message->status;
    ptr->time = message->time;
    ptr->type = message->type;
}

/// @brief Считать по MAX_MESSAGES_SIZE сообщений с сервера для двух пользователей
/// @todo Добавить вызов стаба
Message_list_iterator rpmc_receive_messages_from_user(long session, long receiver_uuid)
{
    return bogus_message_history();
    Message_list_iterator list = {
        .current = NULL,
        .head = NULL
    };

    unsigned int size = MAX_MESSAGES_SIZE;
    unsigned int offset = 0;

    while (size == MAX_MESSAGES_SIZE)
    {
        api_Message_array api_messages = {
            .size = 0
        };
        /// ... stub read session receiver_uuid offset
        offset += MAX_MESSAGES_SIZE;

        for (unsigned int i = 0; i < api_messages.size; ++i) {
            message_list_add_message(&list, &api_messages.messages[i]);
        }
        size = api_messages.size;
    }
    return list;
}

Message_list* message_list_create_empty(void)
{
    Message_list* list = (Message_list*)malloc(sizeof(Message_list));
    printf("%zu\n", (long)list);
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

static api_Message bogus_message(Message_status status, const char* text)
{
    api_Message message = {
        .length = strlen(text),
        .type = mt_text,
        .status = status,
        .time = time(NULL),
    };
    memcpy(message.text, text, message.length);
    message.text[message.length] = 0;
    return message;
}

static Message_list_iterator bogus_message_history(void)
{
    Message_list_iterator list = {
        .current = NULL,
        .head = NULL
    };

    api_Message messages[] = {
        bogus_message(ms_sender, "Hi"),
        bogus_message(ms_receiver, "Hi"),
        bogus_message(ms_sender, "When you can't even say my name"),
        bogus_message(ms_sender, "Has the memory gone? Are you feeling numb?"),
        bogus_message(ms_receiver, "Go on, call my name"),
        bogus_message(ms_receiver, "I can't play this game, so I ask again"),
        bogus_message(ms_sender, "Will you say my name?"),
        bogus_message(ms_receiver, "Has the memory gone? Are you feeling numb?"),
        bogus_message(ms_sender, "Or have I become invisible?"),
    };

    for (size_t i = 0; i < sizeof(messages) / sizeof(api_Message); ++i) {
        message_list_add_message(&list, &messages[i]);
    }
    return list;
}
