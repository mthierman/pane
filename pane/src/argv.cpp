#include <pane/argv.hpp>

namespace pane {
auto argv::u8string() -> std::vector<std::u8string> {
    std::vector<std::u8string> args;
    args.reserve(this->argv.size());

    for (auto& arg : argv) {
        args.emplace_back(pane::to_utf8_lossy(arg));
    }

    return args;
}
} // namespace pane
