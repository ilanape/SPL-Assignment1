#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <string>

using namespace std;

class Graph{
public:
    Graph(std::vector<std::vector<int>> matrix);

    //originals
    void infectNode(int nodeInd);
    bool isInfected(int nodeInd);

    //getters
    const vector<vector<int>>& getEdges() const;
    vector<int> getNeighbors(int nodeInd) const;
    string getStatus(int nodeInd) const;
    int getSize() const;

    //setters
    void setCarrier(int nodeInd);
    void removeNeighbors(int nodeInd);

private:
    std::vector<std::vector<int>> edges;

    vector<std::string> nodesStatus; //"Healthy", "Carrier", "Infected"
};

#endif