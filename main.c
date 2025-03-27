#include "credentials.h"

int main(void) 
{
	Credentials credentials = terminal_register();
    credentials_delete(&credentials);
	return 0;
}
