#include <mbgl/map/backend.hpp>
#include <mbgl/gl/context.hpp>

#include <cassert>

namespace mbgl {

Backend::Backend() : context(std::make_unique<gl::Context>()) {
}

gl::Context& Backend::getContext() {
    return *context;
}

void Backend::initializeExtensions(glProc (*getProcAddress)(const char*)) {
    if (!extensionsInitialized) {
        context->initializeExtensions(getProcAddress);
    }
}

Backend::~Backend() = default;

void Backend::notifyMapChange(MapChange) {
    // no-op
}


} // namespace mbgl
