/* file: hello.idl */

 

[ uuid (6B29FC40-CA47-2167-B31F-00DD010662DA),

  version(1.0)

]

interface hello

{

typedef enum {
	rs_ok = 0,
	rs_name_already_exists = 1,
	rs_name_incorrect_format = 2,
	rs_password_incorrect_format = 3
} Register_status;

long Register([in, string] unsigned char* name, [in, string] unsigned char* password);
long Log_in([in, string] unsigned char* name, [in, string] unsigned char* password, [in, string] unsigned char* device);
void Log_out(long session);

typedef enum {
	sms_ok,
	sms_session_absent,
	sms_receiver_incorrect
} Send_message_status;

Send_message_status Send_message(long session, long receiver_uuid, [in, string] unsigned char* text, long time);

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

typedef struct  {
	User users[5];
	unsigned int size;
} User_array;

User_array Receive_users(long session, Receive_users_filter filter, int offset);


typedef enum  {
	ms_sender,
	ms_receiver
} Message_status;

typedef enum  {
	mt_text,
	mt_file
} Message_type;

typedef struct  {
	unsigned char text[255];
	unsigned int length;
	Message_type type;
	Message_status status;
	long time;
} Message;

typedef struct  {
	Message messages[5];
	unsigned int size;
} Message_array;

Message_array Receive_messages_from_user(long session, long receiver_uuid, int offset);
	

typedef struct  {
	unsigned char chunk[1024];
	unsigned int size;
} Chunk;

long Send_file(long session, long receiver_uuid, unsigned long length, [in, string] unsigned char* file_name, long time);
void Send_chunk(long session, long file_id, Chunk chunk);

typedef struct  {
	unsigned long length;
	long file_id;
} File_metadata;

File_metadata Receive_file(long session, [in, string] unsigned char* file_name);
Chunk Receive_chunk(long session, long file_id, unsigned int chunk_index);
	
}