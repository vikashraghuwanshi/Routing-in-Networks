
Operating System :- Linux(Ubuntu 20.04)

Compile :- g++ routing.cpp -o routing
               
Run     :- ./routing TopologyFileName ConnectionFileName RoutingTableFiileName ForwardingTableFileName PathsFileName hop/distanceMetric(0/1)                                    optimisic/pessimisticApproach(0/1)

            For hop metric :-   hop/distanceMetric = 0
            For dist metric :-  hop/distanceMetric = 1

            For optimistic approach :-  approach = 0
            For pessimistic approach :- approach = 1

Examples :-
1. ./routing top24.txt ARPANET_100.txt routingtable.txt forwardingtable.txt paths.txt 1 1
2. ./routing top14.txt NSFNET_100.txt routingtable.txt forwardingtable.txt paths.txt 1 1



#Overview

In this assignment you will be given topology file. By using shortest path and link disjoint concept
create the routing table. In the routing table for each node to every other node, there will be 1 st
shortest distance and 2 nd shortest distance. (Hint: all pair shortest path or apply Dijkstra for every node).
After that create the label forwarding information base for each node using the routing table.
Then using the optimistic or pessimistic approach create connection between nodes. A connection
request file (Arpanet for 24 node and Nsfnet for 14 nodes) is given in which the connection request
between 2 nodes and the required bandwidth is mentioned.
All the output files format is given below.

#Inputs

The command line will specify the following:
topology of the network
connections between nodes
routing tables
forwarding table
path taken from one node to another
For e.g., the input can be given as below:
% ./-routing -top topology file -conn connectionsfile -rt routingtablefile -ft forwardingtablefile -path
pathsfile - flag hopjdist - p 0|1.

The topology file contains on the first line: NodeCount (N), Edge- count (E) of the network. Nodes
are numbered from 0 through N-1.
On each subsequent line, there are four numbers - the first two integers represent the two endpoints of a bi-directional link, the third integer denote the link cost, and the fourth integer denotes the link's capacity (in Mbps).

The connections file contains on the first line: Number of ConnectionRequests (R). Connections are numbered from 0 through R - 1.
On each subsequent line, there are 5 numbers - the first two integers represent the source and destination node of a unidirectional connection, the remaining 3 integers denote the connection's stated capacity (in Mbps). The stated capacity of a connection request i specifies the requested bandwidth using three integers: (b imin ; b iave ; b imax ), which specify the minimum, average, and maximum bandwidth needed, respectively.
Please note that there may be several connections between the same source-destination pair.
RoutingThe program will first determine two shortest cost (not necessarily link-disjoint) paths for all node-pairs.
You may use either hop or distance metric. The command line parameter will specify the choice. For hop, the link cost will be 1 and for distance third integer in the input file can be taken.

#Connections

The program will then process the specified set of connection requests.

#Optimistic Approach: This is used if -p command-line argument has value 0. Let C l denote the total
capacity of a link. Let biequiv = min[bimax , biavg + 0.25*(bimax - b imin )]. A connection is admitted if the following condition is met, along each link of the path selected for connection i:

such that biequi <= C1 - sum of all assigned bandwidths to any connection.

where n denotes the number of existing connections sharing a given link (along the path selected for the given connection) and j denotes the index of a connection assigned on this link.
Next, for each source-destination pair, select the two link-disjoint paths. If adequate bandwidth is not available along the first shortest-cost path, then the network attempts to set up the connection on the second shortest-cost path. If this also fails, then the connection is NOT admitted, i.e., it fails to meet the admission control test. Once an available path is identified, the connection will be set up along the path. This primarily involves setting up link-unique label id for a given connection request along all links of the path, and updating the corresponding forwarding tables at each intermediate node along the path.

#Pessimistic Approach: If —p value in command-line argument is 1 then use this approach. Let C l denote the total capacity of link l. A connection is admitted if the following condition is met, along each link of the path selected for connection i:

bimax <= C1 - sum of all assigned bandwidths to any connection.

where j denotes the index of a connection assigned on a given link along the path selected for the given connection. This gets repeated for the pessimistic approach.

#Outputs

The routing table file will contain the routing table information for all the nodes. For each network node, the corresponding routing table (i.e. two link-disjoint paths from the given node to all other nodes) displayed with the following fields:
Destination Node
Path
Path Cost

#Table 1: routingtablefile formatThe forwardingtable file will contain the forwarding table information for all the nodes. For each network node, the corresponding forwarding table (for all established connections) will have following format:
Interface in
Label in
Interface out
Label out

#Table 2: Label Information forwarding base format paths will contain only one line consisting of two integers which represent the total number of requested connections and the total number of admitted connections, respectively.

For each connection that is admitted into the network, the output format is as follows:
Connection Id
Source
Destination
Label List
Path Cost

#Table 3: Output format for each connection. Sort it based on connection ID.
Also calculate number of successful connections.
