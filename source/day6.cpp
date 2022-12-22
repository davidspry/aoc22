#include <array>
#include <cstddef>
#include <cstdint>
#include <fmt/format.h>
#include <fstream>
#include <optional>

auto find_marker(const std::string_view input_text,
                 const std::size_t required_marker_length) -> std::optional<std::size_t> {
    auto previous_position = std::array<std::size_t, 26>{};
    auto number_of_consecutive_unique_characters = std::size_t{0};

    for (auto i = 0uz; i < input_text.length(); ++i) {
        const auto character = input_text.at(i);
        const auto character_index = static_cast<std::size_t>(character - 'a');

        const auto position = i + 1;
        const auto unique_length = position - previous_position.at(character_index);
        const auto marker_length = std::min(position, required_marker_length);
        const auto is_unique_within_marker_length = unique_length >= marker_length;

        if (is_unique_within_marker_length) {
            number_of_consecutive_unique_characters += 1;
        } else {
            number_of_consecutive_unique_characters = std::min(unique_length, number_of_consecutive_unique_characters);
        }

        if (number_of_consecutive_unique_characters == required_marker_length) {
            return position;
        }

        previous_position.at(character_index) = position;
    }

    return std::nullopt;
}

auto main() -> int {
    auto input = std::ifstream{"source/day6.txt"};
    constexpr static auto marker_length1 = 4;
    constexpr static auto marker_length2 = 14;

    std::string next_line;
    std::getline(input, next_line);
    if (next_line.length() < marker_length2) {
        fmt::print(stderr, "invalid input data.\n");
        std::exit(1);
    }

    const auto input_data = std::string_view(next_line);
    const auto result1 = find_marker(input_data, marker_length1);
    const auto result2 = find_marker(input_data.substr(result1.value_or(0)), marker_length2);

    if (result1.has_value() && result2.has_value()) {
        fmt::print("results = {{{}, {}}}\n",
                   result1.value(),
                   result1.value() + result2.value());
    }
}
