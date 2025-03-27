#include <pane/guid.hxx>
#include <pane/system.hxx>

namespace pane {
auto guid::create() -> std::expected<Self, std::error_code> {
    GUID guid;

    auto result { CoCreateGuid(&guid) };

    if (result != S_OK) {
        return std::unexpected(hresult_error(result));
    }

    Self self;
    self.storage = std::move(guid);

    // if (auto errorCode { ::CoCreateGuid(&guid) }; errorCode == S_OK) {
    //     return guid;
    // } else {
    //     return std::unexpected(glow::system::format_message(errorCode));
    // }
}
} // namespace pane
