#include "util.h" 

Graph::Graph(size_t a) :
    numberOfVertices(a), activeNumberOfVertices(a), numberOfEdges(0) {
        vertices = new set<size_t>[numberOfVertices];
        for(size_t i=0; i<numberOfVertices; i++) {
            (vertices)[i] = set<size_t>();
        }
    }

Graph::Graph(const Graph & g) :
    numberOfVertices(g.numberOfVertices), activeNumberOfVertices(g.activeNumberOfVertices), numberOfEdges(g.numberOfEdges) {
        vertices = new set<size_t>[numberOfVertices];
        for(size_t i=0; i<numberOfVertices; i++) {
            (vertices)[i] = set<size_t>(g.vertices[i]);
        }
    }

void Graph::removeEdge(Edge e) {
    if(e.v1 >= numberOfVertices || e.v2 >= numberOfVertices || e.v1 == e.v2) {
        cout << "illegal edge in remove Edge" << endl;
        return ;
    }

    bool res = (vertices[e.v1]).erase(e.v2);
    res |= (vertices[e.v2]).erase(e.v1);

    // if that edge is removed, we need to reduce the number of edges
    numberOfEdges -= res;
}

void Graph::addEdge(Edge e) {
    if(e.v1 >= numberOfVertices || e.v2 >= numberOfVertices || e.v1 == e.v2) {
        cout << e.v1 << " " << e.v2 << "illegal edge Add edge" << endl;
        return ;
    }

    std::pair<std::set<size_t>::iterator, bool> ret1, ret2;

    ret1 = (vertices[e.v1]).insert(e.v2);
    ret2 = (vertices[e.v2]).insert(e.v1);

    numberOfEdges += (ret1.second | ret2.second);
}

size_t Graph::degree(size_t vertexID) {
    if(vertexID >= numberOfVertices) {
        cout << "illegal vertex id" << endl;
        return 0;
    }

    return (vertices[vertexID]).size();
}

void Graph::removeVertex(size_t vertexID) {
    if(vertexID >= numberOfVertices) {
        cout << "illegal vertex id" << endl;
        return;
    }
    if (isDeleted(vertexID)) {
        return;
    }
    numberOfEdges -= vertices[vertexID].size();

    set<size_t>::iterator it;
    for (it=vertices[vertexID].begin(); it!=vertices[vertexID].end(); ++it) {
        // Remove the reverse edge
        vertices[*it].erase(vertexID);
    }
    activeNumberOfVertices--;
    vertices[vertexID].clear();
}

void Graph::addVertex(size_t vertexID, set<size_t> & newVertex) {
    if(vertexID >= numberOfVertices) {
        cout << "illegal vertex id" << endl;
        return ;
    }

    if(degree(vertexID) != 0) {
        cout << "adding an existing vertex" << endl;
        return ;
    }

    set<size_t>::iterator it;
    for (it=newVertex.begin(); it!=newVertex.end(); ++it) {
        addEdge(Edge(vertexID, (*it)));
    }
}

void Graph::addVertices(stack<pair<size_t, set<size_t>>> & verticesToAdd){
    while(!verticesToAdd.empty()) {
        pair<size_t, set<size_t>> ver = verticesToAdd.top();
        addVertex(ver.first, ver.second);
        verticesToAdd.pop();
    }
}

bool Graph::findFirstActiveVertex(size_t & startVertexID) {
    startVertexID = 0;
    while((degree(startVertexID) == 0) && startVertexID < numberOfVertices)
        startVertexID ++;
    if(startVertexID == numberOfVertices)
        return false;
    else
        return true;
}

bool Graph::isDeleted(size_t vertexID) {
    return vertices[vertexID].size() == 0;
}

size_t Graph::findActiveVertexNumber() {
    size_t num = 0;
    for(size_t vertexID = 0; vertexID < numberOfVertices; vertexID ++) {
        if(degree(vertexID) > 0)
            num ++;
    }
    return num;
}

size_t Graph::findVertexWithBiggestDegree(size_t & maxDegreeVertexID) {
    size_t maxDegree = 0;
    maxDegreeVertexID = 0;
    for(size_t vertexID=0; vertexID < numberOfVertices; vertexID++) {
        if(degree(vertexID) > maxDegree) {
            maxDegree = degree(vertexID);
            maxDegreeVertexID = vertexID;
        }
    }
    return maxDegree;
}

size_t Graph::findVertexWithMinimalDegree(size_t & minDegreeVertexID) {
    size_t minDegree = numberOfVertices;
    minDegreeVertexID = 0;
    for(size_t vertexID=0; vertexID < numberOfVertices; vertexID++) {
        if(degree(vertexID) > minDegree && degree(vertexID) != 0) {
            minDegree = degree(vertexID);
            minDegreeVertexID = vertexID;
        }
    }
    return minDegree;
}

void Graph::clear() {
    for(size_t i=0; i<numberOfVertices; i++)
        (vertices)[i].clear();

    delete[] vertices;
    numberOfVertices = 0;
    activeNumberOfVertices = 0;
    numberOfEdges = 0;
}

