#ifndef GRAPH_H_
#define GRAPH_H_

#include "utils.hpp"
#include <numeric>
#include <functional>

class Graph {
public:
    std::vector<std::vector<std::uint64_t>> graph;
    enum class PrintFormat {
        PromptAdjecencyList,
        SolutionAdjecencyList,
        PromptAdjecencyMatrix,
        SolutionAdjecencyMatrix
    };
private:
    void printPromptAdjecencyListTo(std::ostream &outputStream) const {
        outputStream << "{";
        for (std::uint64_t i = 0; i < getNumberOfNodes(); ++i) {
            outputStream << "{";
            for (std::uint64_t j = 0; j < graph[i].size(); ++j) {
                outputStream << graph[i][j];
                if (j != graph[i].size() - 1) {
                    outputStream << ",";
                }
            }
            outputStream << "}";
            if (i != getNumberOfNodes() - 1) {
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
        for (std::uint64_t i = 0; i < nodes; ++i) {
            outputStream << "{";
            for (std::uint64_t j = 0; j < nodes; ++j) {
                outputStream << matrix[i][j];
                if (j != nodes - 1) {
                    outputStream << ",";
                }
            }
            outputStream << "}";
            if (i != nodes - 1) {
                outputStream << ",";
            }
        }
        outputStream << "}\n";
    }

    void printSolutionAdjecencyMatrixTo(std::ostream &outputStream) const {
        std::uint64_t nodes = getNumberOfNodes();
        outputStream << nodes  << "\n";
        
        std::vector<std::vector<int>> matrix(nodes, std::vector<int>(nodes, 0));
        for (auto [from, to] : getEdges()) {
            matrix[from][to] = 1;
        }

        for (std::uint64_t i = 0; i < nodes; ++i) {
            for (std::uint64_t j = 0; j < nodes; ++j) {
                outputStream << matrix[i][j];
                if (j != nodes - 1) {
                    outputStream << " ";
                }
            }
            outputStream << "\n";
        }
    }

    void printSolutionAdjecencyListTo(std::ostream &outputStream) const {
        auto edges = getEdges();

        outputStream << getNumberOfNodes() << " " << edges.size() << "\n";
        for (auto edge : edges) {
            outputStream << edge.first << " " << edge.second << "\n";
        }
    }
public:
    Graph(std::vector<std::vector<std::uint64_t>> g, bool directed = false)
        : graph(g) {}

    std::uint64_t getNumberOfNodes() const {
        return graph.size();
    }

    std::uint64_t getNumberOfEdges() const {
        return std::accumulate(graph.begin(), graph.end(), 0,
                               [](const std::uint64_t x, const auto &a) { return x + a.size(); });
    }

    std::vector<std::vector<std::uint64_t>> getAdjecencyMatrix() const {
        std::uint64_t n = getNumberOfNodes();
        std::vector<std::vector<std::uint64_t>> adjMatrix(n, std::vector<std::uint64_t>(n, 0));

        for (std::uint64_t i = 0; i < n; ++i) {
            for (std::uint64_t j : graph[i]) {
                adjMatrix[i][j] = 1;
            }
        }

        return adjMatrix;
    }

    bool operator==(const Graph &other) const {
        if (getNumberOfNodes() != other.getNumberOfNodes()) {
            return false;
        }

        for (std::uint64_t node = 0; node < getNumberOfNodes(); ++node) {
            if (graph[node] != other.graph[node]) {
                return false;
            }
        }
        return true;
    }

    operator std::vector<std::vector<std::uint64_t>>() const {
        return graph;
    }

    std::vector<std::pair<std::uint64_t, std::uint64_t>> getEdges() const {
        std::vector<std::pair<std::uint64_t, std::uint64_t>> edges;
        for (std::uint64_t v = 0; v < getNumberOfNodes() ; ++v)
            for (std::uint64_t u : graph[v])
                edges.emplace_back(v, u);
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
        std::uint64_t nodes, numberOfEdges;
        inputStream >> nodes >> numberOfEdges;
        std::vector<std::vector<std::uint64_t>> g(nodes);
        for (std::uint64_t i = 0; i < numberOfEdges; i++) {
            std::uint64_t a, b;
            inputStream >> a >> b;
            g[a].push_back(b);
        }
        return Graph(g);
    }

    static Graph constructEmptyGraph(std::uint64_t nodes);
    static Graph constructUndirectedClique(std::uint64_t nodes);
    static Graph constructPathGraph(std::uint64_t nodes, std::uint64_t numberOfComponents = 1);
    static Graph constructShallowForestGraph(std::uint64_t nodes, std::uint64_t numberOfTrees);
    static Graph constructShallowTreeGraph(std::uint64_t nodes);
    static Graph constructForestGraph(std::uint64_t nodes, std::uint64_t numberOfTrees);
    static Graph constructTreeGraph(std::uint64_t nodes);
    static Graph constructSimplerJellyfishGraph(std::uint64_t nodes, std::uint64_t cycleSize, std::uint64_t minTentacleLength,
                                                   std::uint64_t numberOfTentacles);
    static Graph constructStarfishGraph(std::uint64_t nodes, std::uint64_t minRayLength, std::uint64_t numberOfRays);
    static Graph constructSilkwormGraph(std::uint64_t nodes);
    static Graph constructTreeOfBoundedDegreeGraph(std::uint64_t nodes, std::uint64_t minDegree, std::uint64_t maxDegree);
    static Graph constructSparseGraph(std::uint64_t nodes);
    static Graph constructDenseGraph(std::uint64_t nodes);

    bool isClique() {
        std::uint64_t numberOfEdges = getNumberOfEdges();
        return numberOfEdges  == getNumberOfNodes() * (getNumberOfNodes() - 1);
    }

    bool isConnected() {
        return undirectedConnectedComponentsNumber() == 1;
    }

    std::uint64_t undirectedConnectedComponentsNumber();
};

#endif
