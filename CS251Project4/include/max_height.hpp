#pragma once
#include <iostream>
#include "graph.hpp"

namespace cs251 {

    class max_height {
        class union_find {
        private:
            std::vector<int> parent;
            std::vector<int> size;
        public:
            union_find(int n) {
                parent.resize(n);
                size.resize(n);
                for (int i = 0; i < n; i++) {
                    parent[i] = i;
                    size[i] = 1;
                }
            }

            int find(int i) {
                if (parent[i] == i) {
                    return i;
                } else {
                    parent[i] = find(parent[i]);
                    return parent[i];
                }
            }

            void unionBySize(int i, int j) {
                int iRep = find(i);
                int jRep = find(j);

                if (iRep == jRep) {
                    return;
                }

                if (size[iRep] < size[jRep]) {
                    parent[iRep] = jRep;
                    size[jRep] += size[iRep];
                } else {
                    parent[jRep] = iRep;
                    size[iRep] += size[jRep];
                }
            }
        };
    public:
        static int calculate(const graph &g);
    };

}
