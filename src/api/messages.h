#pragma once

#include "rpmc_string.h"

typedef enum  {
	ms_sender,
	ms_receiver
} Message_status;

typedef enum  {
	mt_text,
	mt_file
} Message_type;

typedef struct  {
	String text;
	Message_type type;
	Message_status status;
	long time;
} Message;

void message_delete(Message*);

typedef struct Message_list_t {
    Message* message;
    struct Message_list_t* next;
} Message_list;

Message_list* message_list_create_empty(void);
void message_list_delete(Message_list*);

typedef struct {
    Message_list* head;
    Message_list* current;
} Message_list_iterator;

Message_list_iterator rpmc_receive_messages_from_user(long session, long receiver_uuid);
