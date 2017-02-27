#include <mbgl/gl/debugging.hpp>
#include <mbgl/gl/context.hpp>
#include <mbgl/gl/debugging_extension.hpp>

namespace mbgl {
namespace gl {

#ifndef NDEBUG

DebugGroup::DebugGroup(const Context& context_, const std::string& name) : context(context_) {
    if (context.debugging) {
        if (context.debugging->pushDebugGroup) {
            context.debugging->pushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, GLsizei(name.size()), name.c_str());
        } else if (context.debugging->pushGroupMarkerEXT) {
            context.debugging->pushGroupMarkerEXT(GLsizei(name.size() + 1), name.c_str());
        }
    }
}

DebugGroup::~DebugGroup() {
    if (context.debugging) {
        if (context.debugging->popDebugGroup) {
            context.debugging->popDebugGroup();
        } else if (context.debugging->popGroupMarkerEXT) {
            context.debugging->popGroupMarkerEXT();
        }
    }
}

#endif

} // namespace gl
} // namespace mbgl
