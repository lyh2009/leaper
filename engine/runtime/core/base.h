#pragma once

#include <memory>

#define LP_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

// ref
namespace Leaper
{
    template <typename T> using Scope = std::unique_ptr<T>;
    template <typename T, typename... Args> constexpr Scope<T> CreateScope(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template <typename T> using Ref = std::shared_ptr<T>;
    template <typename T, typename... Args> constexpr Ref<T> CreateRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}  // namespace Leaper

// assert
#include <assert.h>
#include <iostream>


#define BIT(x) (1 << x)

#ifndef NOT_DEBUG
#define LP_ASSERT(x, y) assert(x&& y)
#define LP_ASSERT_RETURN(x, y, z)    \
    if (!x)                          \
    {                                \
        std::cout << y << std::endl; \
        return z;                    \
    }
#endif