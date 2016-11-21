
#include <iostream>
#include <numeric>

#include <Graph.h>
#include <GraphFactory.h>


void test()
{
    auto g1 = GraphFactory::createLinearGraph(5, 0);
    std::cout << *g1 << std::endl;

    auto g2 = GraphFactory::createLinearGraph(5, 4);
    std::cout << *g2 << std::endl;

    g1->merge(g2);
    std::cout << *g1 << std::endl;

    auto g3 = GraphFactory::createTree(10, 9);
    std::cout << *g3 << std::endl;

    g1->merge(g3);
    std::cout << *g1 << std::endl;

    auto g4 = GraphFactory::createRandomGraph(123);
    auto g5 = GraphFactory::createRandomGraph(42);
    g4->merge(g5);

    auto g6 = GraphFactory::createCityDistanceGraph();
    auto g7 = g6->minimumSpanningTree();

    const auto totalWeight = std::accumulate(g7->edges().cbegin(), g7->edges().cend(), 0, [](const auto & sum, const auto & edge) { return sum + edge->weight();  });
    std::cout << "Shortest route: " << totalWeight << std::endl;
}


int main(int argc, char * argv[])
{
    test();
    printMemoryUsage();
    test();
    printMemoryUsage();
}
