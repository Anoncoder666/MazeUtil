#ifndef RANDOMSET_H
#define RANDOMSET_H
#include <unordered_map>
#include <stdexcept>
struct pair_hash {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1,T2>& p) const {
        std::size_t h1 = std::hash<T1>{}(p.first);
        std::size_t h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);  // simple hash combine
    }
};

template <typename T1, typename T2>
class RandomPairSet {
    using P = std::pair<T1, T2>;
    std::vector<P> arr;
    unordered_map<P, size_t, pair_hash> pos;

public:
    void insert(const P& x) {
        if (pos.count(x)) return;
        arr.push_back(x);
        pos[x] = arr.size() - 1;
    }

    void remove(const P& x) {
        if (!pos.count(x)) return;
        size_t idx = pos[x];
        P last = arr.back();
        arr[idx] = last;
        pos[last] = idx;
        arr.pop_back();
        pos.erase(x);
    }

    P getRandom() const {
        if (arr.empty()) throw std::out_of_range("RandomPairSet is empty");
        std::uniform_int_distribution<size_t> dist(0, arr.size() - 1);
        return arr[dist(rng)];
    }

    bool contains(const P& x) const {
        return pos.contains(x);
    }

    bool empty() const {
        return arr.empty();
    }

    [[nodiscard]] size_t size() const {
        return arr.size();
    }

    void clear() {
        arr.clear();
        pos.clear();
    }
};

#endif
