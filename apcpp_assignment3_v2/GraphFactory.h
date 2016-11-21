
#pragma once

#include <cstdint>


class Graph;


/*
 * @class GraphFactory
 */
class GraphFactory {
public:
    static Graph * createLinearGraph(std::int32_t numVertices, std::int32_t idOffset = 0);
    static Graph * createCircularGraph(std::int32_t numVertices, std::int32_t idOffset = 0);
    static Graph * createTree(std::int32_t numChildren, std::int32_t idOffset = 0);
    static Graph * createRandomGraph(std::int32_t numVertices, std::int32_t idOffset = 0);
    static Graph * createCityDistanceGraph();
};