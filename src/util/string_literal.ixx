export module jnikot.util.string_literal;

import jnikot.util.std;

namespace jnikot {
    export template<size_t N>
    class string_literal {
    public:
        using array_type = char[N];

        array_type str;

        consteval string_literal(char const (&s)[N]) : str{} {
            std::copy_n(s, N, str);
        }

        consteval string_literal() = default;

        consteval string_literal(string_literal const &) = default;

        consteval explicit string_literal(char c) : str{} {
            str[0] = c;
            str[1] = '\0';
        }

    public:
        template<char to_replace, char replace_target>
        consteval auto replace_with() const {
            using result_type = string_literal<sizeof(str)>;
            result_type result{};
            std::copy_n(str, sizeof(str), result.str);
            std::replace(result.str, result.str + sizeof(str), to_replace, replace_target);
            return result;
        }
    };

    string_literal(char c) -> string_literal<2>;

    export template<size_t s1, size_t s2>
    consteval bool operator==(const string_literal<s1> &lhs, const string_literal<s2> &rhs) {
        if constexpr (s1 != s2) {
            return false;
        } else {
            return std::equal(lhs.str, lhs.str + s1, rhs.str);
        }
    }

    export template<size_t s1, size_t s2>
    consteval auto operator+(const string_literal<s1> &lhs, const string_literal<s2> &rhs) {
        using result_type = string_literal<s1 + s2 - 1>;
        result_type result{};
        std::copy_n(lhs.str, s1 - 1, result.str);
        std::copy_n(rhs.str, s2, result.str + s1 - 1);
        return result;
    }

    export template<size_t s>
    consteval auto operator+(char c, const string_literal<s> &rhs) {
        return string_literal{c} + rhs;
    }

    export template<size_t s>
    consteval auto operator+(const string_literal<s> &lhs, char c) {
        return lhs + string_literal{c};
    }

    export template<string_literal sl>
    constexpr decltype(auto) operator""_sl() {
        return sl;
    }
}

export using jnikot::operator+;
export using jnikot::operator==;
export using jnikot::operator""_sl;

static_assert("Hello"_sl == "Hello"_sl);
static_assert("Hello"_sl + " World"_sl == "Hello World"_sl);

static_assert("com/java/String"_sl.replace_with<'/', '.'>() == "com.java.String"_sl);
static_assert("com/java/String"_sl + ';' == "com/java/String;"_sl);
static_assert('L' + "com/java/String"_sl == "Lcom/java/String"_sl);
