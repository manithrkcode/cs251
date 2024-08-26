#pragma once
#include "cstring"
#include "sstream"
#include "string"

#include <iostream>

namespace cs251
{
    template<typename T>
    class cyclic_double_queue
    {
    public:
        /**
         * \brief The constructor with settings.
         * \param initialCapacity Preset initial capacity of the array.
         * \param increaseFactor Preset increase factor for expanding the array.
         * \param decreaseFactor Preset decrease factor for shrinking the array.
         */
        cyclic_double_queue(size_t initialCapacity = 7, size_t increaseFactor = 2, size_t decreaseFactor = 2);
        /**
         * \brief The default destructor. You should make sure no memory leaks happens after the queue is destroyed.
         */
        ~cyclic_double_queue();
        /**
         * \brief Push the item at the front of the queue.
         * \param item The item to be added.
         */
        void enqueue_front(const T& item);
        /**
         * \brief Push the item at the back of the queue.
         * \param item The item to be added.
         */
        void enqueue_back(const T& item);
        /**
         * \brief Returns the occupancy of the queue.
         * \return Whether the queue is empty.
         */
        bool empty() const;
        /**
         * \brief Dequeue the element at the front of the queue.
         * \return The item dequeued.
         */
        T dequeue_front();
        /**
         * \brief Dequeue the element at the back of the queue.
         * \return The item dequeued.
         */
        T dequeue_back();
        /**
         * \brief Remove the item at the front of the queue, returns nothing.
         */
        void pop_front();
        /**
         * \brief Remove the item at the back of the queue, returns nothing.
         */
        void pop_back();
        /**
         * \brief Provide the access to the item at the front of the queue.
         * \return The modifiable reference to the item.
         */
        T& front();
        /**
         * \brief Provide the access to the item at the back of the queue.
         * \return The modifiable reference to the item.
         */
        T& back();
        /**
         * \brief Empty the queue, and resize the queue to initial capacity.
         * \n !Note! You should set the size and the front index of the queue to 0 when this function is called!
         */
        void clear();
        /**
         * \brief Print the queue's current status according to the handout.
         * \return The string represents the array. For format please refer to the handout.
         */
        std::string print_status() const;
        /**
         * \brief Returns the number of items currently in queue.
         * \return The size of the queue.
         */
        size_t get_size() const;
        /**
         * \brief Returns the total capacity of the array.
         * \return The capacity of the array.
         */
        size_t get_capacity() const;
    private:
        //TODO: You may add private members/methods here.
        /**
         * Item array, the underlying data structure for the queue
         */
        T* m_data = nullptr;
        /**
         * The front index of the queue within the array
         */
        size_t m_frontIndex = 0;
        size_t m_rearIndex = 0;
        /**
         * The length of the array, including the empty slots.
         */
        size_t m_capacity = 0;
        /**
         * Keeps track of the size of the queue
         */
        size_t m_size = 0;
        /**
         * Factor by which the queue length must be increased
         */
        size_t m_increaseFactor = 0;
        /**
         * Factor by which the queue length must be decreased
         */
        size_t m_decreaseFactor = 0;
        /**
         * The initial capacity of the queue, predefined as 7
         */
        size_t m_initialCapacity = 0;

        void increaseCapacity();

        void decreaseCapacity();
    };

    template <typename T>
    cyclic_double_queue<T>::cyclic_double_queue(const size_t initialCapacity, const size_t increaseFactor, size_t decreaseFactor)
    {
        m_initialCapacity = initialCapacity;
        m_increaseFactor = increaseFactor;
        m_decreaseFactor = decreaseFactor;
        m_data = new T[initialCapacity];
        m_capacity = m_initialCapacity;
    }

    template <typename T>
    cyclic_double_queue<T>::~cyclic_double_queue()
    {
        delete[] m_data;
    }

    template <typename T>
    void cyclic_double_queue<T>::increaseCapacity() {
        size_t newCapacity = m_capacity * m_increaseFactor;
        T* newData = new T[newCapacity];
        for (size_t i = 0; i < m_size; i++) {
            newData[i] = m_data[(m_frontIndex + i) % m_capacity];
        }
        m_frontIndex = 0;
        m_rearIndex = m_size;
        m_capacity = newCapacity;
        m_data = newData;
    }

