#pragma once
#include <utility>
#include <functional>

struct pair_hash {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1,T2>& p) const noexcept {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        // Simple but effective hash combine
        return h1 ^ (h2 << 1);
    }
};