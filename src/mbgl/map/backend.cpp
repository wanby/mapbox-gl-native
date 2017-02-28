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

PremultipliedImage Backend::readFramebuffer(const Size& size) const {
    return context->readFramebuffer<PremultipliedImage>(size);
}

void Backend::invalidateFramebufferBinding() {
    context->bindFramebuffer.setDirty();
}

void Backend::updateFramebufferBinding(const uint32_t fbo, const UpdateType update) {
    if (update == UpdateType::Force) {
        context->bindFramebuffer = fbo;
    } else {
        context->bindFramebuffer.setCurrentValue(fbo);
    }
    assert(gl::value::BindFramebuffer::Get() == context->bindFramebuffer.getCurrentValue());
}

uint32_t Backend::getFramebufferBinding() {
    return context->bindFramebuffer.getCurrentValue();
}

void Backend::updateViewportSize(const Size& size, const UpdateType update) {
    if (update == UpdateType::Force) {
        context->viewport = { 0, 0, size };
    } else {
        context->viewport.setCurrentValue({ 0, 0, size });
    }
    assert(gl::value::Viewport::Get() == context->viewport.getCurrentValue());
}

} // namespace mbgl
