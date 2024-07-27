#ifndef GRAPH_H_
#define GRAPH_H_

#include "utils.hpp"
#include <numeric>
#include <functional>

class Graph {
public:
    bool directed = false;
    std::vector<std::vector<uint64_t>> graph;
    enum class PrintFormat {
        PromptAdjecencyList,
        SolutionAdjecencyList,
        PromptAdjecencyMatrix,
        SolutionAdjecencyMatrix
    };

private:
    void printPromptAdjecencyListTo(std::ostream &outputStream) const {
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

    void printPromptAdjecencyMatrixTo(std::ostream &outputStream) const {
        auto nodes = getNumberOfNodes();

        std::vector<std::vector<int>> matrix(nodes, std::vector<int>(nodes, 0));

        for (auto [from, to] : getEdges()) {
            matrix[from][to] = 1;
        }
        
        outputStream << "{";
        for (uint64_t i = 0; i < nodes ; ++i) {
            outputStream << "{";
            for (uint64_t j = 0; j < nodes; ++j) {
                outputStream << matrix[i][j];
                if (j != graph[i].size() - 1) {
                    outputStream << ",";
                }
            }
            outputStream << "}";
            if (i != nodes  - 1) {
                outputStream << ",";
            }
        }
        outputStream << "}\n";
    }

    void printSolutionAdjecencyMatrixTo(std::ostream &outputStream) const {
        uint64_t nodes = getNumberOfNodes();
        outputStream << nodes  << "\n";
        
        std::vector<std::vector<int>> matrix(nodes, std::vector<int>(nodes, 0));
        for (auto [from, to] : getEdges()) {
            matrix[from][to] = 1;
        }

        for (uint64_t i = 0; i < nodes ; ++i) {
            for (uint64_t j = 0; j < nodes; ++j) {
                outputStream << matrix[i][j];
                if (j != graph[i].size() - 1) {
                    outputStream << " ";
                }
            }
            outputStream << "\n";
        }
    }

    void printSolutionAdjecencyListTo(std::ostream &outputStream) const {
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

    vector<vector<uint64_t>> getAdjecencyMatrix() const {
        int n = getNumberOfNodes();
        vector<vector<uint64_t>> adjMatrix(n, vector<uint64_t>(n, 0));

        for (int i = 0; i < n; ++i) {
            for (int j : graph[i]) {
                adjMatrix[i][j] = 1;
            }
        }

        return adjMatrix;
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
            case PrintFormat::PromptAdjecencyList:
                printPromptAdjecencyListTo(outputStream);
                break;
            case PrintFormat::SolutionAdjecencyList:
                printSolutionAdjecencyListTo(outputStream);
                break;
            case PrintFormat::PromptAdjecencyMatrix:
                printPromptAdjecencyListTo(outputStream);
                break;
            case PrintFormat::SolutionAdjecencyMatrix:
                printSolutionAdjecencyListTo(outputStream);
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
