#ifndef NETWORK_FUNCTIONS_H
#define NETWORK_FUNCTIONS_H

#include "network_types.h"

/* Function declarations for the Network Routing Simulator */

/* File I/O functions */
int load_nodes(const char *filename, Node **nodeList);
int load_links(const char *filename, Link **linkList, InvalidNode **invalidList, Node *nodeList);
int load_routes(const char *filename, Route **routeList);
int update_routes(const char *filename, Route **routeList);
/* Packet simulation functions */
int generate_packets(Packet **packetList, int numPackets, Node *nodeList);
int forward_packets(Packet *packetList, Route *routeList);

/* Memory management functions */
void free_nodes(Node *nodeList);
void free_invalid_nodes(InvalidNode *invalidList);
void free_links(Link *linkList);
void free_routes(Route *routeList);
void free_packets(Packet *packetList);

#endif /* NETWORK_FUNCTIONS_H */