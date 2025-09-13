/*
 * Network Routing Simulator - Function Implementations
 * Handles network topology loading, packet generation, and routing simulation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "network_functions.h"

#define BUFFER_SIZE 100
#define MAX_NAME_LEN 20

// Helper function: compare node names alphabetically.
int compare_nodes(const char *a, const char *b) {
    return strcmp(a, b);
}

/* Load network nodes from file and insert them into a sorted linked list */
int load_nodes(const char *filename, Node **nodeList) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening node info file");
        return 1;
    }
    
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, fp)) {
        buffer[strcspn(buffer, "\r\n")] = '\0';
        if(strlen(buffer) == 0) continue;
        
        // Expected format: nodeName;ip
        char *token = strtok(buffer, ";");
        if (!token) continue;
        char name[20];
        strncpy(name, token, sizeof(name));
        name[sizeof(name)-1] = '\0';
        
        token = strtok(NULL, ";");
        if (!token) continue;
        char ip[20];
        strncpy(ip, token, sizeof(ip));
        ip[sizeof(ip)-1] = '\0';
        
        // Create and initialize new node.
        Node *newNode = (Node*) malloc(sizeof(Node));
        if (!newNode) {
            perror("Memory allocation error");
            fclose(fp);
            return 1;
        }
        strcpy(newNode->name, name);
        strcpy(newNode->ip, ip);
        newNode->next = NULL;
        
        // Insert in alphabetical order.
        if (*nodeList == NULL || compare_nodes(newNode->name, (*nodeList)->name) < 0) {
            newNode->next = *nodeList;
            *nodeList = newNode;
        } else {
            Node *current = *nodeList;
            while (current->next && compare_nodes(newNode->name, current->next->name) >= 0) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
        printf("Added node: %s with IP: %s\n", newNode->name, newNode->ip);
    }
    
    fclose(fp);
    return 0;
}

// Helper function: returns 1 if the node exists in nodeList.
int is_valid_node(Node *nodeList, const char *name) {
    Node *current = nodeList;
    while (current) {
        if (strcmp(current->name, name) == 0)
            return 1;
        current = current->next;
    }
    return 0;
}

// Check if an invalid node already exists in the invalid list.
int invalid_node_exists(InvalidNode *invalidList, const char *name) {
    while (invalidList) {
        if (strcmp(invalidList->name, name) == 0)
            return 1;
        invalidList = invalidList->next;
    }
    return 0;
}

/* Load network links from file, validating nodes and tracking invalid ones */
int load_links(const char *filename, Link **linkList, InvalidNode **invalidList, Node *nodeList) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening link info file");
        return 1;
    }
    
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, fp)) {
        buffer[strcspn(buffer, "\r\n")] = '\0';
        if(strlen(buffer) == 0) continue;
        
        // Expected format: src;dst;cost;bandwidth
        char *token = strtok(buffer, ";");
        if (!token) continue;
        char src[20];
        strncpy(src, token, sizeof(src));
        src[sizeof(src)-1] = '\0';
        
        token = strtok(NULL, ";");
        if (!token) continue;
        char dst[20];
        strncpy(dst, token, sizeof(dst));
        dst[sizeof(dst)-1] = '\0';
        
        token = strtok(NULL, ";");
        if (!token) continue;
        int cost = atoi(token);
        
        token = strtok(NULL, ";");
        if (!token) continue;
        int bandwidth = atoi(token);
        
        // Validate nodes.
        int srcValid = is_valid_node(nodeList, src);
        int dstValid = is_valid_node(nodeList, dst);
        
        if (srcValid && dstValid) {
            // Create new link node.
            Link *newLink = (Link*) malloc(sizeof(Link));
            if (!newLink) {
                perror("Memory allocation error");
                fclose(fp);
                return 1;
            }
            strcpy(newLink->src, src);
            strcpy(newLink->dst, dst);
            newLink->cost = cost;
            newLink->bandwidth = bandwidth;
            newLink->next = NULL;
            
            // Append to the end of the link list.
            if (*linkList == NULL) {
                *linkList = newLink;
            } else {
                Link *curr = *linkList;
                while (curr->next)
                    curr = curr->next;
                curr->next = newLink;
            }
            printf("Link added: %s -> %s with cost: %d and bandwidth: %d\n", src, dst, cost, bandwidth);
        } else {
            // Append invalid nodes if not already in the invalid list.
            if (!srcValid && !invalid_node_exists(*invalidList, src)) {
                InvalidNode *inv = (InvalidNode*) malloc(sizeof(InvalidNode));
                if (!inv) {
                    perror("Memory allocation error");
                    fclose(fp);
                    return 1;
                }
                strcpy(inv->name, src);
                inv->next = NULL;
                if (*invalidList == NULL)
                    *invalidList = inv;
                else {
                    InvalidNode *curr = *invalidList;
                    while (curr->next)
                        curr = curr->next;
                    curr->next = inv;
                }
                printf("Invalid node added: %s\n", src);
            }
            if (!dstValid && !invalid_node_exists(*invalidList, dst)) {
                InvalidNode *inv = (InvalidNode*) malloc(sizeof(InvalidNode));
                if (!inv) {
                    perror("Memory allocation error");
                    fclose(fp);
                    return 1;
                }
                strcpy(inv->name, dst);
                inv->next = NULL;
                if (*invalidList == NULL)
                    *invalidList = inv;
                else {
                    InvalidNode *curr = *invalidList;
                    while (curr->next)
                        curr = curr->next;
                    curr->next = inv;
                }
                printf("Invalid node added: %s\n", dst);
            }
        }
    }
    
    fclose(fp);
    
    // Print and then delete invalid nodes.
    if (*invalidList) {
        printf("\nList of invalid nodes:\n");
        InvalidNode *curr = *invalidList;
        while (curr) {
            printf("Invalid node: %s\n", curr->name);
            curr = curr->next;
        }
        
        printf("\nDeleting invalid nodes:\n");
        while (*invalidList) {
            InvalidNode *temp = *invalidList;
            printf("Deleting invalid node: %s\n", temp->name);
            *invalidList = (*invalidList)->next;
            free(temp);
        }
    }
    
    return 0;
}

