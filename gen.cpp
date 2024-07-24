#include "testframe.hpp"
#include "graph.hpp"
#include "gen_utils.hpp"

using namespace std;

void printGraphToAppropriateFiles(int testNumber, const Graph& g);


void testEmptyGraph(int testNumber) {
    int numOfNodes = rnd.next(5, 10); 
    printGraphToAppropriateFiles(testNumber, Graph::constructEmptyGraph(numOfNodes));
}

void testShortPath(int testNumber) {
    int numOfNodes = rnd.next(20, 30); 
    printGraphToAppropriateFiles(testNumber, Graph::constructPathGraph(numOfNodes));
}

void testMultipleShortPaths(int testNumber) {
    int numOfNodes = rnd.next(30, 45);
    int numOfPaths = rnd.next(3, 4); 
    printGraphToAppropriateFiles(testNumber, Graph::constructPathGraph(numOfNodes, numOfPaths));
}

void testSmallClique(int testNumber) {
    int numOfNodes = rnd.next(5, 10);

    printGraphToAppropriateFiles(testNumber, Graph::constructUndirectedClique(numOfNodes));
}

void testForest(int testNumber) {
    int numOfNodes = rnd.next(30, 45);
    int numberOfTrees = rnd.next(3, 4);
    printGraphToAppropriateFiles(testNumber, Graph::constructForestGraph(numOfNodes, numberOfTrees));
}

void testSparseGraph(int testNumber) {
    int numOfNodes = rnd.next(10, 15);
    printGraphToAppropriateFiles(testNumber, Graph::constructSparseGraph(numOfNodes));
}


int main() {
    int seed;
    cin>>seed;
    // registerGen(seed);
    setupDirectories();

    std::map<int, std::function<void(int)>> tests = {
        {0, testEmptyGraph},
        {1, testShortPath},
        {2, testMultipleShortPaths},
        {3, testSmallClique},
        {4, testForest},
        {5, testSparseGraph}
    };

    for(auto [testId, test]: tests) {
        test(testId);
    }

}

void printGraphToAppropriateFiles(int testNumber, const Graph& g) {

    auto [promptInStream, solutionInStream] = setupTest(testNumber); 

    g.printTo(promptInStream, Prompt);
    g.printTo(solutionInStream, Solution);

    promptInStream.close();
    solutionInStream.close();
}
