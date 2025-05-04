#include "graph.h"

Node* Graph::findNode(const string& name) const {
	for (Node* node : nodes) {
		if (node->getName() == name)
			return node;
	}
	return nullptr;
}

Graph::Graph(const string& file_name) {
	ifstream file(file_name);
	if (!file.is_open())
		throw Exception();

	string line;
	getline(file, line);
	while (getline(file, line)){
		istringstream in(line);
		string from, to;
		in >> from >> to;
		if (!in)
			continue;

		Node* fromNode = findNode(from);
		if (!fromNode) {
			fromNode = new Node(from);
			addNode(fromNode);
		}
		Node* toNode = findNode(to);
		if (!toNode) {
			toNode = new Node(to);
			addNode(toNode);
		}

		addEdge(fromNode, toNode);
	}
}
Graph::~Graph() {
	for (auto node : nodes)
		delete node;
}

void Graph::addNode(Node* node) {
	nodes.insert(node);
}
void Graph::removeNode(Node* node) {
	nodes.insert(node);
	for (auto neighbourNode : nodes)
		neighbourNode->removeNeighbour(node);
}

void Graph::addEdge(Node* begin, Node* end) {
	if (!begin || !end) return;
	if (nodes.find(begin) == nodes.end()) return;
	if (nodes.find(end) == nodes.end()) return;

	begin->addNeighbour(end);
	end->addNeighbour(begin);
}
void Graph::removeEdge(Node* begin, Node* end) {
	if (begin)
		begin->removeNeighbour(end);
	if (end)
		end->removeNeighbour(begin);
}

Graph::iterator Graph::begin() const {
	return nodes.cbegin();
}
Graph::iterator Graph::end() const {
	return nodes.cend();
}


