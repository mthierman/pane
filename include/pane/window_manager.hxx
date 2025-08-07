#pragma once
#include <pane/window.hxx>
#include <pane/webview.hxx>
#include <vector>
#include <utility>

namespace pane {
template <typename T> struct window_manager final {
    using Self = window_manager;

    template <typename... Args> window_manager(Args... args) {
        // this->windows.push_back(std::make_unique<T>(std::forward<Args>(args)...));
        this->create(std::forward<Args>(args)...);
    }
    ~window_manager() = default;

    window_manager(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    window_manager(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    template <typename... Args> auto create(this Self& self, Args... args) -> void {
        self.windows.push_back(std::make_unique<T>(std::forward<Args>(args)...));
        // if constexpr (std::is_same_v<T, pane::window>) {
        //     self.windows.push_back(std::make_unique<T>(self.window_config,
        //     self.window_procedure));
        // }
        // if constexpr (std::is_same_v<T, pane::webview>) {
        //     self.windows.push_back(std::make_unique<T>(
        //         self.window_config, self.webview_config, self.window_procedure));
        // }
    }

    // auto create(this Self& self) -> void {
    //     if constexpr (std::is_same_v<T, pane::window>) {
    //         self.windows.push_back(std::make_unique<T>(self.window_config,
    //         self.window_procedure));
    //     }
    //     if constexpr (std::is_same_v<T, pane::webview>) {
    //         self.windows.push_back(std::make_unique<T>(
    //             self.window_config, self.webview_config, self.window_procedure));
    //     }
    // }

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
    // window_config window_config;
    // webview_config webview_config;
};
} // namespace pane
