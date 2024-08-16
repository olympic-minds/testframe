#include "graph.hpp"
#include "rand.hpp"
#include <queue>
#include <set>
#include <utility>
#include <cmath>

Graph& Graph::relabelNodes() {
    auto perm = rnd.perm(getNumberOfNodes());
    std::vector<std::uint64_t> inv_perm(getNumberOfNodes());
    for (std::uint64_t i = 0; i < getNumberOfNodes(); i++) {
        inv_perm[perm[i]] = i;
    }
    auto graph_copy = graph;
    for (std::uint64_t v = 0; v < getNumberOfNodes(); ++v) {
        graph[v] = graph_copy[inv_perm[v]];
        for (auto& neigh : graph[v]) {
            neigh = perm[neigh];
        }
    }
    return *this;
}

std::uint64_t Graph::undirectedConnectedComponentsNumber() {
    std::uint64_t scc_number = 0;
    std::vector<bool> visited(getNumberOfNodes(), false);

    std::function<void(std::uint64_t)> dfs = [&](std::uint64_t v) -> void {
        visited[v] = true;
        for (auto u : graph[v]) {
            if (!visited[v]) {
                dfs(u);
            }
        }
    };

    for (std::uint64_t v = 0; v < getNumberOfNodes(); v++) {
        if (!visited[v]) {
            visited[v] = true;
            scc_number++;
            dfs(v);
        }
    }
    return scc_number;
}

Graph Graph::constructEmptyGraph(std::uint64_t nodes) {
    std::vector<std::vector<std::uint64_t>> g;
    g.resize(nodes);
    return Graph(g);
}

Graph Graph::constructUndirectedClique(std::uint64_t nodes) {
    std::vector<std::vector<std::uint64_t>> g;
    g.resize(nodes);
    for (std::uint64_t i = 0; i < nodes; ++i) {
        for (std::uint64_t j = 0; j < nodes; ++j) {
            if (i == j) {
                continue;
            }

            g[i].push_back(j);
        }
    }

    return Graph(g).relabelNodes();
}

Graph Graph::constructPathGraph(std::uint64_t nodes, std::uint64_t numberOfComponents) {
    std::vector<std::vector<std::uint64_t>> g(nodes);
    std::vector part = rnd.partition(numberOfComponents, nodes);
    std::uint64_t current = 0;
    for (std::uint64_t l = 0; l < part.size(); ++l) {
        std::uint64_t length = part[l];
        for (std::uint64_t i = 0; i < length - 1; ++i) {
            g[current].push_back(current + 1);
            g[current + 1].push_back(current);
            ++current;
        }
        ++current;
    }
    return Graph(g).relabelNodes();
}

Graph Graph::constructShallowForestGraph(std::uint64_t nodes, std::uint64_t numberOfTrees) {
    std::vector<std::vector<std::uint64_t>> g;
    g.resize(nodes);
    std::vector pa = rnd.partition(numberOfTrees, nodes);
    std::uint64_t root = 0, sum = 0, pnt = 0;
    for (std::uint64_t i = 0; i < nodes; i++) {
        if (i == sum) {
            root = sum;
            sum += pa[pnt++];
            continue;
        }
        std::uint64_t neighbor = rnd.intFromRange(root, i-1);
        g[neighbor].push_back(i);
        g[i].push_back(neighbor);
    }
    return Graph(g).relabelNodes();
}

Graph Graph::constructShallowTreeGraph(std::uint64_t nodes) {
    return constructShallowForestGraph(nodes, 1);
}

