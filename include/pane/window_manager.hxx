#pragma once
#include <pane/window.hxx>
#include <pane/webview.hxx>
#include <vector>

namespace pane {
template <typename T> struct window_manager final {
    using Self = window_manager;

    window_manager(struct window_config window_config = {},
                   window::procedure_fn window_procedure = {})
        : window_config { window_config },
          window_procedure { window_procedure } {
        this->add(this->window_config, this->window_procedure);
    }
    window_manager(struct window_config window_config = {},
                   struct webview_config webview_config = {},
                   webview::procedure_fn window_procedure = {})
        : window_config { window_config },
          webview_config { webview_config },
          window_procedure { window_procedure } {
        this->add(this->window_config, this->webview_config, this->window_procedure);
    }
    ~window_manager() = default;

    window_manager(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    window_manager(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    template <typename... Args> auto add(this Self& self, Args&&... args) -> void {
        self.windows.push_back(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
    }

    template <typename U> auto remove(this Self& self, U hwnd) -> void {
        std::erase_if(self.windows, [&](const auto& window) {
            return window && window->window_handle() == reinterpret_cast<HWND>(hwnd);
        });

        if (self.windows.empty()) {
            pane::system::quit();
        }
    }

    std::vector<std::unique_ptr<T>> windows;
    window_config window_config;
    webview_config webview_config;
    T::procedure_fn window_procedure;
};
} // namespace pane
