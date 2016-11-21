
#pragma once

#include <vector>
#include <array>

#include <InstanceCounter.h>


/*
 * @class Vertex
 */
class Vertex {
public:
    explicit Vertex(std::int32_t id);

    std::int32_t id() const;

private:
    std::int32_t m_id;
    InstanceCounter<Vertex> m_instanceCounter;
};


/*
 * @class Edge
 */
class Edge {
public:
    Edge(Vertex * v0, Vertex * v1, std::int32_t weight);

    void reset(Vertex * v0, Vertex * v1);

    std::int32_t weight() const;
    Vertex * v0() const;
    Vertex * v1() const;

private:
    std::int32_t m_weight;
    Vertex * m_v0;
    Vertex * m_v1;
    InstanceCounter<Edge> m_instanceCounter;
};


/*
 * @class Graph
 */
class Graph {
public:
    void addVertex(Vertex * vertex);
    void addEdge(Edge * edge);

    const std::vector<Vertex *> & vertices() const;
    const std::vector<Edge *> & edges() const;

    /*
     * @postcondition g2 is empty, i.e., does not contain any vertices or edges
     */
    void merge(Graph * g2);
    Graph * minimumSpanningTree() const;

    friend std::ostream& operator<<(std::ostream& stream, const Graph & graph);

private:
    std::vector<Vertex *> m_vertices;
    std::vector<Edge *> m_edges;
    InstanceCounter<Graph> m_instanceCounter;
};
