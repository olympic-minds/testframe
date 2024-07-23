#ifndef GRAPH_H_
#define GRAPH_H_

#include "utils.hpp"
#include <numeric>
#include <functional>

class Graph {
public:
    bool directed = false;
    std::vector<std::vector<uint64_t>> graph;

private:
    void printForPromptTo(std::ostream &outputStream) const {
        outputStream << "{";
        for (uint64_t i = 0; i < getNumberOfNodes() ; ++i) {
            outputStream << "{";
            for (uint64_t j = 0; j < graph[i].size(); ++j) {
                outputStream << graph[i][j];
                if (j != graph[i].size() - 1) {
                    outputStream << ",";
                }
            }
            outputStream << "}";
            if (i != getNumberOfNodes()  - 1) {
                outputStream << ",";
            }
        }
        outputStream << "}\n";
    }

    void printForSolutionTo(std::ostream &outputStream) const {
        auto edges = getEdges();

        outputStream << getNumberOfNodes()  << " " << edges.size() << "\n";
        for (auto edge : edges) {
            outputStream << edge.first << " " << edge.second << "\n";
        }
    }

public:
    Graph(std::vector<std::vector<uint64_t>> g, bool directed = false)
        : directed(directed),
          graph(g) {}

    uint64_t getNumberOfNodes() const { return graph.size() ; }

    uint64_t getNumberOfEdges() const { 
        return std::accumulate(graph.begin(), graph.end(), 0, [](const uint64_t x, const auto &a) { 
            return x + a.size(); 
        }); 
    }

    bool operator==(const Graph &other) const {
        if (getNumberOfNodes()  != other.getNumberOfNodes() ) {
            return false;
        }

        for (uint64_t node = 0; node < getNumberOfNodes() ; ++node) {
            if (graph[node] != other.graph[node]) {
                return false;
            }
        }
        return true;
    }

    operator std::vector<std::vector<uint64_t>>() const { return graph; }

    std::vector<std::pair<uint64_t, uint64_t>> getEdges() const {
        std::vector<std::pair<uint64_t, uint64_t>> edges;
        for (uint64_t v = 0; v < getNumberOfNodes() ; ++v)
            for (uint64_t u : graph[v]) edges.emplace_back(v, u);
        return edges;
    }

    Graph &relabelNodes();

    void printTo(std::ostream &outputStream, PrintFormat format) const {
        switch (format) {
            case Prompt:
                printForPromptTo(outputStream);
                break;
            case Solution:
                printForSolutionTo(outputStream);
                break;
        }
    }

    static Graph readGraph(std::istream &inputStream) {
        uint64_t nodes, numberOfEdges;
        inputStream >> nodes >> numberOfEdges;
        std::vector<std::vector<uint64_t>> g(nodes);
        for (uint64_t i = 0; i < numberOfEdges; i++) {
            uint64_t a, b;
            inputStream >> a >> b;
            g[a].push_back(b);
        }
        return Graph(g);
    }

    static Graph constructEmptyGraph(uint64_t nodes);
    static Graph constructUndirectedClique(uint64_t nodes);
    static Graph constructPathGraph(uint64_t nodes, uint64_t numberOfComponents = 1);
    static Graph constructShallowForestGraph(uint64_t nodes, uint64_t numberOfTrees);
    static Graph constructShallowTreeGraph(uint64_t nodes);
    static Graph constructForestGraph(uint64_t nodes, uint64_t numberOfTrees);
    static Graph constructTreeGraph(uint64_t nodes);
    static Graph constructSimplerJellyfishGraph(uint64_t nodes, uint64_t cycleSize, uint64_t maxTentacleLength,
                                                   uint64_t numberOfTentacles);
    static Graph constructStarfishGraph(uint64_t nodes, uint64_t maxRayLength, uint64_t numberOfRays);
    static Graph constructSilkwormGraph(uint64_t nodes);
    static Graph constructTreeOfBoundedDegreeGraph(uint64_t nodes, uint64_t minDegree, uint64_t maxDegree);
    static Graph constructSparseGraph(uint64_t nodes);
    static Graph constructDenseGraph(uint64_t nodes);

    bool isClique() {
        uint64_t numberOfEdges = getNumberOfEdges(); 
        return (directed ? numberOfEdges : numberOfEdges * 2) == getNumberOfNodes()  * (getNumberOfNodes()  - 1); 
    }

    bool isConnected() { return undirectedConnectedComponentsNumber() == 1; }

    uint64_t undirectedConnectedComponentsNumber();
};

#endif
