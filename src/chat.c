#include "chat.h"
#include "api/messages.h"
#include "session.h"
#include "terminal.h"
#include <stddef.h>
#include <stdio.h>

/// @todo Получить имя пользователя по user_id
void terminal_output_chat(long user_id)
{
    Message_list* head = rpmc_receive_messages_from_user(user_session_id(), user_id);
    if (head->message == NULL) {
        printf(COLOR_OFFER "There is no messages yet\n" COLOR_CLEAN);
        message_list_delete(head);
        return;
    }

    Message_list* current = head;
    while (current) {
        const char* sender = current->message->status == ms_sender ? "(you)" : "user";
        printf(COLOR_USER_NAME "%s: " COLOR_CLEAN "%s\n", sender, current->message->text.ptr);
    }

    message_list_delete(head);
}
