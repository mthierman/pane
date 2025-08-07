#pragma once
#include <pane/window.hxx>
#include <pane/webview.hxx>
#include <vector>
#include <utility>

namespace pane {
template <typename T> struct window_manager final {
    using Self = window_manager;

    // window_manager(struct window_config window_config) { this->create(window_config); }

    // window_manager(struct window_config window_config, struct webview_config webview_config) {
    //     this->create(window_config, webview_config);
    // }

    // template <typename... Args> window_manager(Args... args) {
    //     this->create(std::forward<Args>(args)...);
    // }
    window_manager() { this->create(); }
    ~window_manager() = default;

    window_manager(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    window_manager(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    template <typename... Args> auto create(this Self& self, Args... args) -> void {
        self.windows.push_back(std::make_unique<T>());
    }

    // template <typename U> auto destroy(this Self& self, U hwnd) -> void {
    //     std::erase_if(self.windows, [&](const auto& window) {
    //         return window && window->window_handle() == reinterpret_cast<HWND>(hwnd);
    //     });

    //     if (self.windows.empty()) {
    //         pane::system::quit();
    //     }
    // }

private:
    std::vector<std::unique_ptr<T>> windows;
};
} // namespace pane
