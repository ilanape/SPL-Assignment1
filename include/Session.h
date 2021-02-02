#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <string>
#include <queue>
#include "Graph.h"
#include "Tree.h"
#include "json.hpp"

using namespace std;

class Agent;

enum TreeType{
    Cycle,
    MaxRank,
    Root
};

class Session{
public:
    Session(const std::string& path);
    //rule of 5
    virtual ~Session(); //destructor
    Session(const Session& other); //copy constructor
    const Session& operator=(const Session& other); //copy assignment operator
    Session(Session&& other); //move constructor
    const Session& operator=(Session&& other); //move assignment operator

    //originals
    void simulate();
    void addAgent(const Agent& agent);
    void setGraph(const Graph& graph);
    void enqueueInfected(int);
    int dequeueInfected();
    TreeType getTreeType() const;

    Tree* BFS(int source);
    bool isEmptyIQ();

    //getters
    const Graph& getGraph() const;
    string getStatus(int nodeInd) const;
    int getCycle() const;

    //Setters
    void setInfected(int nodeInd);

private:
    bool terminationCond();
    void clear();

    Graph g;
    TreeType treeType;
    std::vector<Agent*> agents;

    queue<int> infectionQueue;
    int cycle;
};

#endif