/* Load routing table from file */
int load_routes(const char *filename, Route **routeList) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening route info file");
        return 1;
    }
    
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, fp)) {
        buffer[strcspn(buffer, "\r\n")] = '\0';
        if(strlen(buffer) == 0) continue;
        
        // Expected format: src;dst;nextHop;cost
        char *token = strtok(buffer, ";");
        if (!token) continue;
        char src[20];
        strncpy(src, token, sizeof(src));
        src[sizeof(src)-1] = '\0';
        
        token = strtok(NULL, ";");
        if (!token) continue;
        char dst[20];
        strncpy(dst, token, sizeof(dst));
        dst[sizeof(dst)-1] = '\0';
        
        token = strtok(NULL, ";");
        if (!token) continue;
        char nextHop[20];
        strncpy(nextHop, token, sizeof(nextHop));
        nextHop[sizeof(nextHop)-1] = '\0';
        
        token = strtok(NULL, ";");
        if (!token) continue;
        int cost = atoi(token);
        
        // Create and insert new routing entry at the front.
        Route *newRoute = (Route*) malloc(sizeof(Route));
        if (!newRoute) {
            perror("Memory allocation error");
            fclose(fp);
            return 1;
        }
        strcpy(newRoute->src, src);
        strcpy(newRoute->dst, dst);
        strcpy(newRoute->nextHop, nextHop);
        newRoute->cost = cost;
        newRoute->next = *routeList;
        *routeList = newRoute;
        
        printf("Route added: %s -> %s via %s with cost %d\n", src, dst, nextHop, cost);
    }
    
    fclose(fp);
    return 0;
}

/* Update routing table with new routes (simulating network changes) */
int update_routes(const char *filename, Route **routeList) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening reroute info file");
        return 1;
    }
    
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, fp)) {
        buffer[strcspn(buffer, "\r\n")] = '\0';
        if(strlen(buffer) == 0) continue;
        
        // Expected format: src;dst;newNextHop;newCost
        char *token = strtok(buffer, ";");
        if (!token) continue;
        char src[20];
        strncpy(src, token, sizeof(src));
        src[sizeof(src)-1] = '\0';
        
        token = strtok(NULL, ";");
        if (!token) continue;
        char dst[20];
        strncpy(dst, token, sizeof(dst));
        dst[sizeof(dst)-1] = '\0';
        
        token = strtok(NULL, ";");
        if (!token) continue;
        char newNextHop[20];
        strncpy(newNextHop, token, sizeof(newNextHop));
        newNextHop[sizeof(newNextHop)-1] = '\0';
        
        token = strtok(NULL, ";");
        if (!token) continue;
        int newCost = atoi(token);
        
        // Search for the matching route entry.
        Route *current = *routeList;
        int found = 0;
        while (current) {
            if (strcmp(current->src, src) == 0 && strcmp(current->dst, dst) == 0) {
                printf("Updating route: %s -> %s, old nextHop: %s, old cost: %d, new nextHop: %s, new cost: %d\n",
                       src, dst, current->nextHop, current->cost, newNextHop, newCost);
                strcpy(current->nextHop, newNextHop);
                current->cost = newCost;
                found = 1;
                break;
            }
            current = current->next;
        }
        if (!found) {
            printf("No existing route found for %s -> %s to update.\n", src, dst);
        }
    }
    
    fclose(fp);
    return 0;
}

