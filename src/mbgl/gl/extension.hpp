#pragma once

#include <initializer_list>
#include <utility>

namespace mbgl {
namespace gl {

template <class>
class ExtensionFunction;

template <class R, class... Args>
class ExtensionFunction<R(Args...)> {
public:
    using glProc = void (*)();
    ExtensionFunction(const glProc ptr_) : ptr(ptr_) {
    }

    explicit operator bool() const {
        return ptr;
    }

    R operator()(Args... args) const {
        return (*reinterpret_cast<R (*)(Args...)>(ptr))(std::forward<Args>(args)...);
    }

private:
    const glProc ptr;
};

} // namespace gl
} // namespace mbgl
