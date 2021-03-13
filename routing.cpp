/*

Vikash Raghuwanshi 204101059
Sushil Kumar       204101057

*/

#include<cstdio>
#include<iostream>
#include<fstream>
#include<vector>
#include<queue>
using namespace std;

typedef pair<int, int> pairs;


//to store tables data
vector<int> routingTable[100][3][100];
vector<int> forwardingTable[100];
vector<int> connectionPathTable;


int INF = 500000;
int forwardTable[100][4], pathCost[100][3][100];
int parent[100], tempParent[100], edgeWeights[100][100];
int pathNumber, labelId = 0, label = 99, node = 0, src, desc, weight;
int flag = 0, admittedConnections = 0, nodeCount = 0, edgeCount, connRequests, tempNode;

double remainingCapacity[100][100];
double linkCapacity, bMin, bAvg, bMax;

vector<int> labelList;
vector<pairs> adjList[100];
vector<int> weights(100, 500000);
vector<vector<int>> allLabelsList;

char readText[10];

//function which check for connection using pessimistic or optimistic approach and makes connecition and then create forwarding tables
int makeConnection() {

  int i;
  labelList.clear();
  for(i = 0; routingTable[src][pathNumber][desc].size() > 1 && routingTable[src][pathNumber][desc][i+1] != -1; i ++) {
    if(remainingCapacity[routingTable[src][pathNumber][desc][i]][routingTable[src][pathNumber][desc][i + 1]] < bMax) break;
    else {
      if(i == 0) {
        forwardTable[src][0] = -1;
        forwardTable[src][1] = -1;
        forwardTable[src][2] = routingTable[src][pathNumber][desc][i + 1];
        forwardTable[src][3] = ++ label;
        labelList.push_back(forwardTable[src][3]);
      }
      else {
        forwardTable[routingTable[src][pathNumber][desc][i]][0] = routingTable[src][pathNumber][desc][i - 1];
        forwardTable[routingTable[src][pathNumber][desc][i]][1] = forwardTable[routingTable[src][pathNumber][desc][i - 1]][3];
        forwardTable[routingTable[src][pathNumber][desc][i]][2] = routingTable[src][pathNumber][desc][i + 1];
        forwardTable[routingTable[src][pathNumber][desc][i]][3] = label ++;
        labelList.push_back(forwardTable[routingTable[src][pathNumber][desc][i - 1]][3] + 1);
      }
    }
  }

  if(routingTable[src][pathNumber][desc].size() > 1 && routingTable[src][pathNumber][desc][i+1] == -1) {

    allLabelsList.push_back(labelList);
    forwardTable[routingTable[src][pathNumber][desc][i]][0] = routingTable[src][pathNumber][desc][i - 1];
    forwardTable[routingTable[src][pathNumber][desc][i]][1] = forwardTable[routingTable[src][pathNumber][desc][i - 1]][3];
    forwardTable[routingTable[src][pathNumber][desc][i]][2] = -1;
    forwardTable[routingTable[src][pathNumber][desc][i]][3] = -1;

    admittedConnections ++;
    for(int j = 0; routingTable[src][pathNumber][desc][j] != -1; j ++) {
      if(routingTable[src][pathNumber][desc][j + 1] != -1) {
        remainingCapacity[routingTable[src][pathNumber][desc][j]][routingTable[src][pathNumber][desc][j + 1]] -= bMax;
        remainingCapacity[routingTable[src][pathNumber][desc][j + 1]][routingTable[src][pathNumber][desc][j]] -= bMax;
      }


      forwardingTable[routingTable[src][pathNumber][desc][j]].push_back(forwardTable[routingTable[src][pathNumber][desc][j]][0]);
      forwardingTable[routingTable[src][pathNumber][desc][j]].push_back(forwardTable[routingTable[src][pathNumber][desc][j]][1]);
      forwardingTable[routingTable[src][pathNumber][desc][j]].push_back(forwardTable[routingTable[src][pathNumber][desc][j]][2]);
      forwardingTable[routingTable[src][pathNumber][desc][j]].push_back(forwardTable[routingTable[src][pathNumber][desc][j]][3]);
      forwardingTable[routingTable[src][pathNumber][desc][j]].push_back(-2);

    }
    return 1;
  }
  return 0;
}


//function used to take out the path recursively
void printPath(int i) {
  if(i == node) return;
  printPath(parent[i]);
  routingTable[node][pathNumber][tempNode].push_back(parent[i]);
}

