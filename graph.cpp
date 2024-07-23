#include "graph.hpp"
#include "testlib.hpp"
#include <queue>

Graph& Graph::relabelNodes() {
    auto perm = rnd.perm(getNumberOfNodes() );
    std::vector<uint64_t>inv_perm(getNumberOfNodes() );
    for (uint64_t i = 0; i < getNumberOfNodes() ; i++) {
        inv_perm[perm[i]] = i;
    }
    auto graph_copy = graph;
    for (uint64_t v = 0; v < getNumberOfNodes() ; ++v) {
        graph[v] = graph_copy[inv_perm[v]];
        for (auto &neigh : graph[v]) {
            neigh = perm[neigh];
        }
    }
    return *this;
}

uint64_t Graph::undirectedConnectedComponentsNumber() {
    uint64_t scc_number = 0;
    bool visited[getNumberOfNodes() ];

    std::function<void(uint64_t)> dfs = [&](uint64_t v) -> void {
        visited[v] = true;
        for (auto u : graph[v]) {
            dfs(u);
        }
    };

    for (uint64_t v = 0; v < getNumberOfNodes() ; v++) {
        if (!visited[v]) {
            visited[v] = true;
            scc_number++;
            dfs(v);
        }
    }
    return scc_number;
}


Graph Graph::constructEmptyGraph(uint64_t nodes) {
    std::vector<std::vector<uint64_t>> g;
    g.resize(nodes);
    return Graph(g);
}

Graph Graph::constructUndirectedClique(uint64_t nodes) {
    std::vector<std::vector<uint64_t>> g;
    g.resize(nodes);
    for (uint64_t i = 0; i < nodes; ++i) {
        for (uint64_t j = 0; j < nodes; ++j) {
            if (i == j) {
                continue;
            }

            g[i].push_back(j);
        }
    }

    return Graph(g).relabelNodes();
}

Graph Graph::constructPathGraph(uint64_t nodes, uint64_t numberOfComponents) {
    std::vector<std::vector<uint64_t>> g(nodes);
    std::vector<uint64_t> part = rnd.partition(numberOfComponents, nodes);
    uint64_t current = 0;
    for (uint64_t l = 0; l < part.size(); ++l) {
        uint64_t length = part[l];
        for (uint64_t i = 0; i < length - 1; ++i) {
            g[current].push_back(current + 1);
            g[current + 1].push_back(current);
            ++current;
        }
        ++current;
    }
    return Graph(g).relabelNodes();
}

Graph Graph::constructShallowForestGraph(uint64_t nodes, uint64_t numberOfTrees) {
    std::vector<std::vector<uint64_t>> g;
    g.resize(nodes);
    std::vector<uint64_t>pa = rnd.partition(numberOfTrees, nodes);
    uint64_t root = 0, sum = 0, pnt = 0;
    for (uint64_t i = 0; i < nodes; i++) {
        if (i == sum) {
            root = sum;
            sum += pa[pnt++];
            continue;
        }
        uint64_t neighbor = rnd.next(root, i - 1);
        g[neighbor].push_back(i);
        g[i].push_back(neighbor);
    }
    return Graph(g).relabelNodes();
}

Graph Graph::constructShallowTreeGraph(uint64_t nodes) { return constructShallowForestGraph(nodes, 1); }

