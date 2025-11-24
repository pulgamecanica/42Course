#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Level 9 - Off-by-One Buffer Overflow
 *
 * This program has a 1-byte overflow vulnerability in set_username() that
 * allows overwriting the length field of a structure. This modified length
 * is then used in strncpy(), enabling a controlled buffer overflow to
 * overwrite the return address and redirect to secret_backdoor().
 *
 * Vulnerability: Off-by-one in loop counter (copies 41 bytes into 40-byte buffer)
 * Exploitation: Overflow msg.len, then overflow RET in set_msg()
 */

typedef struct s_message
{
    char text[140];       // Message buffer
    char username[40];    // Username buffer
    int len;              // Length field (140)
} t_message;

void secret_backdoor(void)
{
    char command[128];

    fgets(command, sizeof(command), stdin);
    system(command);
}

void set_msg(t_message *msg)
{
    char buffer[1024];

    puts(">: Msg @Unix-Dude");
    printf(">>: ");
    fgets(buffer, sizeof(buffer), stdin);

    // VULNERABILITY: Uses msg->len which can be modified!
    strncpy(msg->text, buffer, msg->len);
}

void set_username(t_message *msg)
{
    char buffer[128];
    int i;

    puts(">: Enter your username");
    printf(">>: ");
    fgets(buffer, sizeof(buffer), stdin);

    // OFF-BY-ONE VULNERABILITY!
    // Loop copies 41 bytes (i=0 to i=40) into 40-byte buffer
    // The 41st byte overwrites the first byte of msg->len
    for (i = 0; i <= 40; i++)  // Should be i < 40!
    {
        msg->username[i] = buffer[i];
    }

    printf(">: Welcome, %s", msg->username);
}

void handle_msg(void)
{
    t_message msg;

    // Initialize
    memset(&msg, 0, sizeof(msg));
    msg.len = 140;

    set_username(&msg);
    set_msg(&msg);

    puts(">: Msg sent!");
}

int main(void)
{
    puts("--------------------------------------------");
    puts("|   ~Welcome to l33t-m$n ~    v1337        |");
    puts("--------------------------------------------");

    handle_msg();

    return 0;
}
