#include "scc.hpp"

using namespace cs251;

void scc::strong_connect(int v, int& index, int& SCC, std::vector<int>& stack, std::vector<int>& indexMap,
                         std::vector<int>& lowLinkMap, std::vector<bool>& onStack, const graph& g) {
    indexMap[v] = lowLinkMap[v] = index;
    index = index + 1;
    stack.push_back(v);
    onStack[v] = true;

    for (int i = 0; i < g.get_vertices()[v].m_edges.size(); i++) {
        int w = g.get_vertices()[v].m_edges[i].m_destinationHandle;
        if (indexMap[w] == -1) {
            strong_connect(w, index, SCC, stack, indexMap, lowLinkMap, onStack, g);
            lowLinkMap[v] = std::min(lowLinkMap[v], lowLinkMap[w]);
        } else if (onStack[w] == 1) {
            lowLinkMap[v] = std::min(lowLinkMap[v], indexMap[w]);
        }
    }

    if (lowLinkMap[v] == indexMap[v]) {
        int w;
        do {
            w = stack.back();
            stack.pop_back();
            onStack[w] = false;
        } while (w != v);
        SCC = SCC + 1;
    }
}

int scc::search(const graph& g) {
    int index = 0;
    int SCC = 0;

    std::vector<graph_vertex> vertices = g.get_vertices();

    std::vector<int> stack(vertices.size());
    std::vector<int> indexMap(vertices.size(), -1);
    std::vector<int> lowLinkMap(vertices.size(), -1);
    std::vector<bool> onStack(vertices.size(), false);

    //
    for (int i = 0; i < g.get_vertices().size(); i++) {
        if (indexMap[i] == -1) {
            strong_connect(i, index, SCC, stack, indexMap, lowLinkMap, onStack, g);
        }
    }

    return SCC;
    //throw std::logic_error("scc::" + std::string(__FUNCTION__) + "() not implemented");
}
