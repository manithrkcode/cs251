#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "graph.hpp"

using namespace cs251;

void graph::edgeSort() {
    for (int i = 1; i < m_edges.size(); i++) {
        graph_edge key = m_edges[i];
        int j = i - 1;
        while ((j >= 0) && (m_edges[j].m_weight > key.m_weight)) {
            m_edges[j + 1] = m_edges[j];
            j = j - 1;
        }
        m_edges[j + 1] = key;
    }
}


void graph::read_edge_weights(const std::string& filePath) {
    std::ifstream inFile;
    inFile.open(filePath);

    if (!inFile.is_open()) {
        std::cerr << "Error: Unable to open file " << filePath << std::endl;
        return;
    }

    std::string line;
    int vertices, edges;
    inFile >> vertices >> edges;
    m_vertices.resize(vertices);
    m_edges.resize(edges);
    std::getline(inFile, line);

    graph_vertex u, v;
    for (int i = 0; i < edges; i++) {
        int weight = 0;
        //inFile >> u.m_handle >> v.m_handle >> weight;
        std::getline(inFile, line);
        std::istringstream iss(line);
        iss >> u.m_handle >> v.m_handle >> weight;

        graph_edge tmp;
        tmp.m_sourceHandle = u.m_handle;
        tmp.m_destinationHandle = v.m_handle;
        tmp.m_weight = weight;

        m_vertices[u.m_handle].m_handle = u.m_handle;
        m_vertices[u.m_handle].m_edges.push_back(tmp);
        m_edges[i] = tmp;
    }
    edgeSort();
}

void graph::read_edge_colors(const std::string& filePath) {
    std::ifstream inFile;
    inFile.open(filePath);

    if (!inFile.is_open()) {
        std::cerr << "Error: Unable to open file " << filePath << std::endl;
        return;
    }

    int vertices, edges;
    inFile >> vertices >> edges;
    m_vertices.resize(vertices);
    m_edges.resize(edges);

    graph_vertex u, v;
    for (int i = 0; i < edges; i++) {
        int weight;
        std::string color;
        inFile >> u.m_handle >> v.m_handle >> weight >> color;

        graph_edge tmp;
        tmp.m_sourceHandle = u.m_handle;
        tmp.m_destinationHandle = v.m_handle;
        tmp.m_weight = weight;
        tmp.col = (color == "R") ? RED : (color == "G") ? GREEN : (color == "B") ? BLUE : NONE;

        m_vertices[u.m_handle].m_handle = u.m_handle;
        m_vertices[u.m_handle].m_edges.push_back(tmp);
        m_edges[i] = tmp;
    }
    edgeSort();
}

std::vector<graph_vertex> graph::get_vertices() const {
    return m_vertices;
}

std::vector<graph_edge> graph::get_edges() const {
    return m_edges;
}
