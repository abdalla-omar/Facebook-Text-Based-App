# Network Routing Simulator

A simple C-based network routing simulation that demonstrates packet forwarding through a network topology with support for route updates and link failures.

## Overview

This simulator models a basic network with:
- **Nodes**: Network devices with names and IP addresses
- **Links**: Connections between nodes with cost and bandwidth metrics
- **Routing Tables**: Determine packet forwarding paths
- **Packets**: Data units that travel through the network

The simulation demonstrates how packets are routed through the network and how the system handles network failures by updating routing tables.

## Features

- **Network Topology Loading**: Read nodes and links from configuration files
- **Packet Generation**: Create random packets between network nodes
- **Packet Forwarding**: Route packets using shortest path algorithms
- **Link Failure Simulation**: Update routes when network links fail
- **Invalid Node Detection**: Identify and report misconfigured nodes

## Requirements

- C compiler (gcc recommended)
- Make utility (optional, for using Makefile)
- Standard C libraries

## Installation

1. Clone or download the repository
2. Compile using Make:
   ```bash
   make
   ```
   
   Or compile manually:
   ```bash
   gcc -Wall -o network_simulator main.c network_functions.c
   ```

## Usage

1. Ensure you have the required data files (see Data Files section)
2. Run the simulator:
   ```bash
   ./network_simulator
   ```
   
   Or use Make:
   ```bash
   make run
   ```

## Data Files

The simulator expects the following CSV files in the same directory:

### nodes.txt
Contains network nodes with format:
```
nodeName;ipAddress
nodeA;192.168.1.1
nodeB;192.168.1.2
nodeC;192.168.1.3
```

### links.txt
Contains network connections with format:
```
sourceNode;destinationNode;cost;bandwidth
nodeA;nodeB;10;100
nodeB;nodeC;15;50
```

### routes.txt
Contains routing table entries with format:
```
source;destination;nextHop;cost
nodeA;nodeC;nodeB;25
nodeB;nodeC;nodeC;15
```

### routes_updated.txt
Contains updated routes after link failures:
```
source;destination;newNextHop;newCost
nodeA;nodeC;nodeD;30
```

## Simulation Flow

1. **Load Network Topology**: Read nodes and validate them
2. **Load Network Links**: Establish connections between nodes
3. **Load Routing Table**: Define packet forwarding rules
4. **Initial Packet Simulation**: Generate and forward packets
5. **Simulate Network Failure**: Update routing table
6. **Post-Failure Simulation**: Test packet forwarding with new routes
7. **Cleanup**: Free all allocated memory

## Code Structure

```
network_simulator/
├── main.c                  # Main program and simulation flow
├── network_functions.c     # Core functionality implementation
├── network_functions.h     # Function declarations
├── network_types.h         # Data structure definitions
├── Makefile               # Build configuration
└── README.md              # This file
```

### Key Data Structures

- **Node**: Linked list of network devices
- **Link**: Network connections with metrics
- **Route**: Routing table entries
- **Packet**: Data units with source/destination
- **InvalidNode**: Tracking misconfigured nodes

## Example Output

```
=== Network Routing Simulator ===

1. Loading network nodes...
Added node: nodeA with IP: 192.168.1.1
Added node: nodeB with IP: 192.168.1.2

2. Loading network links...
Link added: nodeA -> nodeB with cost: 10 and bandwidth: 100

3. Loading routing table...
Route added: nodeA -> nodeB via nodeB with cost 10

4. Running initial packet simulation...
   Generating 12 packets...
   Forwarding packets through network...
   
5. Simulating network failure...
   Updating routing table for link failures...
   
6. Running post-failure packet simulation...
   Forwarding packets through updated network...
   
7. Cleaning up...

=== Simulation Complete ===
```

## Customization

You can modify the simulation by:
- Changing the number of packets generated (currently 10-15)
- Adding new nodes and links in the data files
- Modifying routing algorithms in `network_functions.c`
- Adjusting packet size ranges (currently 100-1500 bytes)

## Memory Management

The simulator properly manages memory by:
- Allocating memory dynamically for all data structures
- Freeing all allocated memory on exit
- Handling allocation failures gracefully

## Error Handling

The simulator includes error checking for:
- File opening failures
- Memory allocation errors
- Invalid node references
- Routing loops
- Missing routes

## Future Enhancements

- Graphical visualization of network topology
- Real-time packet tracking
- Support for different routing algorithms
- Network performance metrics
- Configuration file validation
- Interactive mode for manual packet sending
