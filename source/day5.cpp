#include <array>
#include <deque>
#include <fmt/format.h>
#include <fstream>
#include <optional>
#include <stack>
#include <string>

auto consume_until_space(std::string_view& string) -> std::string_view {
    const auto location = string.find_first_of(' ');
    const auto consumed = string.substr(0, location);
    if (location != std::string_view::npos) {
        string.remove_prefix(location + 1);
    }

    return consumed;
}

auto read_move_instruction(const std::string_view move_instruction) {
    auto string = move_instruction;
    auto tokens = std::array<std::string_view, 6>{};
    for (auto& token: tokens) {
        token = consume_until_space(string);
    }

    //! "move {} from {} to {}"
    const auto instructions = std::make_tuple(
            std::strtol(std::string(tokens.at(1)).c_str(), nullptr, 10),
            std::strtol(std::string(tokens.at(3)).c_str(), nullptr, 10),
            std::strtol(std::string(tokens.at(5)).c_str(), nullptr, 10)
    );

    if (errno == EINVAL) {
        fmt::print(stderr, "could not read move instruction {}\n", move_instruction);
        std::exit(1);
    }

    return instructions;
}

struct crate_t {
    char id;
    constexpr static auto crate_string_size = 3uz;

    [[nodiscard]]
    static auto from_string(const std::string_view crate_string) -> std::optional<crate_t> {
        if (crate_string.length() == crate_string_size
            && crate_string.at(0) == '['
            && std::isalpha(crate_string.at(1))
            && crate_string.at(2) == ']') {
            return crate_t{crate_string.at(1)};
        } else {
            return std::nullopt;
        }
    }
};

auto main() -> int {
    auto input = std::ifstream{"source/day5.txt"};
    auto crates1 = std::array<std::deque<crate_t>, 9>{};
    auto crates2 = std::array<std::deque<crate_t>, 9>{};
    auto result1 = std::string{};
    auto result2 = std::string{};

    //! Read the crates
    for (std::string next_line;
         std::getline(input, next_line) &&
         next_line.contains("[");) {
        const auto next_row = std::string_view(next_line);
        for (auto i = 0, k = 0; i < next_line.length(); i += crate_t::crate_string_size + 1, ++k) {
            const auto crate = crate_t::from_string(next_row.substr(i, crate_t::crate_string_size));
            if (crate.has_value()) {
                crates1.at(k).push_front(crate.value());
                crates2.at(k).push_front(crate.value());
            }
        }
    }

    //! Rearrange the crates
    for (std::string next_line;
         std::getline(input, next_line);) {
        if (!next_line.starts_with("move ")) {
            continue;
        }

        const auto [
                number_to_move,
                source_of_move,
                target_of_move
        ] = read_move_instruction(next_line);

        //! Part 1
        auto& source1 = crates1.at(source_of_move - 1);
        auto& target1 = crates1.at(target_of_move - 1);
        for (auto i = 0; i < number_to_move; ++i) {
            target1.push_back(source1.back());
            source1.pop_back();
        }

        //! Part 2
        auto stack = std::stack<crate_t>{};
        auto& source2 = crates2.at(source_of_move - 1);
        auto& target2 = crates2.at(target_of_move - 1);
        for (auto i = 0; i < number_to_move; ++i) {
            stack.push(source2.back());
            source2.pop_back();
        }

        while (!stack.empty()) {
            target2.push_back(stack.top());
            stack.pop();
        }
    }

    //! Read the top-level row of crates.
    result1.reserve(crates1.size());
    result2.reserve(crates2.size());
    for (auto& crate: crates1) {
        const auto crate_contents = crate.back().id;
        result1 += crate_contents;
    }

    for (auto& crate: crates2) {
        const auto crate_contents = crate.back().id;
        result2 += crate_contents;
    }

    fmt::print("results = {{{}, {}}}\n", result1, result2);
}
