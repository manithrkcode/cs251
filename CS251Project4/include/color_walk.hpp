#pragma once
#include <iostream>
#include <vector>
#include "graph.hpp"

namespace cs251 {

    class color_walk {
        //static std::vector<std::vector<graph_vertex>> gPrime_vertices;
    public:
        static std::vector<std::pair<char, int>> calculate(const graph& g, handle startHandle);
        static void createGPrime(std::vector<graph_vertex> g, std::vector<std::vector<graph_vertex>>& gPrime, handle src);
        static void addEdgesByColor(std::vector<std::vector<graph_vertex>>& gPrime, graph_vertex v);

        static void BFS(std::vector<std::vector<graph_vertex>> &gPrime, handle src, color col,
            std::vector<std::vector<std::vector<int>>> &distance);
    };

}
