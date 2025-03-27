#include <pane/pane.hxx>
#include <glaze/glaze.hpp>
#include <map>
#include <string>
#include <array>
#include <iostream>
#include <cstdlib>

struct my_struct {
    int i = 287;
    std::u8string hello = u8"Hello World";
    std::array<uint64_t, 3> arr = { 1, 2, 3 };
    std::map<std::string, int> map { { "one", 1 }, { "two", 2 } };
};

auto main() -> int {
    my_struct obj {};
    std::cout << glz::write_json(obj).value_or("error") << '\n';

    return EXIT_SUCCESS;
}
