#include <stdio.h>
#include <stdlib.h>
#include "a2_functions.h"

int main() {
    NodeInfo *nodeList = NULL;
    LinkInfo *linkList = NULL;
    InvalidNode *invalidList = NULL;
    RouteEntry *routeList = NULL;
    Packet *packetList = NULL;
    
    printf("=== Reading Node Information ===\n");
    if (get_node_info("nodeinfo1.txt", &nodeList) != 0) {
        fprintf(stderr, "Failed to read node information.\n");
        return EXIT_FAILURE;
    }
    
    printf("\n=== Reading Link Information ===\n");
    if (get_link_info("linkinfo1.txt", &linkList, &invalidList, nodeList) != 0) {
        fprintf(stderr, "Failed to read link information.\n");
        return EXIT_FAILURE;
    }
    
    printf("\n=== Reading Routing Information ===\n");
    if (get_route_info("routeinfo1.txt", &routeList) != 0) {
        fprintf(stderr, "Failed to read routing information.\n");
        return EXIT_FAILURE;
    }
    
    printf("\n=== Generating and Forwarding Packets (Initial Routing) ===\n");
    // Generate between 10 and 15 packets.
    int numPackets = 10 + (rand() % 6);
    if (packet_gen(&packetList, numPackets, nodeList) != 0) {
        fprintf(stderr, "Failed to generate packets.\n");
        return EXIT_FAILURE;
    }
    if (packet_forward(packetList, routeList) != 0) {
        fprintf(stderr, "Failed to forward packets.\n");
        return EXIT_FAILURE;
    }
    free_packets(packetList);
    packetList = NULL;
    
    printf("\n=== Simulating Link Failure and Rerouting ===\n");
    // Simulate a link failure (for example, between nodeA and nodeB) by reading updated routes.
    if (get_reroute_info("rerouteinfo.txt", &routeList) != 0) {
        fprintf(stderr, "Failed to update routing information.\n");
        return EXIT_FAILURE;
    }
    
    printf("\n=== Generating and Forwarding Packets (After Rerouting) ===\n");
    numPackets = 10 + (rand() % 6);
    if (packet_gen(&packetList, numPackets, nodeList) != 0) {
        fprintf(stderr, "Failed to generate packets.\n");
        return EXIT_FAILURE;
    }
    if (packet_forward(packetList, routeList) != 0) {
        fprintf(stderr, "Failed to forward packets.\n");
        return EXIT_FAILURE;
    }
    free_packets(packetList);
    
    // Free all allocated memory.
    free_nodes(nodeList);
    free_links(linkList);
    free_routes(routeList);
    
    printf("\n=== Assignment Completed ===\n");
    return EXIT_SUCCESS;
}
