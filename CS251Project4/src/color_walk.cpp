#include "color_walk.hpp"
#include <vector>
#include <climits>

using namespace cs251;

//region max heap
/*
void color_walk::heapifyUp(size_t i)
{
    size_t parent = (i - 1) / 2;
    while ((i > 0) && (m_heap[i].dist > m_heap[parent].dist)) {
        std::swap(m_heap[i], m_heap[parent]);
        i = parent;
        parent = (i - 1) / 2;
    }
}

void color_walk::heapifyDown(size_t i) {
    size_t left = 2 * i + 1;
    size_t right = 2 * i + 2;
    size_t largest = i;

    if ((left < m_heap.size()) && (m_heap[left].dist > m_heap[largest].dist)) {
        largest = left;
    }
    if ((right < m_heap.size()) && (m_heap[right].dist > m_heap[largest].dist)) {
        largest = right;
    }
    if (largest != i) {
        std::swap(m_heap[i], m_heap[largest]);
        heapifyDown(largest);
    }
}

void color_walk::push(state state) {
    m_heap.push_back(state);
    heapifyUp(m_heap.size() - 1);
}

void color_walk::pop() {
    if (m_heap.empty()) {
        return;
    }
    state root = m_heap[0];
    m_heap[0] = m_heap.back();
    m_heap.pop_back();
    heapifyDown(0);
}*/
//endregion

void color_walk::addEdgesByColor(std::vector<std::vector<graph_vertex>>& gPrime, graph_vertex v) {
    for (graph_edge e: v.m_edges) {
        if (e.col == RED) {
            gPrime[v.m_handle][0].m_edges.push_back(e);
        } else if (e.col == GREEN) {
            gPrime[v.m_handle][1].m_edges.push_back(e);
        } else {
            gPrime[v.m_handle][2].m_edges.push_back(e);
        }
    }
}

void color_walk::createGPrime(std::vector<graph_vertex> g, std::vector<std::vector<graph_vertex>>& gPrime, handle src) {
    std::vector<graph_vertex> q;
    std::vector<int> visited;
    q.resize(g.size());
    visited.resize(g.size());

    visited[src] = 1;
    q.push_back(g[src]);

    while (!q.empty()) {
        graph_vertex u = q.back();
        q.pop_back();
        //add to g'
        addEdgesByColor(gPrime, u);
        for (graph_edge e: u.m_edges) {
            if (visited[e.m_destinationHandle] == 0) {
                visited[e.m_destinationHandle] = 1;
                q.push_back(g[e.m_destinationHandle]);
            }
        }
    }
 }

color getPColor(char color) {
    if (color == RED) {
        return BLUE;
    } else if (color == GREEN) {
        return RED;
    } else {
        return GREEN;
    }
}

color getNColor(char color) {
    if (color == RED) {
        return GREEN;
    } else if (color == GREEN) {
        return BLUE;
    } else {
        return RED;
    }
}

void color_walk::BFS(std::vector<std::vector<graph_vertex>>& gPrime, handle src, color col, std::vector<std::vector<std::vector<int>>>& distance) {
    std::vector<graph_vertex> q;
    q.push_back(gPrime[src][col]);

    while (!q.empty()) {
        graph_vertex u = q.back();
        q.pop_back();
        // Relaxation
        for (graph_edge e: u.m_edges) {
            color n_color = getNColor(e.col);
            color p_color = getPColor(e.col);
            int n_dist = e.m_weight + distance[e.m_sourceHandle][col][p_color];
            if (n_dist <= distance[e.m_destinationHandle][col][e.col]) {
                distance[e.m_destinationHandle][col][e.col] = n_dist;
                q.push_back(gPrime[e.m_destinationHandle][n_color]);
            }
        }
    }
}

std::pair<int, char> findMinDistanceAndColor(std::vector<std::vector<std::vector<int>>>& distances, int i) {
    int min = INT_MAX;
    char minColor = NONE;
    for (color c : {RED, GREEN, BLUE}) {
        for (int k = 0; k < 3; k++) {
            if (distances[i][c][k] < min) {
                min = distances[i][c][k];
                if (min == 0) {
                    minColor = '-';
                } else {
                    minColor = (c == 0) ? 'R' : (c == 1) ? 'G' : (c == 2) ? 'B' : '-';
                }
            }
        }
    }
    return std::make_pair(min, minColor);
}

std::vector<std::pair<char, int>> color_walk::calculate(const graph& g, const handle startHandle) {
    //initialize and create g'
    static std::vector<std::vector<graph_vertex>> gPrime_vertices;
    gPrime_vertices.resize(g.get_vertices().size(), std::vector<graph_vertex>(3));
    createGPrime(g.get_vertices(), gPrime_vertices, startHandle);

    //initialize distances
    std::vector<std::vector<std::vector<int>>> distances;
    distances.resize(gPrime_vertices.size());

    for (int i = 0; i < gPrime_vertices.size(); i++) {
        distances[i].resize(3);
        for (int j = 0; j < 3; j++) {
            distances[i][j].resize(3);
            for (int k = 0; k < 3; k++) {
                distances[i][j][k] = INT_MAX;
            }
        }
    }
    for (color c : {RED, GREEN, BLUE}) {
        for (int i = 0; i < 3; i++) {
            distances[startHandle][c][i] = 0;
        }
    }

    //find sssp for each tree
    BFS(gPrime_vertices, startHandle, RED, distances);
    BFS(gPrime_vertices, startHandle, GREEN, distances);
    BFS(gPrime_vertices, startHandle, BLUE, distances);

    /*for (int i = 0; i < distances.size(); i++) {
        std::cout << "vertex: " << i << std::endl;
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                std::string s = (j == 0) ? "R" : (j == 1) ? "G" : (j == 2) ? "B" : "";
                std::cout << s << " ";
                std::cout << distances[i][j][k] << std::endl;
            }
        }
    }*/

    //return shortest path for each vertex from src
    std::vector<std::pair<char, int>> shortestDistances;
    for (int i = 0; i < distances.size(); i++) {
        std::pair<int, char> minDist = findMinDistanceAndColor(distances, i);
        if (minDist.first == INT_MAX) {
            shortestDistances.emplace_back('-', -1);
        } else {
            shortestDistances.emplace_back(minDist.second, minDist.first);
        }
    }

    /*std::cout << startHandle << " ";
    for (auto p : shortestDistances) {
        std::cout << p.first << " " << p.second << " ";
    }
    std::cout << std::endl;*/

    return shortestDistances;
    //throw std::logic_error("scc::" + std::string(__FUNCTION__) + "() not implemented");
}


