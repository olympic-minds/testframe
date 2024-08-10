#include "graph.hpp"
#include "weighted_graph.hpp"
#include "rand.hpp"
#include <queue>
#include <set>
#include <utility>
#include <cmath>

WeightedGraph& WeightedGraph::relabelNodes() {
    auto perm = rnd.perm(getNumberOfNodes());
    std::vector<std::uint64_t> inv_perm(getNumberOfNodes());
    for (std::uint64_t i = 0; i < getNumberOfNodes(); i++) {
        inv_perm[perm[i]] = i;
    }
    auto graph_copy = graph;
    for (std::uint64_t v = 0; v < getNumberOfNodes(); ++v) {
        graph[v] = graph_copy[inv_perm[v]];
        for (auto& neigh : graph[v]) {
            neigh.first = perm[neigh.first];
        }
    }
    return *this;
}

WeightedGraph WeightedGraph::addRandomWeights(Graph g, std::int64_t w_min, std::int64_t w_max) {
    std::vector<std::vector<std::pair<std::uint64_t, std::int64_t>>> graph(g.getNumberOfNodes());
    for (auto [u, v] : g.getEdges()) {
        std::int64_t w = rnd.intFromRange(w_min, w_max);
        if (!g.directed && u <= v) {
            graph[u].push_back({v, w});
            graph[v].push_back({u, w});
        }
        if (g.directed) {
            graph[u].push_back({v, w});
        }
    }
    return WeightedGraph(graph).relabelNodes();
}