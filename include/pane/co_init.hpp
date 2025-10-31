#pragma once
#include <Windows.h>
#include <objbase.h>

namespace pane {
struct co_init final {
    using Self = co_init;

    ~co_init();

    co_init(const Self&) = delete;
    auto operator=(const Self&) -> Self& = delete;

    co_init(Self&&) noexcept = delete;
    auto operator=(Self&&) noexcept -> Self& = delete;

    operator HRESULT(this Self& self);

    static auto apartment_threaded() -> co_init;
    static auto multi_threaded() -> co_init;

    HRESULT result;

private:
    co_init(DWORD concurrency_model);
};
} // namespace pane
