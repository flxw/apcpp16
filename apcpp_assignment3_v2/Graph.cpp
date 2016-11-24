
#include <Graph.h>

#include <map>
#include <set>
#include <iostream>



//
// Class Vertex
//


Vertex::Vertex(std::int32_t id)
: m_id{id}
{
}


std::int32_t Vertex::id() const
{
    return m_id;
}




//
// Class Edge
//


Edge::Edge(std::shared_ptr<Vertex> v0, std::shared_ptr<Vertex> v1, std::int32_t weight)
: m_weight{weight}
{
    reset(v0, v1);
}


void Edge::reset(std::shared_ptr<Vertex> v0, std::shared_ptr<Vertex> v1)
{
    if (v0 < v1)
    {
        m_v0 = v0;
        m_v1 = v1;
    }
    else
    {
        m_v0 = v1;
        m_v1 = v0;
    }
}


std::int32_t Edge::weight() const
{
    return m_weight;
}


std::shared_ptr<Vertex> Edge::v0() const
{
    return m_v0;
}


std::shared_ptr<Vertex> Edge::v1() const
{
    return m_v1;
}





//
// Class Graph
//


const std::vector<std::shared_ptr<Vertex> > & Graph::vertices() const
{
    return m_vertices;
}


const std::vector<std::shared_ptr<Edge> > & Graph::edges() const
{
    return m_edges;
}


void Graph::addVertex(std::shared_ptr<Vertex> vertex)
{
    m_vertices.emplace_back(vertex);
}


void Graph::addEdge(std::shared_ptr<Edge> edge)
{
    m_edges.emplace_back(edge);
}


void Graph::merge(std::shared_ptr<Graph> g2)
{
    // Edge id for the purpose of creating a map/set
    using EdgeId = std::pair<std::int32_t, std::int32_t>;
    auto makeId = [](std::shared_ptr<Edge> edge) { return std::make_pair(edge->v0()->id(), edge->v1()->id()); };

    // build map for fast lookup of vertices by id
    auto vertexMap = std::map<int, std::shared_ptr<Vertex> >{};
    for (const auto & vertex : m_vertices)
    {
        vertexMap[vertex->id()] = vertex;
    }

    // build map for fast lookup of edges by ids of its vertices
    auto edgeSet = std::set<EdgeId>{};
    for (const auto & edge : m_edges)
    {
        edgeSet.insert(makeId(edge));
    }

    // merge vertices
    for (const auto & vertex : g2->m_vertices)
    {
        if (vertexMap.find(vertex->id()) == vertexMap.end())
        {
            vertexMap[vertex->id()] = vertex;
            m_vertices.emplace_back(vertex);
        }
    }

    // merge edges
    for (const auto & edge : g2->m_edges)
    {
        if (edgeSet.find(makeId(edge)) == edgeSet.end())
        {
            // rewire pointers
            const auto & v0 = vertexMap[edge->v0()->id()];
            const auto & v1 = vertexMap[edge->v1()->id()];
            edge->reset(v0, v1);

            edgeSet.insert(makeId(edge));
            m_edges.emplace_back(edge);
        }
    }
}


std::shared_ptr<Graph> Graph::minimumSpanningTree() const
{
    auto mst = std::make_shared<Graph>();

    //
    // TODO: implement minimum spanning tree computation
    //

    return mst;
}


std::ostream & operator<<(std::ostream & stream, const Graph & graph)
{
    for (const auto & edge : graph.m_edges)
    {
        stream << "(" << edge->v0()->id() << "," << edge->v1()->id() << ") ";
    }
    return stream;
}