//For MVC
//For edges that one end with degree bigger than 1, and another end with degree equal to 1, the end with degree bigger than 1 would be put into the minimum vertices cover.
//For edges that both ends' degree are equal to 1, any one of the end should be put into the minimum vertices cover.
//Do the process until there is no vertex with one degree
void Graph::screenOutPartOfVertices(VCTYPE & vc, stack<pair<size_t, set<size_t>>> & verticesDeleted) {
    while(true) {
        bool haveDegreeOne = false;
        for(size_t vertexID = 0; vertexID < numberOfVertices; vertexID ++) {
            if(degree(vertexID) == 1) {
                haveDegreeOne = true;
                size_t neighbor = *(vertices[vertexID].begin());
                vc.push(neighbor);
                verticesDeleted.push(make_pair(neighbor, set<size_t>(vertices[neighbor])));
                removeVertex(neighbor);
            }
        }
        if(!haveDegreeOne)
            break;
    }
}

//For MVC
//Getting one vertices cover
//Based on always choose the vertex with biggest degree
void Graph::getOneVerticesCover(VCTYPE & vc) {
    stack<pair<size_t, set<size_t>>> verticesDeleted;
    while(numberOfEdges != 0) {
        size_t maxDegreeVertexID;
        findVertexWithBiggestDegree(maxDegreeVertexID);
        vc.push(maxDegreeVertexID);
        verticesDeleted.push(make_pair(maxDegreeVertexID, set<size_t>(vertices[maxDegreeVertexID])));
        removeVertex(maxDegreeVertexID);
    }

    addVertices(verticesDeleted);
}

//For MVC
//MAXIMUM MATCHING
size_t Graph::getMaximumMatch() {
    Graph g(*this);
    int matchedWith[g.numberOfVertices];
    size_t numberOfMatches=0;

    //init 
    for(int vertexID=0; vertexID<g.numberOfVertices; vertexID++)
        matchedWith[vertexID] = NOTMATCHED;

    //finding a initial matching
    for(int vertexID=0; vertexID<g.numberOfVertices; vertexID++) {
        if(matchedWith[vertexID] == NOTMATCHED) {
            set<size_t>::iterator it;
            for (it=g.vertices[vertexID].begin(); it!=g.vertices[vertexID].end(); ++it) {
                if(matchedWith[*it] == NOTMATCHED) {
                    matchedWith[*it] = vertexID;
                    matchedWith[vertexID] = *it;
                    numberOfMatches++;
                    break;
                }
            }
        }
    }

    stack<size_t> path;
    size_t lookResult = dfs_lookForAugmentPath(g, path, matchedWith);
    
#ifdef DEBUG_
    cout << "----------------out side" << lookResult << endl;
    outputGraph(g);
    for(int i=0; i<g.numberOfVertices; i++)
        cout << i << "matches" << matchedWith[i] << endl;
#endif

    while(lookResult != FIND_NOTHING) {

#ifdef DEBUG
    cout << "in side" << endl;
    for(int i=0; i<g.numberOfVertices; i++)
        cout << i << "matches" << matchedWith[i] << endl;
#endif

        if(lookResult == FIND_PATH) {
            updateMatches(matchedWith, path);
            numberOfMatches++;
        }else if(lookResult == FIND_FLOWER){
            shrinkFlower(g, path);
        }

        clearStack(path);
        lookResult = dfs_lookForAugmentPath(g, path, matchedWith);
    }

#ifdef DEBUG
    cout <<  "size here" << g.numberOfVertices << endl;
    for(int i=0; i<g.numberOfVertices; i++)
        cout << i << "matches" << matchedWith[i] << endl;
#endif

    g.clear();
    return numberOfMatches;
}


//For MVC
//MATCHING
size_t Graph::getOneMatch() {
    int matchedWith[numberOfVertices];
    size_t numberOfMatches=0;

    //init 
    for(int vertexID=0; vertexID<numberOfVertices; vertexID++)
        matchedWith[vertexID] = NOTMATCHED;

    //finding a initial matching
    for(int vertexID=0; vertexID<numberOfVertices; vertexID++) {
        if(matchedWith[vertexID] == NOTMATCHED) {
            set<size_t>::iterator it;
            for (it=vertices[vertexID].begin(); it!=vertices[vertexID].end(); ++it) {
                if(matchedWith[*it] == NOTMATCHED) {
                    matchedWith[*it] = vertexID;
                    matchedWith[vertexID] = *it;
                    numberOfMatches++;
                    break;
                }
            }
        }
    }

    return numberOfMatches;
}

//update Matches based on augment path
void updateMatches(int * matchedWith, stack<size_t> & path) {
    size_t a, b, c;
    while(path.size() > 2) {
        a = path.top();
        path.pop();
        b = path.top();
        path.pop();

        if(matchedWith[a] == b || matchedWith[b] == a) {
            cout << "find matching error" << endl;
            exit(1);
        }

        matchedWith[a] = b;
        matchedWith[b] = a;
    }
}

