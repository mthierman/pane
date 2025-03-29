#include <pane/guid.hxx>

namespace pane {
auto guid() -> std::optional<GUID> {
    GUID guid;

    auto result { CoCreateGuid(&guid) };

    if (result != S_OK) {
        return std::nullopt;
    }

    return guid;
}
} // namespace pane::guid
