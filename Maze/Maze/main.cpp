#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/graphviz.hpp>
#include <iostream>
#include <fstream>
#include <map>
#include <string>

using namespace boost;
using namespace std;

//map a vertex indicated by a character to an integer value


typedef adjacency_list<boost::listS, boost::vecS, boost::directedS> MyGraph;
typedef graph_traits<MyGraph>::vertex_descriptor MyVertex;

const string FILENAME = "input.txt";

class MyVisitor : public default_dfs_visitor {


	public:
		void discover_vertex(MyVertex v, const MyGraph& g) {
			cerr << v << endl;
			return;
		}
};

void readInputFile(MyGraph& g, map<char, int>& vertices);

int main() {
	map<char, int> vertices;
	MyGraph g;
	readInputFile(g, vertices);
	MyVisitor vis;
	//boost::depth_first_search(g, boost::visitor(vis));

	return 0;
}

void readInputFile(MyGraph& g, map<char, int>& vertices) {
	ifstream fileHandle(FILENAME.c_str());
	if (!fileHandle) {
		cerr << "Error opening input file\n";
		exit(1);
	}
	int vertexCount = 0;
	int numVillages, numEdges;
	fileHandle >> numVillages >> numEdges;
	for (int i = 0; i < numEdges; ++i) {
		char v1, v2, color, type;
		fileHandle >> v1 >> v2 >> color >> type;

		//Check if we have a vertext number for each line read
		if (vertices.count(v1) > 0)
			vertices[v1] = vertexCount++;
		if (!vertices.count(v2))
			vertices[v2] = vertexCount++;

		//create the edge between the two vertices
		add_edge(vertices[v1], vertices[v2], g);

	}
	fileHandle.close();
}