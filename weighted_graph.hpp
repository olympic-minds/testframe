#ifndef WEIGHTED_GRAPH_H_
#define WEIGHTED_GRAPH_H_

#include "utils.hpp"
#include "graph.hpp"
#include <numeric>
#include <functional>

class WeightedGraph {
public: 
    std::vector<std::vector<std::pair<std::uint64_t, std::int64_t>>> graph;
    enum class PrintFormat {
        PromptAdjecencyList,
        SolutionAdjecencyList,
        PromptAdjecencyMatrix,
        SolutionAdjecencyMatrix
    };
    class Edge {
        public:
            uint64_t start;
            uint64_t end;
            int64_t weight;
            Edge (uint64_t start, uint64_t end, uint64_t weight) : start(start), end(end), weight(weight) {}; 
    };
private:
    void printPromptAdjecencyListTo(std::ostream &outputStream) const {
        outputStream << "{";
        for (std::uint64_t i = 0; i < getNumberOfNodes(); ++i) {
            outputStream << "{";
            for (std::uint64_t j = 0; j < graph[i].size(); ++j) {
                outputStream << "{" << graph[i][j].first << ", " << graph[i][j].second << "}";
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

    void printSolutionAdjecencyListTo(std::ostream &outputStream) const {
        auto edges = getEdges();

        outputStream << getNumberOfNodes() << " " << edges.size() << "\n";
        for (auto edge : edges) {
            outputStream << edge.start << " " << edge.end << " " << edge.weight << "\n";
        }
    }

    void printSolutionAdjecencyMatrixTo(std::ostream &outputStream) const {
        std::uint64_t nodes = getNumberOfNodes();
        outputStream << nodes  << "\n";
        
        std::vector<std::vector<std::int64_t>> matrix(nodes, std::vector<std::int64_t>(nodes, 0));
        for (auto [from, to, weight] : getEdges()) {
            matrix[from][to] = weight;
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
    void printPromptAdjecencyMatrixTo(std::ostream &outputStream) const {
        auto nodes = getNumberOfNodes();

        std::vector<std::vector<std::int64_t>> matrix(nodes, std::vector<std::int64_t>(nodes, 0));

        for (auto [from, to, weight] : getEdges()) {
            matrix[from][to] = weight;
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
public:
    WeightedGraph(std::vector<std::vector<std::pair<std::uint64_t, std::int64_t>>> g) : graph(g) {}

    std::uint64_t getNumberOfNodes() const {
        return graph.size();
    }

    std::uint64_t getNumberOfEdges() const {
        return std::accumulate(graph.begin(), graph.end(), 0,
                               [](const std::uint64_t x, const auto &a) { return x + a.size(); });
    }

    std::vector<Edge> getEdges() const {
        std::vector<Edge> edges;
        for (std::uint64_t v = 0; v < getNumberOfNodes() ; ++v)
            for (auto u : graph[v]) {
                edges.emplace_back(Edge(v, u.first, u.second));
            }
        return edges;
    }

    void printTo(std::ostream &outputStream, PrintFormat format) const {
        switch (format) {
            case PrintFormat::PromptAdjecencyList:
                printPromptAdjecencyListTo(outputStream);
                break;
            case PrintFormat::SolutionAdjecencyList:
                printSolutionAdjecencyListTo(outputStream);
                break;
            case PrintFormat::SolutionAdjecencyMatrix:
                printSolutionAdjecencyMatrixTo(outputStream);
                break;
            case PrintFormat::PromptAdjecencyMatrix:
                printPromptAdjecencyMatrixTo(outputStream);
                break;
        }
    }

    static WeightedGraph readWeightedGraph(std::istream &inputStream) {
        std::uint64_t nodes, numberOfEdges;
        inputStream >> nodes >> numberOfEdges;
        std::vector<std::vector<std::pair<std::uint64_t, std::int64_t>>> g(nodes);
        for (std::uint64_t i = 0; i < numberOfEdges; i++) {
            std::uint64_t a, b;
            std::int64_t w;
            inputStream >> a >> b >> w;
            g[a].push_back({b, w});
        }
        return WeightedGraph(g);
    }

    WeightedGraph &relabelNodes();

    bool operator==(const WeightedGraph &other) const {
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

    operator std::vector<std::vector<std::pair<std::uint64_t, std::int64_t>>>() const {
        return graph;
    }

    static WeightedGraph addRandomWeights(Graph g, std::int64_t w_min, std::int64_t w_max);
};

#endif
