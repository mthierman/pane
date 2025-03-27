#include <pane/pane.hxx>
#include <print>
#include <format>

auto main() -> int {
    //
    auto u8string { std::u8string(u8"utf8") };
    auto u16string { std::u16string(u"utf16") };

    auto u8fmt { std::format("{}", u8string) };
    auto u16fmt { std::format(L"{}", u16string) };

    // std::println("{}", u8string);
    // std::println(L"{}", u16string);

    return 0;
}