//function used to delete the 1st shortest path from graph
void endPath(int i) {
  if(i == node) return;

  for(int j = 0; j < adjList[i].size(); j ++) {
    if(adjList[i][j].first == tempParent[i]) adjList[i][j].second = 500000;
  }
  for(int j = 0; j < adjList[tempParent[i]].size(); j ++) {
    if(adjList[tempParent[i]][j].first == i) adjList[tempParent[i]][j].second = 500000;
  }

  endPath(tempParent[i]);
}

//function used to rebuild the 1st shortest path in graph
void buildPath(int i) {
  if(i == node) return;

  for(int j = 0; j < adjList[i].size(); j ++) {
    if(adjList[i][j].first == tempParent[i]) adjList[i][j].second = edgeWeights[i][tempParent[i]];
    }
  for(int j = 0; j < adjList[tempParent[i]].size(); j ++) {
    if(adjList[tempParent[i]][j].first == i) adjList[tempParent[i]][j].second = edgeWeights[i][tempParent[i]];
  }

  buildPath(tempParent[i]);
}

//used to find the shortest paths between source and destination
void dijkstrasLinkDisjoint(int src) {

  priority_queue<pairs, vector<pairs>, greater<pairs>> pq;

  vector<int> visited(100, 0);
  weights.assign(100, 500000);

  pq.push(make_pair(0, src));
  weights[src] = 0;

  int minWeight, deletedNodes = 0;
  while(!pq.empty()) {

    int minWeightSrc = pq.top().second;
    minWeight = pq.top().first;

    if(visited[minWeightSrc] != 1) deletedNodes ++;

    if(deletedNodes == nodeCount) break;

    pq.pop();
    visited[minWeightSrc] = 1;

    for(auto i = adjList[minWeightSrc].begin(); i != adjList[minWeightSrc].end(); i ++) {
      int weight = i->second;
      int adjacentVertex = i->first;
      if (weights[adjacentVertex] > weight + weights[minWeightSrc]) {
          weights[adjacentVertex] = weight + weights[minWeightSrc];
          pq.push(make_pair(weights[adjacentVertex], adjacentVertex));
          parent[adjacentVertex] = minWeightSrc;
      }

    }
  }
}



