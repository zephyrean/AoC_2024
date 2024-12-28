#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>


bool is_safe(std::istringstream& is) {
    bool safe_inc = true;
    bool safe_dec = true;
    int prev, cur;
    is >> prev;
    while (is >> cur) {
        safe_inc = safe_inc && cur > prev && (cur - prev <= 3);
        safe_dec = safe_dec && cur < prev && (prev - cur <= 3);

        if (!safe_inc && !safe_dec) {
            return false;
        }

        prev = cur;
    }
    return true;
}

bool is_safe_dampened(std::istringstream& is) {
    std::vector<int> v((std::istream_iterator<int>(is)),
                       (std::istream_iterator<int>()));
    if (v.size() < 3) {
        return true;
    }

    auto is_inc = [&](int n1, int n2) {
        return n1 < n2 && (n2 - n1 <= 3);
    };
    auto is_dec = [&](int n1, int n2) {
        return is_inc(n2, n1);
    };

    auto check_vector = [&v](auto P) {
        bool found_bad_level = false;
        for (size_t i = 1; i < v.size(); ++i) {
            if (!P(v[i-1], v[i])) {
                if (i == v.size() - 1 || P(v[i-1], v[i+1])) {  // ok to get rid of v[i]?
                    if (found_bad_level) return false;
                    found_bad_level = true;
                    ++i;
                } else if (i == 1 || P(v[i-2], v[i])) {  // ok to get rid of v[i-1]?
                    if (found_bad_level) return false;
                    found_bad_level = true;
                } else {
                    return false;
                }
            }
        }
        return true;
    };

    return check_vector(is_inc) || check_vector(is_dec);
}

template<typename Pred>
size_t count_lines(std::fstream& input, Pred is_safe) {
    std::string line;
    size_t num_safe = 0;
    while (getline(input, line)) {
        std::istringstream is(line);
        num_safe += is_safe(is);
    }
    return num_safe;
}

int main(int argc, char* argv[]) {
    std::fstream input(argv[1]);
    if (!input) {
        std::cout << "No input file found";
        return 1;
    }

    std::cout << "Part 1: " << count_lines(input, is_safe) << '\n';
    input.clear();
    input.seekg(0);
    std::cout << "Part 2: " << count_lines(input, is_safe_dampened) << '\n';

    return 0;
}
