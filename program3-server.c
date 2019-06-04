/* 
 * Name: Abdullah O Alharbi
 * Canvas ID: aalharbi2
 * Course-Section: CS 360 01
 * Assignment: Program #3
 * Collaborators: none
 * Resources: Program #3 requirements page
 * Description: this program is a server, which allows a client to be connected to it.
 * The client will send a packet (4 characters opcode, 128 characters payload). The server validate
 * the packet. If it is valid, it performs the requested operation then sends the packet
 * back to the client. The sevrer will remain up until it gets a signal.
 */


#include <sys/socket.h>
#include <sys/types.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include <err.h>
#include <errno.h>
#include "protocol.h"
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void sig_handler(int);

volatile sig_atomic_t cleanup_exit;

enum CONSTANTS {
    MAX_PENDING = 5,
    MAX_LINE = 256,
    SERVER_PORT = 1070
};

int
main() {
    struct sockaddr_in sa;
    int s;
    cleanup_exit = false;

    /* install signal handlers */
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);

    /* initialize address data structure */
    memset(&sa, '\0', sizeof (sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_port = htons(SERVER_PORT);

    /* passive open: socket, bind, listen, accept */
    if ((s = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        err(1, "Socket open failed");

    if ((bind(s, (struct sockaddr *) &sa, sizeof (sa))) == -1)
        err(2, "Bind failed");

    if ((listen(s, MAX_PENDING)) == -1)
        err(3, "Listen failed");

    /* loop until SIGTERM */
    while (!cleanup_exit) {
        struct sockaddr_storage addr;
        socklen_t addrlen;
        int new_s;
        struct packet p;
        struct packet newp;

        memset(p.opcode, '\0', sizeof (p.opcode));
        memset(p.payload, '\0', sizeof (p.payload));
        
        memset(newp.opcode, '\0', sizeof (newp.opcode));
        memset(newp.payload, '\0', sizeof (newp.payload));

        addrlen = sizeof (addr);
        if ((new_s = accept(s, (struct sockaddr *) &addr, &addrlen)) == -1)
            err(4, "Accept failed");

        recv(new_s, &p, sizeof (p), 0);
        
        
        if(strlen(p.payload) <= 128){
        	newp = createMessage(&p);
        }
        else {
        	strlcpy(newp.payload, "Invalid message size", 21);
        }

        send(new_s, &newp, sizeof (newp), 0);

        close(new_s);
    }

    close(s);
    return 0;
}

/*
 *  function from Dr. Jerkins
 *  handles signals sent to the process
 */
void
sig_handler(int sig) {
    int save_errno = errno;

    switch (sig) {
        case SIGINT:
            puts("CTRL-C pressed");
            break;
        case SIGTERM:
            puts("SIGTERM received");
            cleanup_exit = true;
            break;
        default:
            printf("%d not handled\n", sig);
    }

    errno = save_errno;
}

