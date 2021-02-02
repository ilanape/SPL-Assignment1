#ifndef TREE_H_
#define TREE_H_

#include <vector>

class Session;

class Tree{
public:
    Tree(int rootLabel);
    //rule of 5
    virtual ~Tree(); //destructor
    Tree(const Tree& other); //copy constructor
    const Tree& operator=(const Tree& other); //copy assignment operator
    Tree(Tree&& other); //move constructor
    const Tree& operator=(Tree&& other); //move assignment operator

    //originals
    void addChild(const Tree& child);
    static Tree* createTree(const Session& session, int rootLabel);
    virtual int traceTree()=0;

    void addChild(Tree* child);
    virtual Tree* clone() const=0;

    //getters
    const Tree& getChild(int place) const;
    int getLabel() const;
    int getNumberOfChildren() const;

private:
    void clear();

    int node;
    std::vector<Tree*> children;
};

class CycleTree: public Tree{
public:
    CycleTree(int rootLabel, int currCycle);

    //original
    virtual int traceTree();

    virtual Tree* clone() const;

private:
    int currCycle;
};

class MaxRankTree: public Tree{
public:
    MaxRankTree(int rootLabel);

    //original
    virtual int traceTree();

    virtual Tree* clone() const;
};

class RootTree: public Tree{
public:
    RootTree(int rootLabel);

    //original
    virtual int traceTree();

    virtual Tree* clone() const;
};

#endif