/* Generate random packets between nodes in the network */
int generate_packets(Packet **packetList, int numPackets, Node *nodeList) {
    if (!nodeList) {
        printf("No nodes available for packet generation.\n");
        return 1;
    }
    
    // Count nodes and build an array for random access.
    int count = 0;
    Node *current = nodeList;
    while (current) {
        count++;
        current = current->next;
    }
    if (count == 0) return 1;
    
    Node **nodeArray = malloc(count * sizeof(Node*));
    if (!nodeArray) {
        perror("Memory allocation error");
        return 1;
    }
    
    current = nodeList;
    int index = 0;
    while (current) {
        nodeArray[index++] = current;
        current = current->next;
    }
    
    // Seed the random number generator.
    srand(time(NULL));
    
    for (int i = 0; i < numPackets; i++) {
        int srcIndex = rand() % count;
        int dstIndex = rand() % count;
        while (dstIndex == srcIndex) {
            dstIndex = rand() % count;
        }
        int size = rand() % (1500 - 100 + 1) + 100;
        
        Packet *newPacket = (Packet*) malloc(sizeof(Packet));
        if (!newPacket) {
            perror("Memory allocation error");
            free(nodeArray);
            return 1;
        }
        strcpy(newPacket->src, nodeArray[srcIndex]->name);
        strcpy(newPacket->dst, nodeArray[dstIndex]->name);
        newPacket->size = size;
        newPacket->next = NULL;
        
        // Append packet to the list.
        if (*packetList == NULL) {
            *packetList = newPacket;
        } else {
            Packet *p = *packetList;
            while (p->next)
                p = p->next;
            p->next = newPacket;
        }
        
        printf("Generated packet: %s -> %s, size: %d bytes\n", newPacket->src, newPacket->dst, newPacket->size);
    }
    
    free(nodeArray);
    return 0;
}

// Helper: Find a routing entry for a given source and destination.
Route* find_route(Route *routeList, const char *src, const char *dst) {
    Route *current = routeList;
    while (current) {
        if (strcmp(current->src, src) == 0 && strcmp(current->dst, dst) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

/* Forward packets through the network using the routing table */
int forward_packets(Packet *packetList, Route *routeList) {
    int totalPackets = 0;
    int totalSize = 0;
    
    Packet *pkt = packetList;
    while (pkt) {
        printf("\nForwarding packet: %s -> %s, size: %d bytes\n", pkt->src, pkt->dst, pkt->size);
        char currentNode[20];
        strcpy(currentNode, pkt->src);
        printf("Starting at node: %s\n", currentNode);
        
        // Follow the routing table until the destination is reached.
        while (strcmp(currentNode, pkt->dst) != 0) {
            Route *route = find_route(routeList, currentNode, pkt->dst);
            if (!route) {
                printf("No route found from %s to %s. Dropping packet.\n", currentNode, pkt->dst);
                break;
            }
            printf("Node %s sends to %s\n", currentNode, route->nextHop);
            strcpy(currentNode, route->nextHop);
            // Safety check to avoid loops.
            if (strcmp(currentNode, pkt->src) == 0) {
                printf("Routing loop detected. Dropping packet.\n");
                break;
            }
        }
        if (strcmp(currentNode, pkt->dst) == 0) {
            printf("Packet delivered to %s\n", pkt->dst);
            totalPackets++;
            totalSize += pkt->size;
            printf("Current packet size: %d bytes; Total packets: %d; Total size: %d bytes\n",
                   pkt->size, totalPackets, totalSize);
        }
        
        pkt = pkt->next;
    }
    
    return 0;
}

// Free functions for all linked lists.
void free_nodes(Node *nodeList) {
    Node *temp;
    while (nodeList) {
        temp = nodeList;
        nodeList = nodeList->next;
        free(temp);
    }
}

void free_invalid_nodes(InvalidNode *invalidList) {
    InvalidNode *temp;
    while (invalidList) {
        temp = invalidList;
        invalidList = invalidList->next;
        free(temp);
    }
}

void free_links(Link *linkList) {
    Link *temp;
    while (linkList) {
        temp = linkList;
        linkList = linkList->next;
        free(temp);
    }
}

void free_routes(Route *routeList) {
    Route *temp;
    while (routeList) {
        temp = routeList;
        routeList = routeList->next;
        free(temp);
    }
}

void free_packets(Packet *packetList) {
    Packet *temp;
    while (packetList) {
        temp = packetList;
        packetList = packetList->next;
        free(temp);
    }
}