Graph Graph::constructForestGraph(std::uint64_t nodes, std::uint64_t numberOfTrees) {
    std::vector<std::vector<std::uint64_t>> g;
    g.resize(nodes);
    std::vector pa = rnd.partition(numberOfTrees, nodes);
    std::uint64_t root = 0;
    for (auto currentNodes : pa) {
        std::vector<std::uint64_t> prufer, cnt(currentNodes, 0);
        std::vector<std::vector<std::uint64_t>> g_curr(currentNodes);
        if (currentNodes == 1) {
            root++;
            continue;
        }
        if (currentNodes == 2) {
            g[root].push_back(root + 1);
            g[root + 1].push_back(root);
            root += 2;
            continue;
        }
        for (Random::IntType i = 0; i < currentNodes - 2; i++) {
            std::uint64_t x = rnd.intFromRange(currentNodes - 1);
            prufer.push_back(x);
            ++cnt[x];
        }
        std::priority_queue<std::uint64_t> q;
        for (Random::IntType i = 0; i < currentNodes; ++i) {
            if (!cnt[i]) {
                q.push(i);
            }
        }
        for (auto v : prufer) {
            std::uint64_t u = q.top();
            g_curr[u].push_back(v);
            g_curr[v].push_back(u);
            q.pop();
            if (--cnt[v] == 0) {
                q.push(v);
            }
        }
        std::uint64_t x = q.top();
        q.pop();
        std::uint64_t y = q.top();
        g_curr[x].push_back(y);
        g_curr[y].push_back(x);

        std::queue<std::uint64_t> bfs;

        bfs.push(0);
        std::uint64_t _id = root;
        while (!bfs.empty()) {
            std::uint64_t u = bfs.front();
            cnt[u] = 1;
            bfs.pop();
            for (auto v : g_curr[u]) {
                if (cnt[v] == 0) {
                    root++;
                    g[_id].push_back(root);
                    g[root].push_back(_id);
                    bfs.push(v);
                }
            }
            ++_id;
        }
    }
    return Graph(g).relabelNodes();
}

Graph Graph::constructTreeGraph(std::uint64_t nodes) {
    return constructForestGraph(nodes, 1);
}

Graph Graph::constructSimplerJellyfishGraph(std::uint64_t nodes, std::uint64_t cycleSize, std::uint64_t minTentacleLength,
                                            std::uint64_t numberOfTentacles) {
    std::vector<std::vector<std::uint64_t>> g;
    g.resize(nodes);
    std::vector pa = rnd.partition(numberOfTentacles, nodes - cycleSize, minTentacleLength);
    std::uint64_t next = 1;
    std::uint64_t prev = 0;
    for (std::uint64_t i = 0; i < cycleSize - 1; i++) {
        g[prev].push_back(next);
        g[next].push_back(prev);
        prev = next;
        next++;
    }
    g[prev].push_back(0);
    g[0].push_back(prev);
    for (std::uint64_t raySize : pa) {
        prev = rnd.intFromRange(cycleSize - 1);  // Ray starts at node in [0, cycyleSize - 1]
        for (std::uint64_t i = 0; i < raySize; i++) {
            g[prev].push_back(next);
            g[next].push_back(prev);
            prev = next;
            next++;
        }
    }
    return Graph(g).relabelNodes();
}

Graph Graph::constructStarfishGraph(std::uint64_t nodes, std::uint64_t minRayLength, std::uint64_t numberOfRays) {
    return constructSimplerJellyfishGraph(nodes, 1, minRayLength, numberOfRays);
}

/* Silkworm of size n is a path of size (n+1)/2 and one private node for each node from path */
Graph Graph::constructSilkwormGraph(std::uint64_t nodes) {
    std::vector<std::vector<std::uint64_t>> g;
    g.resize(nodes);
    for (std::uint64_t i = 0; i < nodes; i += 2) {
        if (i + 1 < nodes) {
            g[i].push_back(i + 1);
            g[i + 1].push_back(i);
        }
        if (i + 2 < nodes) {
            g[i].push_back(i + 2);
            g[i + 2].push_back(i);
        }
    }

    return Graph(g).relabelNodes();
}

Graph Graph::constructTreeOfBoundedDegreeGraph(std::uint64_t nodes, std::uint64_t minDegree, std::uint64_t maxDegree) {
    std::vector<std::vector<std::uint64_t>> g(nodes);
    std::vector<std::uint64_t> vec(nodes);
    std::iota(begin(vec), end(vec), 0);
    std::deque<std::uint64_t> availableLeaves(std::begin(vec), std::end(vec));
    std::queue<std::uint64_t> inTree;
    availableLeaves.pop_front();
    inTree.push(0);  // move 0 from availableLeaves to inTree
    while (!availableLeaves.empty() && !inTree.empty()) {
        std::uint64_t currentNode = inTree.front();
        inTree.pop();
        std::uint64_t degree = rnd.intFromRange(std::min(minDegree, (std::uint64_t)availableLeaves.size()),
                                   std::min(maxDegree, (std::uint64_t)availableLeaves.size()));
        while (degree--) {
            std::uint64_t nextNode = availableLeaves.front();
            availableLeaves.pop_front();
            inTree.push(nextNode);
            g[currentNode].push_back(nextNode);
            g[nextNode].push_back(currentNode);
        }
    }
    return Graph(g).relabelNodes();
}