//shrink Flower in graph to a node
void shrinkFlower(Graph & g, stack<size_t> & path) {
    size_t shrinkTo = path.top();
    path.pop();
    size_t toBeShrink = path.top();
    path.pop();
    set<size_t> edgesToAdd;
    while(toBeShrink != shrinkTo) {
        set<size_t>::iterator it;
        for (it=((g.vertices)[toBeShrink]).begin(); it!=((g.vertices)[toBeShrink]).end(); ++it) {
            edgesToAdd.insert(*it);
        }
        g.removeVertex(toBeShrink);

        toBeShrink = path.top();
        path.pop();
    }

    set<size_t>::iterator it;
    for (it=edgesToAdd.begin(); it!=edgesToAdd.end(); ++it) {
        if(*it != shrinkTo) {
            g.addEdge(Edge(shrinkTo, *it));
        }
    }

    edgesToAdd.clear();
}

void copyVCOutOfOrder(VCTYPE & from, VCTYPE & to){
    VCTYPE temp(from);
    while(!to.empty()) {
        to.pop();
    }

    while(!temp.empty()) {
        to.push(temp.top());
        temp.pop();
    }
}

void outputVC(VCTYPE & vc) {
    VCTYPE temp(vc);
    while(!temp.empty()) {
        cout << temp.top() << "\t";
        temp.pop();
    }
    cout << "\n";
}

Graph inputGraph(string graph_file) {
    ifstream inputfile(graph_file);

    if(!inputfile.is_open()) {
        cout << "opening file false" << endl;
        exit(1);
    }

    //init graph
    size_t numberOfVertices, numberOfEdges, unweighted;
    inputfile >> numberOfVertices;
    inputfile >> numberOfEdges;
    inputfile >> unweighted;

    Graph g(numberOfVertices);

    //input edges' info
    string line;
    std::getline(inputfile, line);
    std::stringstream   linestream(line);

    for(size_t v=1; v<=g.numberOfVertices; v++) {
        string line;
        std::getline(inputfile, line);
        std::stringstream   linestream(line);

        size_t u;
        while(linestream >> u) {
            g.addEdge(Edge(v-1, u-1));
        }
    }

    inputfile.close();
    return g;
}

void outputGraph(Graph & g, string discription) {
    cout << discription << endl;
    cout << "number of vertices : " << g.numberOfVertices << endl;
    for(size_t i=0; i<g.numberOfVertices; i++) {
        if(g.degree(i) > 0) {
            cout << "vertex ID " << i << " with degree " << g.degree(i) << endl;
            set<size_t>::iterator it;
            for (it=((g.vertices)[i]).begin(); it!=((g.vertices)[i]).end(); ++it) {
                cout << *(it) << "\t";
            }
            cout << endl;
        }
    }
}

//Using dfs look for augment path
size_t dfs_lookForAugmentPath(Graph & g, stack<size_t> & path, int * matchedWith) {

    size_t * inPath = new size_t[g.numberOfVertices];
    for(size_t vertexID=0; vertexID<g.numberOfVertices; vertexID++) {
        if(matchedWith[vertexID] == NOTMATCHED)
            inPath[vertexID] = SQUARE;
        else
            inPath[vertexID] = NOTINPATH;
    }

    for(size_t rootID=0; rootID<g.numberOfVertices; rootID++) {
        if(matchedWith[rootID] == NOTMATCHED) {
            //start searching
            stack<size_t> trace;
            trace.push(rootID);
            path.push(rootID);
            path.push(rootID);

            while(trace.size() != 0) {
                size_t vertexID = trace.top();
                std::set<size_t>::iterator child;
                for(child = ((g.vertices)[vertexID]).begin(); child != ((g.vertices)[vertexID]).end(); child++) {
                    size_t childVertexID = *child;

                    if( inPath[childVertexID] == SQUARE) {
                        path.push(childVertexID);
                        clearStack(trace);
                        free(inPath);
                        //If meeting flower
                        if(matchedWith[childVertexID] != NOTMATCHED || childVertexID == rootID)
                            return FIND_FLOWER;
                        //If find another unmatched node
                        return FIND_PATH;
                    }

                    size_t childMatchedWith = matchedWith[*child];
                    //Go to search, If in any path, there is no need to search again
                    if(inPath[childVertexID] == NOTINPATH) {
                        inPath[childVertexID] = TRIANGLE;
                        inPath[childMatchedWith] = SQUARE;
                        trace.push(childMatchedWith);
                        path.push(childVertexID);
                        path.push(childMatchedWith);
                        break;
                    }
                }
                if(child == ((g.vertices)[vertexID]).end()) {
                    trace.pop();
                    path.pop();
                    path.pop();
                }
            }
        }
    }

    free(inPath);

    return FIND_NOTHING;
}

template <typename T>
void clearStack(stack<T> & s) {
    while(s.size() != 0)
        s.pop();
}
