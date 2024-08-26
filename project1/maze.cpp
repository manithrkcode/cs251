#include "maze.hpp"
#include "cyclic_double_queue.hpp"
using namespace cs251;


Direction maze::incrementDirection(Direction currentDirection)
{
    switch (currentDirection) {
        case Direction::Right:
            return Direction::Left;
        case Direction::Left:
            return Direction::Top;
        case Direction::Top:
            return Direction::Bottom;
        case Direction::Bottom:
            return Direction::Back;
        case Direction::Back:
            return Direction::Front;
        case Direction::Front:
            return Direction::Right;
        default:
            return Direction::Mark;
    }
}

size_t maze::getNeighborIndex(size_t index, Direction direction) const
{
    size_t x = index % m_dimX;
    size_t y = (index / m_dimX) % m_dimY;
    size_t z = index / (m_dimX * m_dimY);

    switch (direction) {
        case Direction::Front:
            if ((z != 0) && ((z - 1) >= 0)) {
                return get_index(x, y, z - 1);
            } else {
                return FAILURE;
            }
        case Direction::Right:
            if ((x + 1) < m_dimX) {
                return get_index(x + 1, y, z);
            } else {
                return FAILURE;
            }
        case Direction::Left:
            if ((x != 0) && ((x - 1) >= 0)) {
                return get_index(x - 1, y, z);
            } else {
                return FAILURE;
            }
        case Direction::Top:
            if ((y + 1) < m_dimY) {
                return get_index(x, y + 1, z);
            } else {
                return FAILURE;
            }
        case Direction::Bottom:
            if ((y != 0) && ((y - 1) >= 0)) {
                return get_index(x, y - 1, z);
            } else {
                return FAILURE;
            }
        case Direction::Back:
            if ((z + 1) < m_dimZ ) {
                return get_index(x, y, z + 1);
            } else {
                return FAILURE;
            }
        default:
            return FAILURE;
    }
}

void maze::removeWall(size_t current, size_t neighbor, Direction direction) {
    switch (direction) {
        case Direction::Front:
            m_map[current] &= ~static_cast<unsigned>(Direction::Front);
            m_map[neighbor] &= ~static_cast<unsigned>(Direction::Back);
            break;
        case Direction::Right:
            m_map[current] &= ~static_cast<unsigned>(Direction::Right);
            m_map[neighbor] &= ~static_cast<unsigned>(Direction::Left);
            break;
        case Direction::Left:
            m_map[current] &= ~static_cast<unsigned>(Direction::Left);
            m_map[neighbor] &= ~static_cast<unsigned>(Direction::Right);
            break;
        case Direction::Top:
            m_map[current] &= ~static_cast<unsigned>(Direction::Top);
            m_map[neighbor] &= ~static_cast<unsigned>(Direction::Bottom);
            break;
        case Direction::Bottom:
            m_map[current] &= ~static_cast<unsigned>(Direction::Bottom);
            m_map[neighbor] &= ~static_cast<unsigned>(Direction::Top);
            break;
        case Direction::Back:
            m_map[current] &= ~static_cast<unsigned>(Direction::Back);
            m_map[neighbor] &= ~static_cast<unsigned>(Direction::Front);
            break;
        default:
            break;
    }
}

bool maze::isBlocked(size_t index, Direction direction) {
    switch (direction) {
        case Direction::Front:
            return m_map[index] & static_cast<unsigned>(Direction::Front);
        case Direction::Right:
            return m_map[index] & static_cast<unsigned>(Direction::Right);
        case Direction::Left:
            return m_map[index] & static_cast<unsigned>(Direction::Left);
        case Direction::Top:
            return m_map[index] & static_cast<unsigned>(Direction::Top);
        case Direction::Bottom:
            return m_map[index] & static_cast<unsigned>(Direction::Bottom);
        case Direction::Back:
            return m_map[index] & static_cast<unsigned>(Direction::Back);
    }
}

void displayBinary(unsigned num) {
    int size = sizeof(unsigned) * 2;

    for (int i = size - 1; i >= 0; --i) {
        if ((num >> i) & 1) {
            std::cout << "1";
        } else {
            std::cout << "0";
        }

        if (i % 4 == 0) {
            std::cout << " ";
        }
    }
    std::cout << "" << std::endl;
}

void maze::initialize(const size_t dimX, const size_t dimY, const size_t dimZ, unsigned seed)
{
    m_dimX = dimX;
    m_dimY = dimY;
    m_dimZ = dimZ;

    //Initialize array
    m_map = new unsigned[(m_dimX * m_dimY * m_dimZ)];
    for (size_t x = 0; x < m_dimX; x++) {
        for (size_t y = 0; y < m_dimY; y++) {
            for (size_t z = 0; z < m_dimZ; z++) {
                m_map[get_index(x, y, z)] = static_cast<unsigned>(63);
            }
        }
    }

    //Initialize stack
    cyclic_double_queue<size_t> q;
    q.enqueue_back(0);

    while (!q.empty()) {
        //Get top of stack (current coordinate) and mark as visited
        size_t currentCoord = q.back();
        m_map[currentCoord] |= static_cast<unsigned>(Direction::Mark);

        //Get random coord
        Direction nextDirection = get_next_direction(seed);

        int i = 0;
        while (true) {
            //No valid neighbors
            if (i > 5) {
                q.pop_back();
                break;
            }
            //Get neighbor coord
            size_t targetNeighborCoord = getNeighborIndex(currentCoord, nextDirection);

            //If valid and unvisited remove walls and push to stack
            if ((targetNeighborCoord != FAILURE) && (static_cast<int>(m_map[targetNeighborCoord]) <= 63)) {
                removeWall(currentCoord, targetNeighborCoord, nextDirection);
                q.enqueue_back(targetNeighborCoord);
                break;
            }
            //Change direction
            nextDirection = incrementDirection(nextDirection);
            i++;
        }
    }
    //clean maze
    for (size_t i = 0; i < (m_dimX * m_dimY * m_dimZ); i++) {
        m_map[i] &= ~static_cast<unsigned>(Direction::Mark);
    }
}