Graph Graph::constructForestGraph(uint64_t nodes, uint64_t numberOfTrees) {
    std::vector<std::vector<uint64_t>> g;
    g.resize(nodes);
    std::vector<uint64_t>pa = rnd.partition(numberOfTrees, nodes);
    uint64_t root = 0;
    for (auto currentNodes : pa) {
        std::vector<uint64_t>prufer, cnt(currentNodes, 0);
        std::vector<std::vector<uint64_t>> g_curr(currentNodes);
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
        for (uint64_t i = 0; i < currentNodes - 2; i++) {
            uint64_t x = rnd.next(currentNodes);
            prufer.push_back(x);
            ++cnt[x];
        }
        std::priority_queue<uint64_t>q;
        for (uint64_t i = 0; i < currentNodes; ++i) {
            if (!cnt[i]) {
                q.push(i);
            }
        }
        for (auto v : prufer) {
            uint64_t u = q.top();
            g_curr[u].push_back(v);
            g_curr[v].push_back(u);
            q.pop();
            if (--cnt[v] == 0) {
                q.push(v);
            }
        }
        uint64_t x = q.top();
        q.pop();
        uint64_t y = q.top();
        g_curr[x].push_back(y);
        g_curr[y].push_back(x);

        std::queue<uint64_t>bfs;

        bfs.push(0);
        uint64_t _id = root;
        while (!bfs.empty()) {
            uint64_t u = bfs.front();
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

Graph Graph::constructTreeGraph(uint64_t nodes) { return constructForestGraph(nodes, 1); }

Graph Graph::constructSimplerJellyfishGraph(uint64_t nodes, uint64_t cycleSize, uint64_t maxTentacleLength, uint64_t numberOfTentacles) {
    std::vector<std::vector<uint64_t>> g;
    g.resize(nodes);
    std::vector<uint64_t>pa = rnd.partition(nodes - cycleSize, numberOfTentacles, maxTentacleLength);
    uint64_t next = 1;
    uint64_t prev = 0;
    for (uint64_t i = 0; i < cycleSize - 1; i++) {
        g[prev].push_back(next);
        g[next].push_back(prev);
        prev = next;
        next++;
    }
    g[prev].push_back(0);
    g[0].push_back(prev);
    for (uint64_t raySize : pa) {
        prev = rnd.next(cycleSize);  // Ray starts at node in [0, cycyleSize - 1]
        for (uint64_t i = 0; i < raySize; i++) {
            g[prev].push_back(next);
            g[next].push_back(prev);
            prev = next;
            next++;
        }
    }
    return Graph(g).relabelNodes();
}

Graph Graph::constructStarfishGraph(uint64_t nodes, uint64_t maxRayLength, uint64_t numberOfRays) {
    return constructSimplerJellyfishGraph(nodes, 1, maxRayLength, numberOfRays);
}

/* Silkworm of size n is a path of size (n+1)/2 and one private node for each node from path */
Graph Graph::constructSilkwormGraph(uint64_t nodes) {
    std::vector<std::vector<uint64_t>> g;
    g.resize(nodes);
    for (uint64_t i = 0; i < nodes; i += 2) {
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

Graph Graph::constructTreeOfBoundedDegreeGraph(uint64_t nodes, uint64_t minDegree, uint64_t maxDegree) {
    std::vector<std::vector<uint64_t>> g(nodes);
    std::vector<uint64_t>vec(nodes);
    std::iota(begin(vec), end(vec), 0);
    std::deque<uint64_t>availableLeaves(std::begin(vec), std::end(vec));
    std::queue<uint64_t>inTree;
    availableLeaves.pop_front();
    inTree.push(0);  // move 0 from availableLeaves to inTree
    while (!availableLeaves.empty() && !inTree.empty()) {
        uint64_t currentNode = inTree.front();
        inTree.pop();
        uint64_t degree = rnd.next(std::min(minDegree, (uint64_t)availableLeaves.size()),
                              std::min(maxDegree, (uint64_t)availableLeaves.size()));
        while (degree--) {
            uint64_t nextNode = availableLeaves.front();
            availableLeaves.pop_front();
            inTree.push(nextNode);
            g[currentNode].push_back(nextNode);
        }
    }
    return Graph(g).relabelNodes();
}

Graph Graph::constructSparseGraph(uint64_t nodes) {
    uint64_t number_of_edges = rnd.wnext((uint64_t)0, nodes * (uint64_t)sqrt(nodes) / 2, -1);
    std::set<std::pair<int, int>> edges;
    while ((int)edges.size() < number_of_edges) {
        auto vert = rnd.distinct(2, nodes);
        uint64_t a = vert[0], b = vert[1];
        if (a > b) {
            std::swap(a, b);
        }
        edges.insert({a, b});
    }
    std::vector<std::vector<uint64_t>> g(nodes);
    for (auto [a, b] : edges) {
        g[a].push_back(b);
        g[b].push_back(a);
    }
    return Graph(g).relabelNodes();
}

Graph Graph::constructDenseGraph(uint64_t nodes) {
    std::vector<std::pair<int, int>> all_edges;
    all_edges.reserve(nodes * (nodes - 1) / 2);
    for (uint64_t i = 0; i < nodes; i++) {
        for (uint64_t j = i + 1; j < nodes; j++) {
            all_edges.emplace_back(i, j);
        }
    }
    shuffle(all_edges.begin(), all_edges.end());
    uint64_t number_of_edges = rnd.wnext(nodes * (int)sqrt(nodes) / 2, nodes * (nodes - 1) / 2, 1);
    all_edges.resize(number_of_edges);
    std::vector<std::vector<uint64_t>> g(nodes);
    for (auto [a, b] : all_edges) {
        g[a].push_back(b);
        g[b].push_back(a);
    }
    return Graph(g).relabelNodes();
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
