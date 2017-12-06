#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream> 
#include <vector>
#include <string>
#include <cstring>
#include <ctime>
#include <fstream>
#include <sstream>
#include <queue>
#include <set>
#include <stack>
#include <utility>

#define BnB "BnB"
#define APPROX "Approx"
#define LS1 "LS1"
#define LS2 "LS2"

#define NOTMATCHED -1
#define FIND_PATH 1
#define FIND_FLOWER 2
#define FIND_NOTHING 0

#define NOTINPATH 0
#define SQUARE 1
#define TRIANGLE 2

#define TEST_TIME  1 
//#define DEBUG 

using namespace std;

//Since we are searching the result with DFS(for branch and bound), so we use stack to store the result.
//FIXME : TBD
typedef stack<size_t> VCTYPE;

void copyVCOutOfOrder(VCTYPE & from, VCTYPE & to);
void outputVC(VCTYPE & vc);

struct Edge {
    size_t v1;
    size_t v2;
    
    Edge( size_t a=0, size_t b=0 ):
    v1(a), v2(b) {}
};

//Graph
class Graph {
    public:
    size_t numberOfVertices; //it is settled from the start and wouldn't change with update.
    size_t numberOfEdges; //can be used to judge the end of searching. That is all edges are deleted(covered).
    size_t activeNumberOfVertices;
    //the reason why we don't use matrix to store is that most of our testing graphs seem like very sparse.
    set<size_t> * vertices;

    //To store the sign of which vertices are active to avoid delete vertices derectly from Graph when search for result.
    //Active vertex mean not delete(vertices are deteled becasue 1. be pushed in to vertex cover set 2.its edges are all got covered)

    Graph(size_t a);
    Graph(const Graph & g);

    void removeEdge(Edge e); 
    void addEdge(Edge e); 
    void removeVertex(size_t vertexID); 
    void addVertex(size_t vertexID, set<size_t> & newVertex);
    void addVertices(stack<pair<size_t, set<size_t>>> & verticesToAdd);

    size_t degree(size_t vertexID); 

    bool findFirstActiveVertex(size_t & startVertexID);
    bool isDeleted(size_t vertexID);

    size_t findActiveVertexNumber();
    size_t findVertexWithBiggestDegree(size_t & maxDegreeVertexID);
    size_t findVertexWithMinimalDegree(size_t & minDegreeVertexID);
    void clear();

    //For MVC
    //For edges that one end with degree bigger than 1, and another end with degree equal to 1, the end with degree bigger than 1 would be put into the minimum vertices cover.
    //For edges that both ends' degree are equal to 1, any one of the end should be put into the minimum vertices cover.
    //Do the process until there is no vertex with one degree
    void screenOutPartOfVertices(VCTYPE & vc, stack<pair<size_t, set<size_t>>> & verticesDeleted);
   
    //For MVC
    //Getting one vertices cover
    //Based on always choose the vertex with biggest degree
    void getOneVerticesCover(VCTYPE & vc); 

    //For MVC
    //MAXIMUM MATCHING
    size_t getMaximumMatch();
    size_t getOneMatch();
};

//input graph from file
Graph inputGraph(string graph_file);

//output graph
//For testing usage
void outputGraph(Graph & g, string discription = "");

//update Matches based on augment path
void updateMatches(int * matchedWith, stack<size_t> & path);
//shrink Flower in graph to a node
void shrinkFlower(Graph & g, stack<size_t> & path);
 //Using dfs look for augment path
size_t dfs_lookForAugmentPath(Graph & g, stack<size_t> & path, int * matchedWith);
//clear stack
template <typename TYPENAME>
void clearStack(stack<TYPENAME> & s); 
