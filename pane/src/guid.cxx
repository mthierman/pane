#include <pane/guid.hxx>
#include <pane/system.hxx>
#include <pane/hstring.hxx>
#include <wil/win32_helpers.h>

namespace pane {
auto guid::create() -> std::expected<Self, std::error_code> {
    Self self;

    auto result { CoCreateGuid(&self.storage) };

    if (result != S_OK) {
        return std::unexpected(hresult_error(result));
    }

    return self;
}

auto guid::to_string(this Self& self) -> std::optional<string> {
    std::wstring buffer;
    buffer.resize(wil::guid_string_buffer_length);

    if (StringFromGUID2(self.storage, buffer.data(), wil::guid_string_buffer_length) == 0) {
        return std::nullopt;
    }

    auto converted_guid { string::from_utf16(buffer) };

    if (!converted_guid) {
        return std::nullopt;
    }

    return converted_guid.value();
}
} // namespace pane
