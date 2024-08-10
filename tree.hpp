#ifndef TREE_H_
#define TREE_H_

#include <iostream>
#include <memory>
#include <vector>
#include <functional>

#include "graph.hpp"
#include "rand.hpp"

template<typename T>
class Tree {
public:
    class TreeNode {
    public:
        T value;
        TreeNode* parent;
        std::vector<std::unique_ptr<TreeNode>> children;

        TreeNode(const T& val, TreeNode* par = nullptr) 
            : value(val), parent(par) {}

        TreeNode* addChild(const T& val) {
            children.push_back(std::make_unique<TreeNode>(val, this));
            return children.back().get();
        }
    };

    std::unique_ptr<TreeNode> root;

    Tree() : root(nullptr) {}

    void setRoot(const T& value) {
        root = std::make_unique<TreeNode>(value);
    }

    using ValueGenerator = std::function<T()>;

    static Tree convertGraphToTree(Graph& graph, typename Tree<T>::ValueGenerator generateValue) {
        std::uint64_t numNodes = graph.getNumberOfNodes();
        if (numNodes == 0) {
            return Tree<T>();
        }
        
        std::vector<std::vector<std::uint64_t>> edges = graph.graph;

        Tree<T> tree;
        tree.setRoot(generateValue());

        std::vector<bool> visited(numNodes, false);

        std::function<void(int, typename Tree<T>::TreeNode*)> buildTree = [&](int currentNode, typename Tree<T>::TreeNode* parent) {
            visited[currentNode] = true;

            for (int neighbor : edges[currentNode]) {
                if (!visited[neighbor]) {
                    typename Tree<T>::TreeNode* childNode = parent->addChild(generateValue());
                    buildTree(neighbor, childNode);
                }
            }
        };

        buildTree(0, tree.root.get());
        return tree;
    }
    static Tree constructShallowTree(std::uint64_t nodes, ValueGenerator generateValue) {
        Tree<T> tree;

        if (nodes == 0) {
            return tree;
        }

        tree.setRoot(generateValue());
        TreeNode* current = tree.root.get();

        for (std::uint64_t i = 1; i < nodes; ++i) {
            int decision = rnd.intFromRange(0, 99);

            if (decision < 70) {  // 70% chance to add a single child to the current node (bias towards depth)
                current = current->addChild(generateValue());
            } else if (decision < 90 && current->parent) {  // 20% chance to move back to the parent and add a sibling
                current = current->parent->addChild(generateValue());
            } else {  // 10% chance to add another child to the current node (increasing breadth slightly)
                current->addChild(generateValue());
            }
        }

        return tree;
    }

    static Tree constructSilkwormTree(std::uint64_t nodes, ValueGenerator generateValue) {
        auto g = Graph::constructSilkwormGraph(nodes);
        return convertGraphToTree(g, generateValue);
    }

    static Tree constructTreeOfBoundedDegree(std::uint64_t nodes, std::uint64_t minDegree, std::uint64_t maxDegree, ValueGenerator generateValue) {
        auto g = Graph::constructTreeOfBoundedDegreeGraph(nodes, minDegree, maxDegree);
        return convertGraphToTree(g, generateValue);
    }
    static Tree constructBinaryTree(std::uint64_t nodes, ValueGenerator generateValue) {
        auto g = Graph::constructTreeOfBoundedDegreeGraph(nodes, 1, 2);
        return convertGraphToTree(g, generateValue);
    }

    static Tree constructStarfishTree(std::uint64_t nodes, std::uint64_t minRayLength, std::uint64_t numberOfRays, ValueGenerator generateValue) {
        auto g = Graph::constructStarfishGraph(nodes, minRayLength, numberOfRays);
        return convertGraphToTree(g, generateValue);
    }
};

#endif // TREE_H_
