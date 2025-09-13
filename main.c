/*
 * Network Routing Simulator
 * A simple network packet routing simulation application
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "network_functions.h"

int main() {
    // Initialize random seed for packet generation
    srand(time(NULL));
    
    // Initialize data structures
    Node *nodeList = NULL;
    Link *linkList = NULL;
    InvalidNode *invalidList = NULL;
    Route *routeList = NULL;
    Packet *packetList = NULL;
    
    printf("\n=== Network Routing Simulator ===\n");
    
    // Load network topology
    printf("\n1. Loading network nodes...\n");
    if (load_nodes("nodes.txt", &nodeList) != 0) {
        fprintf(stderr, "Error: Failed to load network nodes.\n");
        return EXIT_FAILURE;
    }
    
    printf("\n2. Loading network links...\n");
    if (load_links("links.txt", &linkList, &invalidList, nodeList) != 0) {
        fprintf(stderr, "Error: Failed to load network links.\n");
        return EXIT_FAILURE;
    }
    
    printf("\n3. Loading routing table...\n");
    if (load_routes("routes.txt", &routeList) != 0) {
        fprintf(stderr, "Error: Failed to load routing table.\n");
        return EXIT_FAILURE;
    }
    
    printf("\n4. Running initial packet simulation...\n");
    int numPackets = 10 + (rand() % 6);  // Generate 10-15 packets
    printf("   Generating %d packets...\n", numPackets);
    
    if (generate_packets(&packetList, numPackets, nodeList) != 0) {
        fprintf(stderr, "Error: Failed to generate packets.\n");
        return EXIT_FAILURE;
    }
    
    printf("\n   Forwarding packets through network...\n");
    if (forward_packets(packetList, routeList) != 0) {
        fprintf(stderr, "Error: Failed to forward packets.\n");
        return EXIT_FAILURE;
    }
    free_packets(packetList);
    packetList = NULL;
    
    printf("\n5. Simulating network failure...\n");
    printf("   Updating routing table for link failures...\n");
    
    if (update_routes("routes_updated.txt", &routeList) != 0) {
        fprintf(stderr, "Error: Failed to update routing table.\n");
        return EXIT_FAILURE;
    }
    
    printf("\n6. Running post-failure packet simulation...\n");
    numPackets = 10 + (rand() % 6);
    printf("   Generating %d new packets...\n", numPackets);
    
    if (generate_packets(&packetList, numPackets, nodeList) != 0) {
        fprintf(stderr, "Error: Failed to generate packets.\n");
        return EXIT_FAILURE;
    }
    
    printf("\n   Forwarding packets through updated network...\n");
    if (forward_packets(packetList, routeList) != 0) {
        fprintf(stderr, "Error: Failed to forward packets.\n");
        return EXIT_FAILURE;
    }
    free_packets(packetList);
    
    // Clean up
    printf("\n7. Cleaning up...\n");
    free_nodes(nodeList);
    free_links(linkList);
    free_routes(routeList);
    
    printf("\n=== Simulation Complete ===\n\n");
    return EXIT_SUCCESS;
}
