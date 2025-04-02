#pragma once

typedef enum {
	ruf_all,
	ruf_online,
	ruf_friends
} Receive_users_filter;

typedef enum  {
	us_online,
	us_offline
} User_status;

typedef struct  {
	long uuid;
	unsigned char name[15];
	unsigned int name_length;
	User_status status;
} User;

typedef struct User_list_t {
    User* user;
    struct User_list_t* next;
} User_list;

void user_list_delete(User_list*);

typedef struct {
    User_list* head;
    User_list* current;
} User_list_iterator;

User_list_iterator rpmc_receive_users(long session, Receive_users_filter filter);
