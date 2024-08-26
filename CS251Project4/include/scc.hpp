#pragma once
#include "graph.hpp"

namespace cs251 {

    class scc {
    public:
        static int search(const graph& g);

        static void strong_connect(int v, int &index, int &SCC, std::vector<int> &stack, std::vector<int> &indexMap,
                            std::vector<int> &lowLinkMap, std::vector<bool> &onStack, const graph &g);
    };

}
