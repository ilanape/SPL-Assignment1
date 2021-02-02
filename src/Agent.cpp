#include "../include/Agent.h"

using namespace std;

/**
 *  THIS IS THE IMPLEMENTATION FILE OF CLASS AGENT
 */
/**
* Constructor.
*/
Agent::Agent(){}

/**
 * Destructor.
 * Abstract class.
 */
Agent::~Agent(){}

/**
 *  THIS IS THE IMPLEMENTATION FILE OF CLASS CONTACTTRACER
 */
 /**
  * Constructor.
  */
ContactTracer::ContactTracer(){}

/**
 * Original.
 * performs contact tracers moves
 * @param session
 */
void ContactTracer::act(Session& session){
    if(!session.isEmptyIQ()){ //need to act
        int infectedNode= session.dequeueInfected();
        Tree* BFSTree= session.BFS(infectedNode); //based on infected node from queue
        int toRemove=BFSTree->traceTree();
        Graph toUpdate= session.getGraph(); //copy graph as get graph is a const reference
        toUpdate.removeNeighbors(toRemove); //edges removal
        session.setGraph(toUpdate);
        delete BFSTree; //receives it pointer, hence is responsible to delete it after use
    }
}

/**
 * adds this clone to heap
 * @return pointer to new agent on heap
 */
Agent* ContactTracer:: clone() const{ return new ContactTracer(*this);}

/**
 *  THIS IS THE IMPLEMENTATION FILE OF CLASS VIRUS
 */
 /**
  * Constructor.
  * @param nodeInd
  */
Virus::Virus(int nodeInd):nodeInd(nodeInd){}

/**
 * performs virus's moves
 * @param session
 */
void Virus::act(Session& session){
    if(session.getStatus(nodeInd)!="Infected") {//carrier
        session.setInfected(nodeInd); //becomes Infected
        session.enqueueInfected(nodeInd); //enters infection queue
    }
    //happens in any case (carrier or infected)
    Graph g=session.getGraph(); //copy on which changes are performed
    vector<int> neighbors = g.getNeighbors(nodeInd);
    for(int neighbor: neighbors) {
        if (session.getStatus(neighbor) == "Healthy") { //can spread to him
            Virus newVirus = Virus(neighbor);
            session.addAgent(newVirus); //adds to heap
            g.setCarrier(neighbor); //update
            session.setGraph(g); //update graph with updated graph
            break;
        }
    }
}

/**
 * adds this clone to heap
 * @return pointer to new agent on heap
 */
Agent* Virus:: clone() const{ return new Virus(*this);}
