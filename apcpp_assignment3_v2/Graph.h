
#pragma once

#include <vector>
#include <array>
#include <memory>

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
    Edge(std::shared_ptr<Vertex> v0, std::shared_ptr<Vertex> v1, std::int32_t weight);

    void reset(std::shared_ptr<Vertex> v0, std::shared_ptr<Vertex> v1);

    std::int32_t weight() const;
    std::shared_ptr<Vertex> v0() const;
    std::shared_ptr<Vertex> v1() const;

private:
    std::int32_t m_weight;
    std::shared_ptr<Vertex> m_v0;
    std::shared_ptr<Vertex> m_v1;
    InstanceCounter<Edge> m_instanceCounter;
};


/*
 * @class Graph
 */
class Graph {
public:
    void addVertex(std::shared_ptr<Vertex> vertex);
    void addEdge(std::shared_ptr<Edge> edge);

    const std::vector<std::shared_ptr<Vertex> > & vertices() const;
    const std::vector<std::shared_ptr<Edge> > & edges() const;

    /*
     * @postcondition g2 is empty, i.e., does not contain any vertices or edges
     */
    void merge(std::shared_ptr<Graph> g2);
    std::shared_ptr<Graph> minimumSpanningTree() const;

    friend std::ostream& operator<<(std::ostream& stream, const Graph & graph);

private:
    std::vector<std::shared_ptr<Vertex> > m_vertices;
    std::vector<std::shared_ptr<Edge> > m_edges;
    InstanceCounter<Graph> m_instanceCounter;
};