int main(int argc, char *argv[]) {
  
  if(argc < 8) {
        printf("Invalid Command Line Arguments!!!\n");
        exit(1);
  }


  //checking that the file is .doc type or .txt type and then reading the topology file
  FILE *ff = fopen(argv[1], "r");

  int size = 0;
  for(int i = 0; argv[1][i] != '\0'; i ++) size ++;

  if(argv[1][size - 4] == '.' && argv[1][size - 3] == 'd' && argv[1][size - 2] == 'o' && argv[1][size - 1] == 'c') {
    fscanf(ff, "%s %d", readText, &edgeCount);
    for(int i = 3; readText[i] != '\0'; i ++) {
      nodeCount = (nodeCount*10) + (readText[i] - '0');
    }
  }
  else fscanf(ff, "%d %d", &nodeCount, &edgeCount);

  int i = 0;
  while(i ++ < edgeCount) {

    fscanf(ff, "%d %d %d %lf", &src, &desc, &weight, &linkCapacity);
    
    if(atoi(argv[6]) == 0) {
      weight = 1;
    }
    adjList[src].push_back({desc, weight});
    adjList[desc].push_back({src, weight});
    remainingCapacity[src][desc] = linkCapacity;
    remainingCapacity[desc][src] = linkCapacity;
    edgeWeights[src][desc] = weight;
    edgeWeights[desc][src] = weight;
  }
  fclose(ff);

  //finding 2 shortest paths for everynode
  for(int i = 0; i < nodeCount; i ++) {
    
    node = i;
    dijkstrasLinkDisjoint(i);

    for(int k = 0; k < nodeCount; k ++) {
      tempNode = k;
      pathNumber = 1;
      flag = 0;
      if(weights[k] == 500000) {
          flag = 1;
      }
      if(flag == 0) {
        printPath(k);
        routingTable[node][pathNumber][tempNode].push_back(tempNode);
      }
      routingTable[node][pathNumber][tempNode].push_back(-1);
      if(flag == 0) {
        pathCost[i][pathNumber][k] = weights[k];
      }
      flag = 0;
    }

    for(int k = 0; k < nodeCount; k ++) {
      tempParent[k] = parent[k];
    }

    for(int j = 0; j < nodeCount; j ++) {
      tempNode = j;
      pathNumber = 2;
      if(i != j) {
        if(routingTable[node][1][tempNode].size() > 1) {
          endPath(j);
          dijkstrasLinkDisjoint(i);
          if(weights[j] == 500000) {
            flag = 1;
          }
          if(flag == 0) {
            printPath(j);
            pathCost[i][pathNumber][j] = weights[j];
          }
          buildPath(j);
        }
        else flag = 1;

      }
      if(flag == 0) routingTable[node][pathNumber][tempNode].push_back(j);
      flag = 0;
      routingTable[node][pathNumber][tempNode].push_back(-1);
    }
  }

  //updating the routingTable in routing table File
  ff = fopen(argv[3], "w");
  fprintf(ff, "Destination                      Path                   PathCost");
  for(int i = 0; i < nodeCount; i ++) {
    fprintf(ff, "\nShortest Path for Node %d :- \n\n", i);
    for(int j = 1; j < 3; j ++) {
      fprintf(ff, "Shortest Path %d :-\n", j);
      for(int k = 0; k < nodeCount; k ++) {
        fprintf(ff, "%d     ==>                 ", k);
        for(int l = 0; routingTable[i][j][k][l] != -1; l ++) {
          fprintf(ff, "%d ", routingTable[i][j][k][l]);
        }
        if(routingTable[i][j][k].size() == 1) fprintf(ff, "No Path Exist\n");
        else fprintf(ff, "                              %d\n", pathCost[i][j][k]);
      }
    fprintf(ff, "\n\n");
    }
  }
  fclose(ff);

  //checking that the file is .doc type or .txt type and then reading the Connection file
  ff = fopen(argv[2], "r");
  
  size = 0;
  for(int i = 0; argv[2][i] != '\0'; i ++) size ++;
  if(argv[2][size - 4] == '.' && argv[2][size - 3] == 't' && argv[2][size - 2] == 'x' && argv[2][size - 1] == 't') {
    fscanf(ff, "%d", &connRequests);
  }
  else {
    fscanf(ff, "%s", readText);
    for(int i = 3; readText[i] != '\0'; i ++) {
      connRequests = (connRequests*10) + (readText[i] - '0');
    }
  }

  for(int i = 0; i < connRequests; i ++) {

    fscanf(ff, "%d %d %lf %lf %lf", &src, &desc, &bMin, &bAvg, &bMax);

    //by default pessimistic, if condition satisfies then optimistic
    if(atoi(argv[7]) == 0) {
      if(bMax > bAvg + 0.25 * (bMax - bMin)) {
        bMax = bAvg + 0.25 * (bMax - bMin);
      }
    }

    pathNumber = 1;
    int res = makeConnection();
    if(res == 0) {
      pathNumber = 2;
      res = makeConnection();
    }
    if(res == 1) {
      connectionPathTable.push_back(i);
      connectionPathTable.push_back(src);
      connectionPathTable.push_back(desc);
      connectionPathTable.push_back(labelId ++);
      connectionPathTable.push_back(pathNumber);
      connectionPathTable.push_back(-2);
    }

  }
  fclose(ff);

  //updating the Forwarding Table File
  ff = fopen(argv[4], "w");
  fprintf(ff, "\nInterface in          Label in          Interface out          Label out\n");
  for(int i = 0; i < nodeCount; i ++) {
    fprintf(ff, "\nForwarding Table for Node %d :-\n", i);
    if(forwardingTable[i].size() > 1) {
      for(int j = 0; j < forwardingTable[i].size(); j ++) {
        fprintf(ff, "   %d                     ", forwardingTable[i][j++]);
        fprintf(ff, "%d                   ", forwardingTable[i][j++]);
        fprintf(ff, "%d                   ", forwardingTable[i][j++]);
        fprintf(ff, "%d\n", forwardingTable[i][j++]);

      }
    }
  }
  fclose(ff);

  cout << "\nTotal Admitted Connections :- " << admittedConnections << endl << endl;

  //updating the paths.txt File
  ff = fopen(argv[5], "w");
  fprintf(ff, "%d %d", connRequests, admittedConnections);
  fclose(ff);

  printf("\nConnection Id    Source   Destination                Label List                    Path Cost\n");
  int temp = 0;
  for(int i = 0; i < connectionPathTable.size(); i ++) {
    temp ++;
    if(temp == 4) {
      printf("          ");
      for(int j = 0; j < allLabelsList[connectionPathTable[i]].size(); j ++) {
        if(j == allLabelsList[connectionPathTable[i]].size() - 1) printf("%d", allLabelsList[connectionPathTable[i]][j]);
        else printf("%d-", allLabelsList[connectionPathTable[i]][j]);
      }
      printf("                           ");
    }
    else if(connectionPathTable[i] != -2) {
      if(temp == 5) printf("%d", pathCost[connectionPathTable[i - 3]][connectionPathTable[i]][connectionPathTable[i - 2]]);
      else printf("     %d       ", connectionPathTable[i]);
    }
    else {
      temp = 0;
      printf("\n");
    }
  }
  return 0;
}
