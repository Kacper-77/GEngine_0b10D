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

    std::vector<T> QueryNeighbors(int x, int y) const {
        std::vector<T> result;
        Int2 center = {x, y};

        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                Int2 neighbor = {center.x + dx, center.y + dy};
                auto it = m_cells.find(neighbor);
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
        return cellSize;
    }

private:
    std::unordered_map<Int2, std::vector<T>> m_cells;
    static inline const int cellSize = 64;
};