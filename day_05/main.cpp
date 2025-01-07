#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <queue>
#include <algorithm>


using TopologicalOrder = std::unordered_map<int, int>;

// The only thing used from this graph is the has_edge(...) method :(
class Graph {
public:
    void add_edge(int from, int to) {
        _graph[from].insert(to);
    }

    TopologicalOrder get_topological_order() const {
        std::stack<int> top_order;
        std::unordered_set<int> visited;
        for (const auto& [v, _] : _graph) {
            if (!visited.contains(v)) {
                top_sort(v, visited, top_order);
            }
        }

        TopologicalOrder result;
        int cur_order = 0;
        while (!top_order.empty()) {
            result.insert({top_order.top(), cur_order++});
            top_order.pop();
        }
        return result;
    }

    TopologicalOrder get_topological_order_with_grouping() const {
        TopologicalOrder levels;

        TopologicalOrder input_deg;
        for (const auto& [from, to] : _graph) {
            if (!input_deg.contains(from)) {
                input_deg.insert({from, 0});
            }
            for (auto v : to) {
                ++input_deg[v];
            }
        }

        std::queue<int> q;
        int cur_level = 0;
        for (auto [v, deg] : input_deg) {
            if (deg == 0) {
                q.push(v);
                levels[v] = cur_level;
            }
        }

        while (!q.empty()) {
            auto cur_v = q.front();
            q.pop();
            if (_graph.contains(cur_v)) {
                for (auto w : _graph.at(cur_v)) {
                    if (--input_deg[w] == 0) {
                        q.push(w);
                        levels[w] = levels[cur_v] + 1;
                    }
                }
            }
        }

        return levels;
    }

    bool has_edge(int from, int to) const {
        return _graph.contains(from) && _graph.at(from).contains(to);
    }

    bool has_path(int from, int to) const {
        std::queue<int> q;
        std::unordered_set<int> visited;

        q.push(from);
        visited.insert(from);

        while (!q.empty()) {
            auto v = q.front();
            q.pop();
            if (_graph.contains(v)) {
                for (auto w : _graph.at(v)) {
                    if (w == to) {
                        return true;
                    }
                    if (!visited.contains(w)) {
                        q.push(w);
                        visited.insert(w);
                    }
                }
            }
        }
        return false;
    }

private:

    void top_sort(int v, std::unordered_set<int>& visited, std::stack<int>& top_order) const {
        visited.insert(v);
        if (_graph.contains(v)) {
            for (auto w : _graph.at(v)) {
                if (!visited.contains(w)) {
                    top_sort(w, visited, top_order);
                }
            }
        }
        top_order.push(v);
    }

    std::unordered_map<int, std::unordered_set<int>> _graph;
};


void read_graph(std::fstream& input, Graph& g) {
    std::string s;
    int from, to;
    char delim;
    while (getline(input, s) && !s.empty()) {
        std::istringstream is(s);
        is >> from >> delim >>  to;
        g.add_edge(from, to);
    }
}

std::vector<int> read_vector(const std::string& s) {
    std::istringstream is(s);
    std::vector<int> result;
    int n;
    char delim;
    while (is) {
        is >> n;
        result.push_back(n);
        is >> delim;
    }
    return result;
}

bool is_in_right_order(const std::vector<int>& vec, const Graph& g) {
    for (int i = 0; i < vec.size() - 1; ++i) {
        if (g.has_edge(vec[i+1], vec[i])) {
            return false;
        }
    }
    return true;
}

// Did not read the task carefully - no need for any graphs here...
//bool is_in_right_order(const std::vector<int>& vec, const Graph& g) {
//    for (int i = 0; i < vec.size() - 1; ++i) {
//        if (!g.has_path(vec[i], vec[i+1])) {
//            return false;
//        }
//    }
//    return true;
//}
//bool is_in_right_order(const std::vector<int>& vec, const TopologicalOrder& top_order) {
//    int prev_order = -1;
//    for (auto v : vec) {
//        if (top_order.at(v) < prev_order) {
//            return false;
//        }
//        prev_order = top_order.at(v);
//    }
//    return true;
//}

template<typename F>
size_t read_vec_and_calc_sum(std::fstream& input, F f) {
    size_t sum = 0;
    std::string s;
    while (getline(input, s)) {
        sum += f(std::move(read_vector(s)));
    }
    return sum;
}

size_t part_1(std::fstream& input, const Graph& g) {
    return read_vec_and_calc_sum(input,
                                 [&](std::vector<int>&& vec) -> size_t {
                                    return is_in_right_order(vec, g) ? vec[vec.size() / 2] : 0;
                                 });
}

size_t part_2(std::fstream& input, const Graph& g) {
    return read_vec_and_calc_sum(input,
                                 [&](std::vector<int>&& vec) -> size_t {
                                    if (is_in_right_order(vec, g)) {
                                        return 0;
                                    }
                                    std::sort(vec.begin(), vec.end(),
                                              [&](int left, int right) {
                                                return g.has_edge(left, right);
                                              });
                                    return vec[vec.size() / 2];
                                 });
}


int main(int argc, char* argv[]) {
    std::fstream input(argv[1]);
    if (!input) {
        std::cout << "Input file not found";
        return 1;
    }

    Graph g;
    read_graph(input, g);

    std::cout << "Part 1: " << part_1(input, g) << '\n';
    input.clear();
    input.seekg(0);
    std::cout << "Part 2: " << part_2(input, g) << '\n';

    return 0;
}
