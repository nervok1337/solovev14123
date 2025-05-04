#ifndef NODE_H
#define NODE_H
#include <string>
#include <set>
using namespace std;

class Node {
private:
	string name;
	set<Node*> neighbours;

	void addNeighbour(Node* neighbour);
	void removeNeighbour(Node* neighbour);
public:
	explicit Node(const std::string& name);
	~Node() = default;

	const string& getName() const;

	using iterator = set<Node*>::const_iterator;
	iterator nb_begin() const;
	iterator nb_end() const;
	friend class Graph;
};

#endif // !_NODE_H