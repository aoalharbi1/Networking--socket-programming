/* 
 * Name: Abdullah O Alharbi
 * Canvas ID: aalharbi2
 * Course-Section: CS 360 01
 * Assignment: Program #3
 * Collaborators: none
 * Resources: Program #3 requirements page
 * Description: the protocol header file
 */
 
struct packet 
{
	char opcode[5];
	char payload[128];
};


struct packet createPacket(int choice, char *s);

struct packet createMessage(struct packet *p);

