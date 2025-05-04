#ifndef BFSALGORITHM_H
#define BFSALGORITHM_H
#include "Graph.h"
#include <queue>
#include <set>

class BFSAlghoritm {
private:
	const Graph& graph;
public:
	explicit BFSAlghoritm(const Graph& graph);
	bool isConnect(Node* start, Node* end);
	set<Node*> getComponent(Node* start);
};

#endif
