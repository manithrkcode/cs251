#include "max_height.hpp"

using namespace cs251;

int max_height::calculate(const graph& g) {
    std::vector<graph_vertex> vertices = g.get_vertices();
    std::vector<graph_edge> edges = g.get_edges();
    union_find uf = union_find(vertices.size());

    std::vector<graph_edge> tree;
    while (tree.size() < (vertices.size() - 1)) {
        graph_edge e = edges.back();
        if (uf.find(e.m_sourceHandle) != uf.find(e.m_destinationHandle)) {
            tree.push_back(e);
            uf.unionBySize(e.m_sourceHandle, e.m_destinationHandle);
        }
        edges.pop_back();
    }
    return tree.back().m_weight;
}