bool maze::solve(const size_t startX, const size_t startY, const size_t startZ, const  size_t endX, const size_t endY, const size_t endZ)
{
    cyclic_double_queue<size_t> q;
    q.enqueue_back(get_index(startX, startY, startZ));
    size_t* prev = new size_t[(m_dimX * m_dimY * m_dimZ)];

    //clean maze
    for (size_t i = 0; i < (m_dimX * m_dimY * m_dimZ); i++) {
        m_map[i] &= ~static_cast<unsigned>(Direction::Mark);
    }

    while (!q.empty()) {
        size_t currentIndex = q.dequeue_back();
        m_map[currentIndex] |= static_cast<unsigned>(Direction::Mark);

        if (currentIndex == get_index(endX, endY, endZ)) {
            //clean maze
            for (size_t i = 0; i < (m_dimX * m_dimY * m_dimZ); i++) {
                m_map[i] &= ~static_cast<unsigned>(Direction::Mark);
                //print_walls(0);
            }
            //trace path
            while (currentIndex != get_index(startX, startY, startZ)) {
                m_map[currentIndex] |= static_cast<unsigned>(Direction::Mark);
                currentIndex = prev[currentIndex];
            }
            m_map[currentIndex] |= static_cast<unsigned>(Direction::Mark);
            return true;
        }

        Direction nextDirection = Direction::Front;
        for (size_t j = 0; j < 6; j++) {
            if (!isBlocked(currentIndex, nextDirection)) {
                size_t targetNeighborIndex = getNeighborIndex(currentIndex, nextDirection);
                if ((targetNeighborIndex != FAILURE) && (static_cast<int>(m_map[targetNeighborIndex]) <= 63)) {
                    q.enqueue_back(targetNeighborIndex);
                    prev[targetNeighborIndex] = currentIndex;
                }
            }
            nextDirection = incrementDirection(nextDirection);
        }
    }
    return false;
}

std::string maze::print_walls(const size_t z) const
{
    std::stringstream output;

    for (size_t y = 0; y < m_dimY; y++) {
        //LINE 1
        for (size_t x = 0; x < m_dimX; x++) {
            output << "+";
            if (m_map[get_index(x, y, z)] & static_cast<unsigned>(Direction::Bottom)) {
                output << "---";
            }
            else {
                output << "   ";
            }
        }
        output << "+" << std::endl;
        //LINE 2
        for (size_t x = 0; x < m_dimX; x++) {
            if (m_map[get_index(x, y, z)] & static_cast<unsigned>(Direction::Left)) {
                output << "| ";
            }
            else {
                output << "  ";
            }
            if ((m_map[get_index(x, y, z)] & static_cast<unsigned>(Direction::Front)) && (m_map[get_index(x, y, z)] & static_cast<unsigned>(Direction::Back))) {
                output << "X ";
            }
            else if (m_map[get_index(x, y, z)] & static_cast<unsigned>(Direction::Front)) {
                output << "F ";
            }
            else if (m_map[get_index(x, y, z)] & static_cast<unsigned>(Direction::Back)) {
                output << "B ";
            }
            else {
                output << "  ";
            }
        }
        output << "|" << std::endl;
        //LAST LINE
        if (y == (m_dimY - 1)) {
            for (size_t x = 0; x < m_dimX; x++) {
                output << "+";
                if (m_map[get_index(x, y, z)] & static_cast<unsigned>(Direction::Top)) {
                    output << "---";
                }
                else {
                    output << "   ";
                }
            }
            output << "+" << std::endl;
        }
    }
    return output.str();
}

std::string maze::print_marks(const size_t z) const
{
    std::stringstream output;

    for (size_t y = 0; y < m_dimY; y++) {
        //LINE 1
        for (size_t x = 0; x < m_dimX; x++) {
            output << "+";
            if (m_map[get_index(x, y, z)] & static_cast<unsigned>(Direction::Bottom)) {
                output << "---";
            }
            else {
                output << "   ";
            }
        }
        output << "+" << std::endl;
        //LINE 2
        for (size_t x = 0; x < m_dimX; x++) {
            if (m_map[get_index(x, y, z)] & static_cast<unsigned>(Direction::Left)) {
                output << "| ";
            }
            else {
                output << "  ";
            }

            if (m_map[get_index(x, y, z)] & static_cast<unsigned>(Direction::Mark)) {
                output << "# ";
            }
            else {
                output << "  ";
            }
        }
        output << "|" << std::endl;
        //LAST LINE
        if (y == (m_dimY - 1)) {
            for (size_t x = 0; x < m_dimX; x++) {
                output << "+";
                if (m_map[get_index(x, y, z)] & static_cast<unsigned>(Direction::Top)) {
                    output << "---";
                }
                else {
                    output << "   ";
                }
            }
            output << "+" << std::endl;
        }
    }
    return output.str();
}

void maze::get_dim(size_t& dimX, size_t& dimY, size_t& dimZ) const
{
    dimX = m_dimX;
    dimY = m_dimY;
    dimZ = m_dimZ;
}

Direction maze::get_next_direction(unsigned& seed)
{
    //Please do not modify this function!
    //Any modification of this function may result in a zero for all grading cases.
    std::mt19937 gen(seed);
    seed = gen();
    return static_cast<Direction>(1 << seed % 6);
}

size_t maze::get_index(const size_t x, const size_t y, const size_t z) const
{
    return x + (y * m_dimX) + (z * (m_dimX * m_dimY));
}
