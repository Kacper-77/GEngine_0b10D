#pragma once

#include "Int2.h"
#include <vector>
#include <unordered_map>

template <typename T>
class SpatialGrid {
public:
    // INSERT, QUERY, CLEAR, GETALLCELLS
    void Clear() {
        m_cells.clear();
    }

    void Insert(const Int2& cell, T item) {
        m_cells[cell].push_back(item);
    }

    const std::vector<T>& Query(int x, int y) const {
        static const std::vector<T> empty;
        Int2 cell = {x, y};
        auto it = m_cells.find(cell);

        return it != m_cells.end() ? it->second : empty;
    }

    // Return all entities from the 8 neighboring cells around (x, y)
    std::vector<T> QueryNeighbors(int x, int y) const {
        std::vector<T> result; // store found entities
        Int2 center = {x, y};  // current cell coordinates

        // Loop through offsets in a 3x3 area around the center
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue; // skip the center cell itself

                Int2 neighbor = {center.x + dx, center.y + dy}; // neighbor cell coordinates
                auto it = m_cells.find(neighbor);               // check if neighbor cell exists

                if (it != m_cells.end()) {
                    result.insert(result.end(), it->second.begin(), it->second.end());
                }
            }
        }
        return result;
    }

    const std::unordered_map<Int2, std::vector<T>>& GetAllCells() const {
        return m_cells;
    }

    int GetCellSize() {
        return cellSize_;
    }

    void SetCellSize(int size) {
        cellSize_ = size;
    }

private:
    std::unordered_map<Int2, std::vector<T>> m_cells;
    static inline int cellSize_ = 64;
};