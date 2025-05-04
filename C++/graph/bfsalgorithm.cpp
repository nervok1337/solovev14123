#include "bfsalgorithm.h"

BFSAlghoritm::BFSAlghoritm(const Graph& graph) : graph(graph) {}

bool BFSAlghoritm::isConnect(Node* start, Node* end) {
	if (!start || !end)
		return false;

	queue<Node*> nodesQueue;
	set<Node*> nodesVisited;

	nodesQueue.push(start);
	nodesVisited.insert(start);
	while (!nodesQueue.empty()) {
		Node* current = nodesQueue.front();
		nodesQueue.pop();
		
		if (current == end)
			return true;
		for (auto it = current->nb_begin(); it != current->nb_end(); ++it) {
			if (nodesVisited.find(*it) == nodesVisited.end()) {
				nodesVisited.insert(*it);
				nodesQueue.push(*it);
			}
		}
	}
	return false;
}

set<Node*> BFSAlghoritm::getComponent(Node* start) {
	set<Node*> component;

	if (!start)
		return component;

	queue<Node*> nodesQueue;
	set<Node*> nodesVisited;
	nodesQueue.push(start);
	nodesVisited.insert(start);

	while (!nodesQueue.empty()) {
		Node* current = nodesQueue.front();
		nodesQueue.pop();
		component.insert(current);

		for (auto it = current->nb_begin(); it != current->nb_end(); ++it) {
			if (nodesVisited.find(*it) == nodesVisited.end()) {
				nodesVisited.insert(*it);
				nodesQueue.push(*it);
			}
		}
	}

	return component;
}