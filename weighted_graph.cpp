#include "graph.hpp"
#include "weighted_graph.hpp"
#include "rand.hpp"
#include <queue>
#include <set>
#include <utility>
#include <cmath>

WeightedGraph WeightedGraph::addRandomWeights(Graph g, std::int64_t w_min, std::int64_t w_max) {
    std::vector<std::vector<std::pair<std::uint64_t, std::int64_t>>> graph(g.getNumberOfNodes());
    for (auto [u, v] : g.getEdges()) {
        std::int64_t w = rnd.intFromRange(w_min, w_max);
        graph[u].push_back({v, w});
        graph[v].push_back({u, w});
    }
    return WeightedGraph(graph);
}