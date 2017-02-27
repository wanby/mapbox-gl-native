#include <mbgl/map/backend.hpp>
#include <mbgl/gl/context.hpp>

#include <cassert>

namespace mbgl {

Backend::Backend() = default;

Backend::~Backend() = default;

gl::Context& Backend::getContext() {
    std::call_once(initialized, [this] {
        context = std::make_unique<gl::Context>();
        using namespace std::placeholders;
        context->initializeExtensions(std::bind(&Backend::initializeExtension, this, _1));
        context->enableDebugging();
    });
    return *context;
}

void Backend::notifyMapChange(MapChange) {
    // no-op
}

} // namespace mbgl
