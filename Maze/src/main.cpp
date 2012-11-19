#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/graphviz.hpp>
#include <iostream>
#include <fstream>
#include <map>
#include <stack>
#include <string>
#include <vector>

using namespace boost;
using namespace std;

typedef adjacency_list<listS, vecS, undirectedS> MyGraph;
typedef graph_traits<MyGraph>::vertex_descriptor MyVertex;
typedef graph_traits<MyGraph>::edge_descriptor MyEdge;

class GraphEdge {
  public:
    GraphEdge(int v, int v2, int ec, char c, char t) :
      vertex(v),
      vertex2(v2),
      edgeCount(ec),
      color(c),
      type(t),
	  parent(-1) {}

   int getStartVertex() { return vertex; }
   int getEndVertex() { return vertex2; }
   int getEdgeCount() { return edgeCount; }
   char getColor() { return color; }
   char getType() { return type; }
   int getParent() { return parent; }
   void setParent(int e) { parent = e; }

  private:
    int vertex, vertex2, edgeCount;
    char color, type;
	int parent;
};

typedef map<int, char> ReverseVertexMap;
typedef map<char, int> VertexMap;
typedef vector<GraphEdge> VectorEdges;

class MyVisitor : public default_dfs_visitor {


	public:
    MyVisitor(int si, int ei, VertexMap vm, ReverseVertexMap rvm, VectorEdges ie) :
      startIndex(si),
      endIndex(ei),
      vertexMap(vm),
      reverseVertexMap(rvm),
      inputEdges(ie),
      recording(false) {}

	void tree_edge(MyEdge e, const MyGraph& g) {
		int sourceIndex = source(e, g);
		int targetIndex = target(e, g);
		cout << "Going from : " << sourceIndex << " to " << targetIndex << endl;
	}

  private:
    int startIndex, endIndex;
    bool recording;
    vector<int> vertices;
    VertexMap vertexMap;
    ReverseVertexMap reverseVertexMap;
    VectorEdges inputEdges;
};

const string FILENAME = "data/input.txt";


int readInputFile(MyGraph& g, VertexMap& vertices, ReverseVertexMap& reverseVertices, VectorEdges& inputEdges);
void processAndAddEdges(MyGraph& g, VertexMap& vertices, VectorEdges& inputEdges);
bool compareTwoEdges(GraphEdge e, GraphEdge e2);
int mapEdgeIndex(int index, bool isLeavingStart);


int main() {
  ReverseVertexMap reverseVertices;
	VertexMap vertices;
  VectorEdges inputEdges;
	MyGraph g;
  int numEdges = readInputFile(g, vertices, reverseVertices, inputEdges);
  processAndAddEdges(g, vertices, inputEdges); 
  cout << "Edges again: " << numEdges << endl;
	MyVisitor vis(0, numEdges, vertices, reverseVertices, inputEdges);
  depth_first_search(g, boost::visitor(vis));


	return 0;
}

int readInputFile(MyGraph& g, VertexMap& vertices, ReverseVertexMap& reverseVertices, VectorEdges& inputEdges) {
	ifstream fileHandle(FILENAME.c_str());
	if (!fileHandle) {
		cerr << "Error opening input file\n";
		exit(1);
	}
	int vertexCount = 0;
  int edgeCount = 0;
	int numVillages, numEdges;
	fileHandle >> numVillages >> numEdges;
	for (int i = 0; i < numEdges; ++i) {
		char v1, v2, color, type;
		fileHandle >> v1 >> v2 >> color >> type;

		//Check if we have a vertext number for each line read
		if (vertices.find(v1) == vertices.end()) {
			vertices[v1] = vertexCount++;
      reverseVertices[vertexCount] = v1;
    }
		if (vertices.find(v2) == vertices.end()) {
			vertices[v2] = vertexCount++;
      reverseVertices[vertexCount] = v2;
    }
    
//    cout << "Recording edge from " << v1 << " to " << v2 << " index: " << edgeCount << endl;
    inputEdges.push_back(GraphEdge(vertices[v1], vertices[v2], edgeCount++, color, type));
		//create the edge between the two vertices
		//add_edge(vertices[v1], vertices[v2], g);

	}

	fileHandle.close();

  return mapEdgeIndex(edgeCount -1, true);
}

void processAndAddEdges(MyGraph& g, VertexMap& vertices, VectorEdges& inputEdges) {
  VectorEdges::iterator it;
  int i = 0;
  for (it = inputEdges.begin(); it != inputEdges.end(); ++it) {
    GraphEdge startNode = *it;

    VectorEdges::iterator it2;
    for (it2 = inputEdges.begin(); it2 != inputEdges.end(); ++it2) {
      if (it != it2) {
        GraphEdge endNode = *it2;
        
        if (startNode.getStartVertex() == endNode.getStartVertex() &&
            compareTwoEdges(startNode, endNode)) {
          
          //map the start node (which is traversing backwards) to the end node (which you are traversing normally)
          add_edge(mapEdgeIndex(startNode.getEdgeCount(), false), mapEdgeIndex(endNode.getEdgeCount(), true), g);
          /*
          cout << "Adding a backwards edge ";
          cout << "(" << mapEdgeIndex(startNode.getEdgeCount(), true) << ", " << mapEdgeIndex(endNode.getEdgeCount(), true) << ")\n";
          */
        } else if (startNode.getEndVertex() == endNode.getStartVertex() &&
          compareTwoEdges(startNode, endNode)) {
          //map the start node and end node; both of which are traversed normally
          add_edge(mapEdgeIndex(startNode.getEdgeCount(), true), mapEdgeIndex(endNode.getEdgeCount(), true), g);
          /*
          cout << "Adding a normal edge ";
          cout << "(" << mapEdgeIndex(startNode.getEdgeCount(), true) << ", " << mapEdgeIndex(endNode.getEdgeCount(), true) << ")\n";
          */
        }

      }
    }
  }
}

bool compareTwoEdges(GraphEdge e, GraphEdge e2) {
  if (e.getColor() == e2.getColor() ||
      e.getType() == e2.getType()) {
    return true;
  }
  return false;
}

int mapEdgeIndex(int index, bool isLeavingStart) {
  if (isLeavingStart)
    return index * 2;
  return (index * 2) + 1;
}

int reverseMapEdgeIndex(int index) {
  if (index % 2 == 0) {
    return (index / 2);
  }
  return ((index - 1) /2);
}
