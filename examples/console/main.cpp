#include <Windows.h>
#include <cstdlib>
#include <print>
#include <ranges>
#include <vector>
#include <string>

namespace std {
template <> struct formatter<std::u8string_view> : formatter<string_view> {
    auto format(std::u8string_view string, format_context& context) const noexcept {
        return formatter<string_view>::format(
            std::string_view { reinterpret_cast<const char*>(string.data()), string.length() },
            context);
    }
};

template <size_t size> struct formatter<char8_t[size]> : formatter<u8string_view> {
    auto format(const char8_t* string, format_context& context) const noexcept {
        return formatter<u8string_view>::format(std::u8string_view { string, (size - 1) }, context);
    }
};

template <> struct formatter<const char8_t*> : formatter<u8string_view> {
    auto format(const char8_t* string, format_context& context) const noexcept {
        return formatter<u8string_view>::format(std::u8string_view { string }, context);
    }
};

template <> struct formatter<std::u8string> : formatter<u8string_view> {
    auto format(const std::u8string& string, format_context& context) const noexcept {
        return formatter<u8string_view>::format(
            std::u8string_view { string.data(), string.length() }, context);
    }
};

template <> struct formatter<std::u16string_view, wchar_t> : formatter<wstring_view, wchar_t> {
    auto format(std::u16string_view string, wformat_context& context) const {
        return formatter<wstring_view, wchar_t>::format(
            std::wstring_view { reinterpret_cast<const wchar_t*>(string.data()), string.length() },
            context);
    }
};

template <size_t size>
struct formatter<char16_t[size], wchar_t> : formatter<u16string_view, wchar_t> {
    auto format(const char16_t* string, wformat_context& context) const {
        return formatter<u16string_view, wchar_t>::format(
            std::u16string_view { string, (size - 1) }, context);
    }
};

template <> struct formatter<const char16_t*, wchar_t> : formatter<u16string_view, wchar_t> {
    auto format(const char16_t* string, wformat_context& context) const noexcept {
        return formatter<u16string_view, wchar_t>::format(std::u16string_view { string }, context);
    }
};

template <> struct formatter<std::u16string, wchar_t> : formatter<u16string_view, wchar_t> {
    auto format(const std::u16string& string, wformat_context& context) const {
        return formatter<u16string_view, wchar_t>::format(
            std::u16string_view { string.data(), string.length() }, context);
    }
};
} // namespace std

auto to_utf8_lossy(std::wstring_view string) -> std::u8string {
    auto length { WideCharToMultiByte(
        CP_UTF8, 0, string.data(), string.size(), nullptr, 0, nullptr, nullptr) };

    std::u8string buffer;
    buffer.resize(length);

    WideCharToMultiByte(CP_UTF8,
                        0,
                        string.data(),
                        string.size(),
                        reinterpret_cast<char*>(buffer.data()),
                        length,
                        nullptr,
                        nullptr);

    return buffer;
}

// https://learn.microsoft.com/en-us/cpp/c-language/using-wmain
auto wmain(int argc, wchar_t** argv, wchar_t** /* envp */) -> int {
    // auto args { pane::system::get_argv_u8(argc, argv) };

    std::vector<std::u8string> args;
    args.reserve(argc);

    for (auto warg : std::span(argv, argc)) {
        args.emplace_back(to_utf8_lossy(warg));
    }

    for (auto [idx, arg] : std::views::enumerate(args)) {
        std::println("{}: {}", idx, arg);
    }

    return EXIT_SUCCESS;
}
