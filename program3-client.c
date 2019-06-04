/* 
 * Name: Abdullah O Alharbi
 * Canvas ID: aalharbi2
 * Course-Section: CS 360 01
 * Assignment: Program #3
 * Collaborators: none
 * Resources: Program #3 requirements page
 * Description: this program is a client, which asks the user to choose one of the
 * operation from a menu. Then, it asks the user to enter the message. Then, it sends it
 * to the server which performs the operation specified in the opcode of the packet.
 * Finally, the server sends back a packet and the client displays it.
 */
 
#include <sys/socket.h>
#include <sys/types.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <err.h>
#include "protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum CONSTANTS {
    MAX_LINE = 256,
    SERVER_PORT = 1070,
    MESSAGE_LENGTH = 128
};

int
menu(int choice, char *msg);

int
main(int argc, char *argv[]) {
    struct sockaddr_in sa;
    struct hostent *hostp;
    int s;
    char *host;
    int choice;
    char message[MESSAGE_LENGTH];
    struct packet p;

    memset(p.opcode, '\0', sizeof (p.opcode));
    memset(p.payload, '\0', sizeof (p.payload));
    
    memset(message, '\0', MESSAGE_LENGTH);

    if (argc == 2)
        host = argv[1];
    else
        errx(1, "%s servername\n", getprogname());

    /* get the IP address of the server (parameter from command line) */
    hostp = gethostbyname(host);
    if (!hostp)
        errx(2, "Unable to resolve %s\n", host);

    /* initialize address data structure */
    memset(&sa, '\0', sizeof (sa));
    sa.sin_family = AF_INET;
    memcpy(&sa.sin_addr, hostp->h_addr, sizeof (sa.sin_addr));
    sa.sin_port = htons(SERVER_PORT); 

    /* active open: socket, connect */
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        err(3, "Socket open failed");

    if ((connect(s, (struct sockaddr *) &sa, sizeof (sa))) == -1) {
        close(s);
        err(4, "Connect failed");
    }

	choice = 0;
    choice = menu(choice, message);
    
    if(strlen(message) > 128){
        printf("Invalid message size");
        exit(1);
    }

    p = createPacket(choice, message);

    send(s, &p, sizeof (p), 0);

    /* get response from server */
    if (recv(s, &p, sizeof (p), 0) == -1) {
        close(s);
        err(5, "Receive failed");
    }

    printf("%s\n", p.payload);

    close(s);
    return 0;
}

int
menu(int choice, char *message) {
    char ignore;

    do {
        printf("1- No operation, don’t change string\n");
        printf("2- Reverse the string\n");
        printf("3- Make the string uppercase\n");
        printf("4- Reverse the string and make it uppercase\n");
        printf("5- Exit\n\n");

        printf("Choose one of the options above\n");
        scanf("%d", &choice);
        scanf("%c", &ignore);

        if (choice > 0 && choice < 5) {
            printf("Enter your message: ");
            fgets(message, MESSAGE_LENGTH, stdin);
            /* remove trailing newline */
            if ((strlen(message) > 0) && (message[strlen(message) - 1] == '\n'))
                message[strlen(message) - 1] = '\0';
        }
    } while (choice < 1 || choice > 5);

    return choice;
}

