#include <cstdint>
#include <fmt/format.h>
#include <fstream>
#include <optional>

struct outcome_t {
    enum class type { win, lose, draw };

    static auto from_char(const char input_char) -> std::optional<type> {
        switch (input_char) {
            case 'X':
                return type::lose;
            case 'Y':
                return type::draw;
            case 'Z':
                return type::win;
            default:
                return std::nullopt;
        }
    }
};

struct shape_t {
    enum class type: std::uint32_t { rock, paper, scissors };

    static auto from_char(const char input_char) -> std::optional<type> {
        switch (input_char) {
            case 'A':
            case 'X':
                return type::rock;
            case 'B':
            case 'Y':
                return type::paper;
            case 'C':
            case 'Z':
                return type::scissors;
            default:
                return std::nullopt;
        }
    }
};

auto compute_score(const shape_t::type player1_type,
                   const outcome_t::type outcome) -> std::uint32_t {
    switch (outcome) {
        case outcome_t::type::win:
            return 6u + std::to_underlying(player1_type) + 1;
        case outcome_t::type::draw:
            return 3u + std::to_underlying(player1_type) + 1;
        case outcome_t::type::lose:
            return 0u + std::to_underlying(player1_type) + 1;
    }
}

auto infer_outcome_from_player_types(const shape_t::type player1_type,
                                     const shape_t::type player2_type) -> outcome_t::type {
    const auto p1 = std::to_underlying(player1_type);
    const auto p2 = std::to_underlying(player2_type);
    if ((p1 + 2) % 3 == p2) {
        return outcome_t::type::win;
    } else if (p1 == p2) {
        return outcome_t::type::draw;
    } else {
        return outcome_t::type::lose;
    }
}

auto infer_player1_type_from_outcome(const shape_t::type player2_type,
                                     const outcome_t::type outcome) -> shape_t::type {
    switch (outcome) {
        case outcome_t::type::win:
            return static_cast<shape_t::type>((std::to_underlying(player2_type) + 1) % 3);
        case outcome_t::type::lose:
            return static_cast<shape_t::type>((std::to_underlying(player2_type) + 2) % 3);
        case outcome_t::type::draw:
            return player2_type;
    }
}

auto main() -> int {
    auto input = std::ifstream{"source/day2.txt"};
    auto result1 = std::uint32_t{0};
    auto result2 = std::uint32_t{0};

    for (std::string next_line;
         std::getline(input, next_line);) {
        if (next_line.length() < 3) {
            fmt::print(stderr, "invalid line '{}' read from input\n", next_line);
            std::exit(1);
        }

        const auto player1 = shape_t::from_char(next_line.at(2));
        const auto player2 = shape_t::from_char(next_line.at(0));
        const auto desired_outcome = outcome_t::from_char(next_line.at(2));

        if (!(player1.has_value() &&
              player2.has_value() &&
              desired_outcome.has_value())) {
            fmt::print(stderr, "invalid line '{}' read from input\n", next_line);
            std::exit(1);
        }

        const auto inferred_outcome = infer_outcome_from_player_types(player1.value(), player2.value());
        const auto inferred_player1 = infer_player1_type_from_outcome(player2.value(), desired_outcome.value());

        result1 += compute_score(player1.value(), inferred_outcome);
        result2 += compute_score(inferred_player1, desired_outcome.value());
    }

    fmt::print("results = {{{}, {}}}\n", result1, result2);
}
