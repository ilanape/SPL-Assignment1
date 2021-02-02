#include "../include/Graph.h"
#include <vector>

using namespace std;

/**
 *  THIS IS THE IMPLEMENTATION FILE OF CLASS GRAPH
 */
 /**
  * Constructor.
  * @param matrix
  */
Graph::Graph(std::vector<std::vector<int>> matrix):edges(matrix), nodesStatus(matrix.size(),"Healthy"){}

/**
 * Original.
 * changes node's status to infected from virus's act on it
 * @param nodeInd
 */
void Graph::infectNode(int nodeInd){ nodesStatus[nodeInd]="Infected";}

/**
 * Original.
 * helps create infection list in the end.
 * @param nodeInd
 * @return true if infected and false otherwise
 */
bool Graph::isInfected(int nodeInd) {return nodesStatus[nodeInd]=="Infected";}

/**
 * BFS assistant
 * @param nodeInd
 * @return neighbors of nodeInd
 */
vector<int> Graph::getNeighbors(int nodeInd) const{
    vector<int> neighbors;
    for(size_t i=0; i<edges[nodeInd].size(); i=i+1){
        if(edges[nodeInd][i]==1) neighbors.push_back(i);
    }
    return neighbors;
}

/**
 * sets node to be carrier when it has a new virus on it.
 * @param nodeInd
 */
void Graph::setCarrier(int nodeInd){ nodesStatus[nodeInd]="Carrier";}

/**
 * Updates graph by contact tracer's decision who to disconnect
 * @param nodeInd
 */
void Graph::removeNeighbors(int nodeInd) {
    for (size_t i = 0; i <edges[nodeInd].size(); i = i + 1) {
        edges[nodeInd][i] = 0;
        edges[i][nodeInd] = 0;
    }
}

/**
 * Termination condition helper
 * @param nodeInd
 * @return
 */
string Graph::getStatus(int nodeInd) const{ return nodesStatus[nodeInd];}

/**
 * BFS bases function assistant to initialize visiting vector
 * @return
 */
int Graph::getSize() const { return edges.size();}

/**
 * In use in output.json file creation
 * @return const reference to graphs edges
 */
const vector<vector<int>>& Graph:: getEdges() const{ return edges;}
