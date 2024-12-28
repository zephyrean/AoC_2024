#include <iostream>
#include <set>
#include <fstream>


size_t part_1(const std::multiset<int>& l1, const std::multiset<int>& l2) {
    auto it1 = l1.cbegin();
    auto it2 = l2.cbegin();
    size_t sum = 0;
    for (; it1 != l1.cend(); ++it1, ++it2) {
        sum += std::abs(*it1 - *it2);
    }
    return sum;
}

size_t part_2(const std::multiset<int>& l1, const std::multiset<int>& l2) {
    auto it1 = l1.cbegin();
    size_t result = 0;
    for (auto it1 = l1.cbegin(); it1 != l1.cend(); ++it1) {
        result += *it1 * l2.count(*it1);
    }
    return result;
}

int main(int argc, char* argv[]) {
    std::ifstream input(argv[1]);
    if (!input) {
        std::cout << "No input file found\n";
        return 1;
    }
    std::multiset<int> l1, l2;

    int n1, n2;
    while (input >> n1 >> n2) {
        l1.insert(n1);
        l2.insert(n2);
    }

    std::cout << "Part 1: " << part_1(l1, l2) << '\n';
    std::cout << "Part 2: " << part_2(l1, l2) << '\n';

    return 0;
}
