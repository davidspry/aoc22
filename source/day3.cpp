#include <array>
#include <cstddef>
#include <cstdint>
#include <fmt/format.h>
#include <fstream>
#include <optional>
#include <string_view>

using membership_array_t = std::array<bool, 2 * 26>;

auto char_to_index(const unsigned char character) -> std::optional<std::size_t> {
    if ('a' <= character && character <= 'z') {
        return static_cast<std::uint32_t>(character - 'a');
    } else if ('A' <= character && character <= 'Z') {
        return static_cast<std::uint32_t>(character - 'A') + 26u;
    } else {
        return std::nullopt;
    }
}

class group_t {
    std::array<std::string, 3> rucksacks{};
    std::size_t rucksack_index{0};

    [[nodiscard]]
    auto is_full() const {
        return rucksack_index == rucksacks.size();
    }

public:
    auto add_rucksack(const std::string_view rucksack) {
        rucksacks.at(rucksack_index) = std::string(rucksack);
        rucksack_index += 1;
        return !is_full();
    }

    auto reset() {
        rucksack_index = 0;
    }

    [[nodiscard]]
    auto infer_badge_value() const -> std::optional<char> {
        if (!is_full()) {
            return std::nullopt;
        }

        auto membership1 = membership_array_t{};
        auto membership2 = membership_array_t{};
        for (const auto item_code: rucksacks.at(0)) {
            if (const auto index = char_to_index(item_code); index.has_value()) {
                membership1.at(index.value()) = true;
            } else {
                fmt::print(stderr, "unexpected input character {}\n", item_code);
                std::exit(1);
            }
        }

        for (const auto item_code: rucksacks.at(1)) {
            if (const auto index = char_to_index(item_code); index.has_value()) {
                membership2.at(index.value()) = membership1.at(index.value());
            } else {
                fmt::print(stderr, "unexpected input character {}\n", item_code);
                std::exit(1);
            }
        }

        for (const auto item_code: rucksacks.at(2)) {
            if (const auto index = char_to_index(item_code); index.has_value()) {
                if (membership2.at(index.value())) {
                    return item_code;
                }
            } else {
                fmt::print(stderr, "unexpected input character {}\n", item_code);
                std::exit(1);
            }
        }

        return std::nullopt;
    }
};

auto main() -> int {
    auto input = std::ifstream{"source/day3.txt"};
    auto group = group_t{};
    auto result1 = std::uint32_t{0};
    auto result2 = std::uint32_t{0};

    for (std::string next_line;
         std::getline(input, next_line);) {
        //! Part One
        if (next_line.length() % 2 != 0) {
            fmt::print(stderr, "input is not divisible by 2.\n");
            std::exit(1);
        }

        const auto compartment_size = next_line.length() / 2uz;
        const auto compartment1 = std::string_view(next_line.data(), compartment_size);
        const auto compartment2 = std::string_view(next_line.data() + compartment_size, compartment_size);
        auto membership = membership_array_t{};

        for (const auto item_code: compartment1) {
            if (const auto index = char_to_index(item_code); index.has_value()) {
                membership.at(index.value()) = true;
            } else {
                fmt::print(stderr, "unexpected input character {}\n", item_code);
                std::exit(1);
            }
        }

        for (const auto item_code: compartment2) {
            if (const auto index = char_to_index(item_code); index.has_value()) {
                if (membership.at(index.value())) {
                    result1 += index.value() + 1u;
                    break;
                }
            } else {
                fmt::print(stderr, "unexpected input character {}\n", item_code);
                std::exit(1);
            }
        }

        //! Part Two
        group.add_rucksack(next_line);
        if (const auto badge_value = group.infer_badge_value(); badge_value.has_value()) {
            const auto badge_value_priority = char_to_index(badge_value.value()).value() + 1u;
            result2 += badge_value_priority;
            group.reset();
        }
    }

    fmt::print("results = {{{}, {}}}\n", result1, result2);
}
