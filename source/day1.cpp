#include <cstdint>
#include <cstdlib>
#include <fmt/format.h>
#include <fstream>
#include <numeric>
#include <string>

struct result_t {
    constexpr static auto buffer_size = 4;
    std::uint64_t sum;
    std::uint64_t max[buffer_size];

    auto finish_current_list() {
        max[buffer_size - 1] = sum;
        std::sort(std::begin(max), std::end(max), std::greater{});
        sum = 0;
    }
};

auto main() -> int {
    auto input = std::ifstream{"source/day1.txt"};
    auto result = result_t{};

    for (std::string next_line;
         std::getline(input, next_line).good();) {
        if (next_line.starts_with('\n') or
            next_line.empty()) {
            result.finish_current_list();
            continue;
        }

        if (const auto value = std::strtol(next_line.c_str(), nullptr, 10); value > 0) {
            result.sum += static_cast<std::uint64_t>(value);
        } else {
            fmt::print("value \"{}\" could not be read as an integer\n", value);
        }
    }

    fmt::print("results = {{{}, {}}}\n",
               result.max[0],
               std::accumulate(std::begin(result.max),
                               std::begin(result.max) + 3,
                               std::uint64_t{}));
}
