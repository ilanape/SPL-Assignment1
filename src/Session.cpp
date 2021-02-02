#include "../include/Session.h"
#include "../include/Agent.h"
#include <vector>
#include <queue>
#include <fstream>

using namespace std;
using json = nlohmann::json; // for convenience

/**
 *  THIS IS THE IMPLEMENTATION FILE OF CLASS SESSION
 */

/**
 * Constructor.
 * @param path
 */
Session::Session(const std::string& path):g({}),treeType(),agents(),infectionQueue(), cycle(0){
    ifstream file(path);
    json input;
    file >> input;
    g=Graph(input["graph"]); //gets the graph from the json input
    //treeType update
    if(input["tree"]=="M"){treeType=MaxRank;}
    else if(input["tree"]=="R") {treeType=Root;}
    else {treeType=Cycle;}
    //agents pointer vector update
    for(auto &element:input["agents"]){
        if(element[0]=="V"){
            Virus toAdd(element[1]);
            addAgent(Virus(element[1])); //adds to heap
            g.setCarrier(element[1]);
        }
        else addAgent(ContactTracer()); //adds to heap
    }
}

/**
 * Destructor
 */
Session::~Session(){ clear();}

/**
 * Copy Constructor
 * @param other
 */
Session::Session(const Session& other):g({}),treeType(),agents({}),infectionQueue(),cycle(0){
    g=other.g;
    treeType=other.getTreeType();
    infectionQueue=other.infectionQueue;
    cycle=other.getCycle();
    for(Agent* agent:other.agents){
        addAgent(*agent); //creates new agent and adds it to agents
    }
}

/**
 * Copy Assignment Operator
 * @param other
 */
const Session& Session::operator=(const Session& other) {
    if(this != &other) {
        clear(); //clears current resources
        //field update with other's
        g = other.g;
        treeType = other.getTreeType();
        infectionQueue = other.infectionQueue;
        cycle = other.getCycle();
        for (Agent *agent:other.agents) { //creates new agents same as other's and adds it to agents
            addAgent(*agent);
        }
    }
    return *this;
}

/**
 * Move Constructor
 */
Session::Session(Session&& other):g({}),treeType(),agents({}),infectionQueue(),cycle(0){
    //field update with other's
    g=other.g;
    treeType= other.getTreeType();
    infectionQueue=other.infectionQueue;
    cycle=other.getCycle();
    agents= other.agents;
    other.agents.clear(); //other's pointers are removed
}

/**
 * move assignment operator
 * @param other
 * @return this
 */
const Session& Session::operator=(Session&& other) {
    if(this != &other) {
        clear(); //clears current resources
        //field update with other's
        g = other.g;
        treeType = other.getTreeType();
        infectionQueue = other.infectionQueue;
        cycle = other.getCycle();
        agents= other.agents;
        other.agents.clear(); //other's pointers are removed
    }
    return *this;
}

/**
 * delete's this resources and clears the vector
 */
void Session::clear() {
    for(Agent* agent:agents)
        if (agent){
            delete agent;
        }
    agents.clear();
}

/**
 * starts the program by activating the agents in order
 */
void Session::simulate() {
    while(!terminationCond()){
        size_t size=agents.size(); //being updated with each cycle
        for(size_t i=0; i<size; i=i+1){
            agents[i]->act(*this); //agents activation in order
        }
        cycle=cycle+1;
    }
    //output.json creation
    json out;
    out["graph"]=g.getEdges();
    vector<int> infectedList;
    for(int i=0; i<g.getSize(); i=i+1){ //creates a list of infected nodes
        if(g.isInfected(i))
            infectedList.push_back(i);
    }
    out["infected"]=infectedList;
    ofstream i("./output.json");
    i << out;
}

/**
 * Original.
 * creates a clone on heap (as we get a const reference) and adds it to agent vector.
 * @param agent
 */
void Session::addAgent(const Agent &agent) {
    Agent* clone=agent.clone();
    agents.push_back(clone);
}

/**
 * Original.
 * adds i to infection queue.
 * @param i - infected node index
 */
void Session::enqueueInfected(int i){ infectionQueue.push(i); }

/**
 * Original.
 * pops first infected node's index from quque.
 * @return
 */
int Session::dequeueInfected(){
    int toPop=infectionQueue.front();
    infectionQueue.pop();
    return toPop;
}

/**
 * Based on BFS algorithm.
 * Checks if each connectivity component in graph is either fully infected or fully healthy.
 * if so, indicates to terminate the program.
 * @return boolean
 */
bool Session::terminationCond(){
    vector<string> colors(g.getSize(),"White"); //visiting status
    for(int node=0; node<g.getSize(); node=node+1){
        if(colors[node]=="White") { //new component
            colors[node] = "Gray"; //visited
            string componentStatus = g.getStatus(node); //representative
            if (componentStatus == "Carrier") return false; //not finished
            //Infected or Carrier
            queue<int> visit;
            visit.push(node);
            while (!visit.empty()) { //same connectivity component
                int curr = visit.front();
                visit.pop();
                vector<int> neighborsOfCurr = g.getNeighbors(curr);
                for (int element:neighborsOfCurr) {
                    colors[element] = "Gray";
                    if (g.getStatus(element) != componentStatus) return false; //different status - not finished
                }
            }//end of while
        }
    }
    return true;
}

/**
 * BFS.
 * @param source node index.
 * @return pointer to BFS tree (on heap) from given node in compatible type.
 */
Tree* Session:: BFS(int source) {
    Tree* root= Tree::createTree(*this, source);
    vector<string> colors(g.getSize(),"White"); //visiting status initialization
    colors[source] = "Gray";
    queue<Tree*> visit;
    visit.push(root);
    while (!visit.empty()) {
        Tree* currTree=visit.front();
        int currNodeInd =currTree->getLabel();
        visit.pop();
        for (int neighbor: g.getNeighbors(currNodeInd)) {
            if (colors[neighbor] == "White") {//new visit
                colors[neighbor] = "Gray";
                Tree* neighborTree= Tree::createTree(*this, neighbor); //neighbor is a child tree
                visit.push(neighborTree);
                currTree->addChild(neighborTree); //add child which receives a pointer - knows that its already on heap
            }
        }
    }
    return root;
}

/**
 * returns to contact tracer's act function if it can still act
 * @return false if can continue, true if no action needs to be performed
 */
bool Session::isEmptyIQ(){ return infectionQueue.empty();}

/**
 * Original.
 * @return this tree type
 */
TreeType Session::getTreeType() const { return treeType;}

/**
 * Get graph
 * @return const reference to this session's graph
 */
const Graph& Session::getGraph() const{return g;}

/**
 * eases status check in virus act from agent class
 * @param nodeInd
 * @return this node's health status
 */
string Session::getStatus(int nodeInd) const{ return g.getStatus(nodeInd);}

/**
 * helps cycle tree's traceTree function
 * @return cycle number
 */
int Session::getCycle() const{ return cycle;}

/**
 * Original.
 * sets updated graph to be current graph (as get graph is const and can't have changed directly on it)
 * @param graph
 */
void Session::setGraph(const Graph &graph) {g=graph;}

/**
 * eases node's status update in graph from virus's act in agent class
 * @param nodeInd
 */
void Session::setInfected(int nodeInd){ g.infectNode(nodeInd); }



