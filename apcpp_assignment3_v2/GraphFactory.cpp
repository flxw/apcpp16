
#include <GraphFactory.h>

#include <random>
#include <set>
#include <array>

#include <Graph.h>
#include <map>
#include <list>


Graph * GraphFactory::createLinearGraph(std::int32_t numVertices, std::int32_t idOffset)
{
    auto graph = new Graph{};
    for (auto i = std::int32_t{0}; i < numVertices; ++i)
    {
        graph->addVertex(new Vertex{i + idOffset});
    }
    for (auto i = std::int32_t{1}; i < numVertices; ++i)
    {
        graph->addEdge(new Edge{graph->vertices()[i - 1], graph->vertices()[i], 1});
    }
    return graph;
}


Graph * GraphFactory::createCircularGraph(std::int32_t numVertices, std::int32_t idOffset)
{
    auto graph = createLinearGraph(numVertices, idOffset);
    graph->addEdge(new Edge{graph->vertices().back(), graph->vertices().front(), 1});
    return graph;
}


Graph * GraphFactory::createTree(std::int32_t numChildren, std::int32_t idOffset)
{
    auto graph = new Graph{};
    graph->addVertex(new Vertex{idOffset});
    for (auto i = std::int32_t{0}; i < numChildren; ++i)
    {
        graph->addVertex(new Vertex{idOffset + i + 1});
        graph->addEdge(new Edge{graph->vertices().front(), graph->vertices().back(), 1});
    }
    return graph;
}


Graph * GraphFactory::createRandomGraph(std::int32_t numVertices, std::int32_t idOffset)
{
    // Edge id for the purpose of creating a map/set
    using EdgeId = std::pair<std::int32_t, std::int32_t>;
    auto makeId = [](Edge * edge) { return std::make_pair(edge->v0()->id(), edge->v1()->id()); };

    auto graph = new Graph{};
    auto prng = std::mt19937{42}; // fixed seed

    // generate vertices
    auto vertexIds = std::set<std::int32_t>{};
    for (auto i = std::int32_t{0}; i < numVertices; ++i)
    {
        auto id = std::int32_t{0};
        do
        {
            id = prng() % std::numeric_limits<std::int32_t>::max();
        } while (vertexIds.find(id) != vertexIds.end());

        vertexIds.insert(id);
        graph->addVertex(new Vertex{id});
    }

    // generate edges
    auto vertexIdList = std::vector<std::int32_t>{vertexIds.begin(), vertexIds.end()};
    auto edgeSet = std::set<EdgeId>{};
    for (const auto & v0 : graph->vertices())
    {
        for (auto i = std::size_t{0}; i < 3; ++i)
        {
            const auto & v1 = graph->vertices()[prng() % vertexIdList.size()];
            if (v0 != v1)
            {
                auto edge = new Edge{ v0, v1, 1 };
                if (edgeSet.find(makeId(edge)) == edgeSet.end())
                {
                    edgeSet.insert(makeId(edge));
                    graph->addEdge(std::move(edge));
                }
            }
        }
    }

    return graph;
}


Graph * GraphFactory::createCityDistanceGraph()
{
    static const auto N = std::int32_t{20};
    static const auto weightsTable = std::array<int, N * N>{
        -1, 255, 504, 477, 424, 512, 477, 422, 453, 315, 165, 149, 249, 378, 471, 439, 452, 336, 479, 398,
        255, -1, 612, 356, 393, 534, 338, 284, 308, 95, 377, 294, 132, 462, 321, 297, 320, 196, 370, 237,
        504, 612, -1, 456, 304, 190, 486, 477, 493, 582, 359, 360, 488, 150, 503, 486, 470, 483, 433, 510,
        477, 356, 456, -1, 152, 288, 34, 72, 57, 269, 474, 380, 249, 336, 56, 62, 37, 162, 25, 123,
        424, 393, 304, 152, -1, 152, 182, 177, 190, 329, 371, 293, 261, 187, 200, 184, 167, 212, 131, 218,
        512, 534, 190, 288, 152, -1, 322, 328, 335, 478, 412, 364, 401, 157, 342, 332, 311, 364, 264, 371,
        477, 338, 486, 34, 182, 322, -1, 57, 30, 248, 485, 389, 240, 363, 23, 41, 26, 150, 59, 101,
        422, 284, 477, 72, 177, 328, 57, -1, 32, 196, 439, 340, 182, 343, 49, 17, 36, 93, 90, 51,
        453, 308, 493, 57, 190, 335, 30, 32, -1, 217, 470, 371, 212, 364, 18, 14, 24, 122, 81, 70,
        315, 95, 582, 269, 329, 478, 248, 196, 217, -1, 405, 310, 100, 433, 229, 207, 231, 118, 285, 150,
        165, 377, 359, 474, 371, 412, 485, 439, 470, 405, -1, 100, 312, 259, 487, 456, 460, 375, 467, 434,
        149, 294, 360, 380, 293, 364, 389, 340, 371, 310, 100, -1, 214, 229, 389, 358, 363, 275, 375, 334,
        249, 132, 488, 249, 261, 401, 240, 182, 212, 100, 312, 214, -1, 338, 229, 199, 217, 90, 258, 151,
        378, 462, 150, 336, 187, 157, 363, 343, 364, 433, 259, 229, 338, -1, 377, 355, 343, 337, 318, 370,
        471, 321, 503, 56, 200, 342, 23, 49, 18, 229, 487, 389, 229, 377, -1, 32, 33, 138, 81, 84,
        439, 297, 486, 62, 184, 332, 41, 17, 14, 207, 456, 358, 199, 355, 32, -1, 26, 109, 84, 61,
        452, 320, 470, 37, 167, 311, 26, 36, 24, 231, 460, 363, 217, 343, 33, 26, -1, 128, 58, 85,
        336, 196, 483, 162, 212, 364, 150, 93, 122, 118, 375, 275, 90, 337, 138, 109, 128, -1, 175, 62,
        479, 370, 433, 25, 131, 264, 59, 90, 81, 285, 467, 375, 258, 318, 81, 84, 58, 175, -1, 142,
        398, 237, 510, 123, 218, 371, 101, 51, 70, 150, 434, 334, 151, 370, 84, 61, 85, 62, 142, -1
    };

    auto graph = new Graph{};

    //
    // TODO: implement graph generation from weight matrix
    //

    return graph;
}
