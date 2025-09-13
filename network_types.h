#ifndef NETWORK_TYPES_H
#define NETWORK_TYPES_H

/* Data structures for the Network Routing Simulator */

/* Network node with name and IP address */
typedef struct Node {
    char name[20];
    char ip[20];
    struct Node *next;
} Node;

/* Invalid node tracking */
typedef struct InvalidNode {
    char name[20];
    struct InvalidNode *next;
} InvalidNode;

/* Network link between two nodes */
typedef struct Link {
    char src[20];
    char dst[20];
    int cost;
    int bandwidth;
    struct Link *next;
} Link;

/* Routing table entry */
typedef struct Route {
    char src[20];
    char dst[20];
    char nextHop[20];
    int cost;
    struct Route *next;
} Route;

typedef struct Packet {
    char src[20];
    char dst[20];
    int size;
    struct Packet *next;
} Packet;

#endif /* NETWORK_TYPES_H */