Graph Graph::constructSparseGraph(std::uint64_t nodes) {
    std::uint64_t number_of_edges = rnd.intFromRange(nodes * static_cast<std::uint64_t>(std::sqrt(nodes)) / 2);
    std::set<std::pair<int, int>> edges;
    while ((std::uint64_t)edges.size() < number_of_edges) {
        auto vert = rnd.distinct(2, nodes - 1);
        std::uint64_t a = vert[0], b = vert[1];
        if (a > b) {
            std::swap(a, b);
        }
        edges.insert({a, b});
    }
    std::vector<std::vector<std::uint64_t>> g(nodes);
    for (auto [a, b] : edges) {
        g[a].push_back(b);
        g[b].push_back(a);
    }
    return Graph(g).relabelNodes();
}

Graph Graph::constructDenseGraph(std::uint64_t nodes) {
    std::vector<std::pair<int, int>> all_edges;
    all_edges.reserve(nodes * (nodes - 1) / 2);
    for (std::uint64_t i = 0; i < nodes; i++) {
        for (std::uint64_t j = i + 1; j < nodes; j++) {
            all_edges.emplace_back(i, j);
        }
    }

    rnd.shuffle(all_edges);
    std::uint64_t number_of_edges = rnd.intFromRange(
        nodes * static_cast<std::uint64_t>(std::sqrt(nodes)) / 2,
        nodes * (nodes - 1) / 2
    );
    all_edges.resize(number_of_edges);
    std::vector<std::vector<std::uint64_t>> g(nodes);
    for (auto [a, b] : all_edges) {
        g[a].push_back(b);
        g[b].push_back(a);
    }
    return Graph(g).relabelNodes();
}

Graph Graph::constructRandomDAG(std::uint64_t nodes, std::uint64_t edges, std::uint64_t height) {
    std::vector<std::vector<std::uint64_t>> g(nodes);
    std::vector pa = rnd.partition(height, nodes, 1);
    std::vector<std::vector<std::uint64_t>> layers(pa.size());
    uint64_t number_of_layers = pa.size();
    for (uint64_t i = 1; i < pa.size(); i++) {
        pa[i] += pa[i - 1];
    } 
    int j = 0;
    for (uint64_t i = 0; i < number_of_layers; i++) {
        while (j < pa[i]) {
            layers[i].push_back(j);
            j++;
        }
    } 
    for (uint64_t i = 0; i < edges; i ++) { // high_layer -> edge -> low_layer
        uint64_t from_layer = rnd.intFromRange(1, number_of_layers - 1);
        uint64_t to_layer = rnd.intFromRange(0, from_layer - 1);
        uint64_t from_node = layers[from_layer][rnd.intFromRange(0, layers[from_layer].size() - 1)];
        uint64_t to_node = layers[to_layer][rnd.intFromRange(0, layers[to_layer].size() - 1)];
        g[from_node].push_back(to_node);
    }
    return Graph(g, true).relabelNodes();
}

Graph Graph::constructDirectedGraph(Graph graph) {
    std::vector<std::vector<std::uint64_t>> g(graph.getNumberOfNodes());
    assert(!graph.directed);
    for (auto [u, v] : graph.getEdges()) {
        if (u <= v) {
            uint64_t r = rnd.intFromRange(2);
            if(r != 0) {
                g[u].push_back(v);
            }
            if(r != 2) {
                g[v].push_back(u);
            }
        }
    }
    return Graph(g, true).relabelNodes();
}

// Ścieżka // Zbiór ścieżek
// Drzewo // Las
// Losowe gęste // Losowe rzadkie
// Gwiazda // Rozgwiazda
// Silkworm
// Meduza
// Drzewa n-arne
// Graf r-regularny
// Dwudzielne
// Bi-clique
// Krata (n x m)
// Grid (n x m)

// DAG