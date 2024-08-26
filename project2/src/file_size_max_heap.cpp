#include "file_size_max_heap.hpp"
using namespace cs251;

void file_size_max_heap::heapifyUp(int index) {
    if (index == 0)
        return;

    int parentIndex = (index - 1) / 2;
    if (m_nodes[parentIndex].m_value < m_nodes[index].m_value) {
        std::swap(m_nodes[index], m_nodes[parentIndex]);
        heapifyUp(parentIndex);
    }
}

void file_size_max_heap::heapifyDown(int index) {
    int biggest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < m_nodeSize && m_nodes[left].m_value > m_nodes[biggest].m_value) {
        biggest = left;
    }
    if (right < m_nodeSize && m_nodes[right].m_value > m_nodes[biggest].m_value) {
        biggest = right;
    }

    if (biggest != index) {
        std::swap(m_nodes[index], m_nodes[biggest]);
        heapifyDown(biggest);
    }
}

void file_size_max_heap::push(const size_t fileSize, const handle handle) {
    file_size_max_heap_node newNode;
    newNode.m_handle = handle;
    newNode.m_value = fileSize;

    m_nodes.push_back(newNode);
    m_nodeSize++;

    heapifyUp(m_nodeSize - 1);
}

handle file_size_max_heap::top() const
{
    class heap_empty : public std::runtime_error {
    public: heap_empty() : std::runtime_error("Heap is empty!") {} };

    if (m_nodeSize > 0) {
        return m_nodes[0].m_handle;
    }
    else {
        throw heap_empty();
    }
}

void file_size_max_heap::remove(const handle handle) {
    int indexToRemove = -1;
    for (int i = 0; i < m_nodeSize; ++i) {
        if (m_nodes[i].m_handle == handle) {
            indexToRemove = i;
            break;
        }
    }

    if (indexToRemove != -1) {
        std::swap(m_nodes[indexToRemove], m_nodes[m_nodeSize - 1]);
        m_nodeSize--;

        heapifyDown(indexToRemove);
    }
}