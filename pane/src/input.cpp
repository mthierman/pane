#include <pane/input.hpp>

namespace pane::input {
auto is_key_down(int virtual_key) -> bool { return GetKeyState(virtual_key) & 0x8000; }
} // namespace pane::input
