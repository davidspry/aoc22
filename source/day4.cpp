#include <cstdint>
#include <cstdlib>
#include <fmt/format.h>
#include <fstream>
#include <optional>
#include <string_view>

class range_t {
    std::uint32_t m_lower_bound;
    std::uint32_t m_upper_bound;

public:
    range_t(const std::uint32_t lower_bound,
            const std::uint32_t upper_bound): m_lower_bound(lower_bound),
                                              m_upper_bound(upper_bound) {}

    static auto from_string(const std::string_view range_string) -> std::optional<range_t> {
        const auto range_delimiter = range_string.find('-');
        if (range_delimiter == std::numeric_limits<std::size_t>::max()) {
            return std::nullopt;
        }

        const auto lower_bound_string = std::string(range_string.substr(0, range_delimiter));
        const auto upper_bound_string = std::string(range_string.substr(range_delimiter + 1));
        const auto lower_bound = std::strtol(lower_bound_string.c_str(), nullptr, 10);
        const auto upper_bound = std::strtol(upper_bound_string.c_str(), nullptr, 10);

        if (errno != EINVAL) {
            return range_t(lower_bound, upper_bound);
        } else {
            return std::nullopt;
        }
    }

    [[nodiscard]]
    auto contains(const range_t& other_range) const {
        return m_lower_bound <= other_range.m_lower_bound &&
               m_upper_bound >= other_range.m_upper_bound;
    }

    [[nodiscard]]
    auto overlaps(const range_t& other_range) const {
        return m_lower_bound <= other_range.m_upper_bound &&
               m_upper_bound >= other_range.m_lower_bound;
    }
};

auto main() -> int {
    auto input = std::ifstream{"source/day4.txt"};
    auto result1 = std::uint32_t{0};
    auto result2 = std::uint32_t{0};

    for (std::string next_line;
         std::getline(input, next_line);) {
        const auto range_separator = next_line.find(',');
        if (range_separator == std::numeric_limits<std::size_t>::max()) {
            fmt::print(stderr, "invalid input {}\n", next_line);
            std::exit(1);
        }

        const auto range1 = range_t::from_string(std::string_view(next_line).substr(0, range_separator));
        const auto range2 = range_t::from_string(std::string_view(next_line).substr(range_separator + 1));

        if (range1.has_value() && range2.has_value()) {
            if (range1.value().contains(range2.value()) ||
                range2.value().contains(range1.value())) {
                result1 += 1;
            } else if (range1.value().overlaps(range2.value()) ||
                       range2.value().overlaps(range1.value())) {
                result2 += 1;
            }
        } else {
            fmt::print(stderr, "could not create range from input {}\n", next_line);
            std::exit(1);
        }
    }

    result2 += result1; //! Contains implies overlap

    fmt::print("results = {{{}, {}}}\n", result1, result2);
}
