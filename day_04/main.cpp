#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <array>

using Grid = std::vector<std::string>;

const std::string XMAS("XMAS");
const std::string MAS("MAS");


struct Point {
    int x;
    int y;

    Point& operator+=(Point& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Point& operator-=(Point& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Point operator-() const {
        return {-x, -y};
    }
};


const Point TOP_LEFT(-1, -1);
const Point TOP_RIGHT(1, -1);

const std::array<Point, 8> DIRECTIONS{{{-1,-1},  // top-left
                                       {0, -1},  // top
                                       {1, -1},  // top-right
                                       {1,  0},  // right
                                       {1,  1},  // bottom-right
                                       {0,  1},  // bottom
                                       {-1, 1},  // bottom-left
                                       {-1, 0}}};// left

bool out_of_bounds(const Grid& grid, Point p) {
    return p.x < 0 || p.y < 0 || p.y >= grid.size() || p.x >= grid[p.y].size();
}

// grid contains word start from start in the specified direction
bool word_starts(const Grid& grid, const std::string& word, Point start, Point direction) {
    for (int i = 1; i < word.size(); ++i) {
        start += direction;
        if (out_of_bounds(grid, start) || grid[start.y][start.x] != word[i]) {
            return false;
        }
    }
    return true;
}

bool word_mid_point(const Grid& grid, const std::string& word, Point mid_point, Point direction) {
    int mid_index = word.size() / 2;
    Point left = mid_point;
    Point right = mid_point;
    for (int i = 1; i <= mid_index; ++i) {
        left += direction;
        right -= direction;
        if (out_of_bounds(grid, left) || out_of_bounds(grid, right) ||
            grid[left.y][left.x] != word[mid_index-i] || grid[right.y][right.x] != word[mid_index+i]) {
            return false;
        }
    }
    return true;
}


size_t part_1(const Grid& grid, const std::string& word) {
    size_t num_words = 0;
    for (int y = 0; y < grid.size(); ++y) {
        for (int x = 0; x < grid[y].size(); ++x) {
            if (grid[y][x] != word[0]) {
                continue;
            }
            // check all directions
            Point start(x, y);
            for (auto direction : DIRECTIONS) {
                if (word_starts(grid, word, start, direction)) {
                    ++num_words;
                }
            }
        }
    }
    return num_words;
}

size_t part_2(const Grid& grid, const std::string& word) {
    size_t num_crosses = 0;
    // assume word length is odd
    int mid = word.size() / 2;
    for (int y = mid; y < grid.size() - mid; ++y) {
        for (int x = mid; x < grid[y].size() - mid; ++x) {
            if (grid[y][x] != word[mid]) {
                continue;
            }
            Point mid_point(x, y);
            if ((word_mid_point(grid, word, mid_point, TOP_LEFT) || word_mid_point(grid, word, mid_point, -TOP_LEFT)) &&
                (word_mid_point(grid, word, mid_point, TOP_RIGHT) || word_mid_point(grid, word, mid_point, -TOP_RIGHT))) {
                ++num_crosses;
            }
        }
    }
    return num_crosses;
}

int main(int argc, char* argv[]) {
    std::fstream input(argv[1]);
    if (!input) {
        std::cout << "Input file not found";
        return 1;
    }

    std::string s;
    Grid grid;
    while (getline(input, s)) {
        grid.push_back(s);
    }

    std::cout << "Part 1: " << part_1(grid, XMAS) << '\n';
    std::cout << "Part 2: " << part_2(grid, MAS) << '\n';

    return 0;
}
