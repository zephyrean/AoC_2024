#include <iostream>
#include <fstream>
#include <regex>
#include <iterator>
#include <string>


size_t part_1(std::ifstream& input) {
    std::regex pattern(R"(mul\((\d{1,3}),(\d{1,3})\))");

    std::istream_iterator<std::string> file_start(input);
    std::istream_iterator<std::string> file_end;

    size_t sum = 0;
    for (auto it = file_start; it != file_end; ++it) {
        for (std::sregex_iterator ri(it->cbegin(), it->cend(), pattern); ri != std::sregex_iterator{}; ++ri) {
            sum += std::stoi((*ri)[1]) * std::stoi((*ri)[2]);
        }
    }
    return sum;
}


enum class Operator {
    Enable,
    Disable,
    Multiply
};

size_t part_2(std::ifstream& input) {
    std::regex instructions_pattern(R"((do\(\)|don't\(\)|mul\((\d{1,3}),(\d{1,3})\)))");

    // now that's a proper indian-style code
    auto get_operator = [&](std::ssub_match sm) {
        if (*sm.first == 'm') return Operator::Multiply;
        return sm.length() > 4 ? Operator::Disable : Operator::Enable;
    };

    std::istream_iterator<std::string> file_start(input);
    std::istream_iterator<std::string> file_end;

    size_t sum = 0;
    bool do_apply = true;
    for (auto it = file_start; it != file_end; ++it) {
        for (std::sregex_iterator ri(it->cbegin(), it->cend(), instructions_pattern); ri != std::sregex_iterator{}; ++ri) {
            auto op = get_operator((*ri)[0]);
            if (op == Operator::Multiply) {
                if (do_apply) sum += std::stoi((*ri)[2]) * std::stoi((*ri)[3]);
            } else {
                do_apply = (op == Operator::Enable);
            }
        }
    }

    return sum;
}


int main(int argc, char* argv[]) {
    std::ifstream input(argv[1]);
    if (!input) {
        std::cout << "Input file not found" << '\n';
        return 1;
    }

    std::cout << "Part 1: " << part_1(input) << '\n';
    input.clear();
    input.seekg(0);
    std::cout << "Part 2: " << part_2(input) << '\n';

    return 0;
}
