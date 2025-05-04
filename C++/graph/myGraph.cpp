#include "Graph.h"
#include "bfsalgorithm.h" 

void resultTask(const Graph& graph, const string& file_name);

int main() {
    Graph testGraph("testgraph.txt");
    resultTask(testGraph, "testgraph");

    Graph bigGraph("1000.csv");
    resultTask(bigGraph, "1000");

    return 0;
}

void resultTask(const Graph& graph, const string& file_name) {
    set<Node*> unvisited(graph.begin(), graph.end());
    BFSAlghoritm bfs(graph);
    int index = 0;

    while (!unvisited.empty()) {
        Node* start = *unvisited.begin();
        set<Node*> component = bfs.getComponent(start);

        string result_file_name = file_name + "_" + to_string(index++) + ".txt";
        ofstream out(result_file_name);
        out << "from\tto\n";
        for (Node* node : component) {
            for (auto it = node->nb_begin(); it != node->nb_end(); ++it) {
                if (component.count(*it) && node->getName() < (*it)->getName()) {
                    out << node->getName() << "\t" << (*it)->getName() << "\n";
                }
            }
        }
        for (Node* n : component) {
            unvisited.erase(n);
        }
    }
}