    template <typename T>
    void cyclic_double_queue<T>::decreaseCapacity() {
        size_t newCapacity = m_capacity / m_decreaseFactor;
        if (newCapacity < m_initialCapacity) {
            newCapacity = m_initialCapacity;
        }
        T* newData = new T[newCapacity];
        for (size_t i = 0; i < m_size; i++) {
            newData[i] = m_data[(m_frontIndex + i) % m_capacity];
        }
        m_frontIndex = 0;
        m_rearIndex = m_size;
        m_capacity = newCapacity;
        m_data = newData;
    }

    template <typename T>
    void cyclic_double_queue<T>::enqueue_front(const T& item)
    {
        if (m_size == m_capacity) {
            increaseCapacity();
        }
        m_frontIndex = (m_frontIndex - 1 + m_capacity) % m_capacity;
        m_data[m_frontIndex] = item;
        m_size++;
    }

    template <typename T>
    void cyclic_double_queue<T>::enqueue_back(const T& item)
    {
        if (m_size == m_capacity) {
            increaseCapacity();
        }
        m_data[m_rearIndex] = item;
        m_rearIndex = (m_rearIndex + 1) % m_capacity;
        m_size++;
    }

    template <typename T>
    T cyclic_double_queue<T>::dequeue_front()
    {
        if (m_size == 0) {
            throw std::out_of_range("cyclic_double_queue is empty!") ;
        }

        T value = m_data[m_frontIndex];
        m_frontIndex = (m_frontIndex + 1) % m_capacity;
        m_size--;
        if ((m_capacity != m_initialCapacity) && ((m_size * 4) < m_capacity)) {
            decreaseCapacity();
        }
        return value;
    }

    template <typename T>
    T cyclic_double_queue<T>::dequeue_back()
    {
        if (m_size == 0) {
            throw std::out_of_range("cyclic_double_queue is empty!") ;
        }

        m_rearIndex = (m_rearIndex - 1 + m_capacity) % m_capacity;
        T value = m_data[m_rearIndex];
        m_size--;
        if ((m_capacity != m_initialCapacity) && ((m_size * 4) < m_capacity)) {
            decreaseCapacity();
        }
        return value;
    }

    template <typename T>
    void cyclic_double_queue<T>::pop_front()
    {
        if (m_size == 0) {
            throw std::out_of_range("cyclic_double_queue is empty!") ;
        }

        m_frontIndex = (m_frontIndex + 1) % m_capacity;
        m_size--;
        if ((m_capacity != m_initialCapacity) && ((m_size * 4) < m_capacity)) {
            decreaseCapacity();
        }
        return;
    }

    template <typename T>
    void cyclic_double_queue<T>::pop_back()
    {
        if (m_size == 0) {
            throw std::out_of_range("cyclic_double_queue is empty!") ;
        }

        m_rearIndex = (m_rearIndex - 1 + m_capacity) % m_capacity;
        m_size--;
        if ((m_capacity != m_initialCapacity) && ((m_size * 4) < m_capacity)) {
            decreaseCapacity();
        }
        return;
    }

    template <typename T>
    bool cyclic_double_queue<T>::empty() const
    {
        if (m_size == 0) {
            return true;
        }
        return false;
    }

    template <typename T>
    T& cyclic_double_queue<T>::front()
    {
        if (m_size == 0) {
            throw std::out_of_range("cyclic_double_queue is empty!") ;
        }
        return m_data[m_frontIndex];
    }

    template <typename T>
    T& cyclic_double_queue<T>::back()
    {
        if (m_size == 0) {
            throw std::out_of_range("cyclic_double_queue is empty!") ;
        }
        return m_data[(m_rearIndex - 1 + m_capacity) % m_capacity];
    }

    template <typename T>
    void cyclic_double_queue<T>::clear()
    {
        m_size = 0;
        m_frontIndex = 0;
        m_rearIndex = 0;
        m_capacity = m_initialCapacity;
        m_data = new T[m_capacity];
    }

    template <typename T>
    std::string cyclic_double_queue<T>::print_status() const
    {
        std::string print = "";

        for (int i = 0; i < m_capacity; i++) {
            if ((m_size > 0) && (((m_rearIndex < m_frontIndex) && ((i < m_rearIndex) || (i >= m_frontIndex)))
                || ((m_frontIndex < m_rearIndex) && ((i < m_rearIndex) && (i >= m_frontIndex))))
                || (m_size > 1 && m_frontIndex == m_rearIndex)) {
                print += "[+]";
            } else {
                print += "[-]";
            }
        }
        return print;
    }

    template <typename T>
    size_t cyclic_double_queue<T>::get_size() const
    {
        return m_size;
    }

    template <typename T>
    size_t cyclic_double_queue<T>::get_capacity() const
    {
        return m_capacity;
    }
}