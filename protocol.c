/* 
 * Name: Abdullah O Alharbi
 * Canvas ID: aalharbi2
 * Course-Section: CS 360 01
 * Assignment: Program #3
 * Collaborators: none
 * Resources: Program #3 requirements page
 * Description: the protocol source file which contains the implementation of the protocol
 * functions
 */

#include <ctype.h>
#include "protocol.h"
#include <stdio.h>
#include <string.h>

struct packet createPacket(int choice, char *s) {
    struct packet p;

    memset(p.opcode, '\0', sizeof (p.opcode));
    memset(p.payload, '\0', sizeof (p.payload));

    switch (choice) {
        case 1:
            strncpy(p.opcode, "NOOP", 4);
            break;
        case 2:
            strncpy(p.opcode, "REVS", 4);
            break;
        case 3:
            strncpy(p.opcode, "UCAS", 4);
            break;
        case 4:
            strncpy(p.opcode, "RECS", 4);
            break;
        default:
            strncpy(p.opcode, "EROR", 4);
    }

    strcpy(p.payload, s);
    
    return p;
}

struct packet createMessage(struct packet *p) {

    struct packet newPacket;

    memset(newPacket.opcode, '\0', sizeof (newPacket.opcode));
    memset(newPacket.payload, '\0', sizeof (newPacket.payload));

    if (p->payload[0] == '\0')
        return newPacket;

    if (strlen(p->payload) == 0 && strlen(p->payload) > 128) {
        strncpy(newPacket.opcode, "EROR", 4);
        strlcpy(newPacket.payload, "Invalid message size", 21);
    }

    // opcode is NOOP, no operation, don’t change string
    if (strcmp(p->opcode, "NOOP") == 0) {

        strncpy(newPacket.opcode, "RPLY", 4);
        strlcpy(newPacket.payload, p->payload, strlen(p->payload) + 1);

    }

    // opcode is REVS, reverse the string
    else if (strcmp(p->opcode, "REVS") == 0) {

        int end;
        int i = 0;

        end = strlen(p->payload) - 1;

        strncpy(newPacket.opcode, "RPLY", 4);

        while (end >= 0) {

            newPacket.payload[i] = p->payload[end];

            i++;
            end--;
        }
    }

    // opcode is UCAS, uppercase the string
    else if (strcmp(p->opcode, "UCAS") == 0) {
        int i = 0;

        strncpy(newPacket.opcode, "RPLY", 4);

        while (p->payload[i]) {
            newPacket.payload[i] = toupper(p->payload[i]);
            i++;
        }
    }
    
    // opcode is RECS, reverse and Uppercase the string
    else if (strcmp(p->opcode, "RECS") == 0) {

        int i = 0;
        int end;
        
        end = strlen(p->payload) - 1;

        strncpy(newPacket.opcode, "RPLY", 4);
        
        while (end >= 0) {

            newPacket.payload[i] = p->payload[end];

            i++;
            end--;
        }
        
        i = 0;

        while (p->payload[i]) {
            newPacket.payload[i] = toupper(newPacket.payload[i]);
            i++;
        }  

    }

    else {
        strncpy(newPacket.opcode, "EROR", 4);
        strlcpy(newPacket.payload, "Protocol violation", 18);
    }
    
    return newPacket;
}

