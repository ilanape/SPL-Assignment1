#include "../include/Tree.h"
#include "../include/Session.h"

using namespace std;
/**
 *  THIS IS THE IMPLEMENTATION FILE OF CLASS TREE
 */

/**
 * Constructor.
 * @param rootLabel
 */
Tree::Tree(int rootLabel):node(rootLabel),children(){}

/**
 * Destructor.
 */
Tree::~Tree(){ clear();}

/**
 * Copy Constructor.
 * @param other
 */
Tree::Tree(const Tree& other):Tree(other.node){
    for(Tree* tree:other.children){
        addChild(*tree); //creates new trees as other's and adds them to children
    }
}

/**
 * Copy Assignment Operator
 * @param other
 */
const Tree& Tree::operator=(const Tree& other) {
    if(this != &other){
        clear(); //clears current resources
        node=other.node;
        for(Tree* tree:other.children){ //creates new trees same as other's and adds it to children
            addChild(*tree);
        }
    }
    return *this;
}

/**
 * Move Constructor
 */
Tree::Tree(Tree&& other):Tree(other.node){
    //field update with other's
    children=other.children;
    other.children.clear(); //other's pointers are removed
}

/**
 * Move Assignment Operator
 * @param other
 * @return this
 */
const Tree& Tree::operator=(Tree&& other) {
    if(this != &other) {
        clear(); //clears current resources
        //field update with other's
        node = other.node;
        children=other.children;
        other.children.clear(); //other's pointers are removed
    }
    return *this;
}

/**
 * delete's this resources and clears the vector
 */
void Tree::clear() {
    for(Tree* tree:children){
        if (tree){
            delete tree;
        }
    }
    children.clear();
}

/**
 * Original.
 * creates trees on heap by type.
 * @param session const reference
 * @param rootLabel
 * @return new tree's pointer to heap
 */
Tree* Tree::createTree(const Session& session, int rootLabel){
    Tree* toReturn;
    if(session.getTreeType()==Cycle){
        toReturn = new CycleTree(rootLabel, session.getCycle());
    }
    else if(session.getTreeType()==MaxRank){
        toReturn = new MaxRankTree(rootLabel);
    }
    else toReturn = new RootTree(rootLabel);
    return toReturn;
}

/**
 * Original.
 * creates a clone on heap (as we get a const reference) and adds it to children vector.
 * @param child
 */
void Tree::addChild(const Tree& child){
    Tree* clone= child.clone();
    children.push_back(clone);
}

/**
 * override to original that knows that the tree in already existed on heap, hence no need to clone.
 * assist in BFS creation
 * @param child
 */
void Tree::addChild(Tree* child){ children.push_back(child);}

/**
 * @return this root index
 */
int Tree::getLabel() const{ return node;}

/**
 * provides max rank tree traceTree function data on node's rank.
 * @return size of children vector
 */
int Tree::getNumberOfChildren() const{ return children.size();}

/**
 * cycle tree and max rank tree traceTree function helper.
 * @param place
 * @return const reference to children tree in index place
 */
const Tree& Tree::getChild(int place) const{ return *children[place];}


/**
 *  THIS IS THE IMPLEMENTATION FILE OF CLASS CYCLETREE
 */

/**
* CycleTree Constructor.
*/
CycleTree::CycleTree(int rootLabel, int currCycle):Tree(rootLabel),currCycle(currCycle){}

/**
 * Original.
 * @return node index in cycle's place from left if exists and last from else.
 */
int CycleTree::traceTree(){
    int curr=0; //number of steps indicator
    const Tree* currentTree=this;
    while(curr<currCycle && currentTree->getNumberOfChildren()!=0){
        currentTree=&(currentTree->getChild(0));
        curr=curr+1;
    }
    return currentTree->getLabel();
}

/**
 * adds this clone to heap
 * @return pointer to new cycle tree on heap
 */
Tree* CycleTree:: clone() const {return new CycleTree(*this);}

/**
 *  THIS IS THE IMPLEMENTATION FILE OF CLASS MAXRANKTREE
 */

/**
 * MaxRankTree Constructor.
 */
MaxRankTree::MaxRankTree(int rootLabel):Tree(rootLabel){}

/**
 * Original.
 * @return node index of minimal depth node with max children.
 */
int MaxRankTree::traceTree(){
    //root values as default
    int toReturn=getLabel();
    int maxValue=getNumberOfChildren();
    queue<const Tree*> visit;
    visit.push(this);
    while (!visit.empty()) {
        const Tree* currNode = visit.front();
        visit.pop();
        for (int i=0; i<currNode->getNumberOfChildren();i=i+1) {
            const Tree* child= &(currNode->getChild(i));
            if(child->getNumberOfChildren()>maxValue){
                toReturn=child->getLabel();
                maxValue=child->getNumberOfChildren();
            }
            visit.push(child);
        }
    }
    return toReturn;
}

/**
 * adds this clone to heap
 * @return pointer to new max rank tree on heap
 */
Tree* MaxRankTree:: clone() const {return new MaxRankTree(*this);}

/**
 *  THIS IS THE IMPLEMENTATION FILE OF CLASS ROOTLABELTREE
 */

/**
 * RootTree Constructor.
 */
RootTree::RootTree(int rootLabel):Tree(rootLabel){}

/**
 * @return root's index
 */
int RootTree::traceTree() {return getLabel();}

/**
 * adds this clone to heap
 * @return pointer to new root tree on heap
 */
Tree* RootTree:: clone() const {return new RootTree(*this);}