#pragma once

#include <mbgl/map/change.hpp>

#include <memory>
#include <mutex>

namespace mbgl {

namespace gl {
class Context;
using ProcAddress = void (*)();
} // namespace gl

class BackendScope;

class Backend {
public:
    Backend();
    virtual ~Backend();

    // Returns the backend's context which manages OpenGL state.
    gl::Context& getContext();

    // Called when the map needs to be rendered; the backend should call Map::render() at some point
    // in the near future. (Not called for Map::renderStill() mode.)
    virtual void invalidate() = 0;

    // Notifies a watcher of map x/y/scale/rotation changes.
    virtual void notifyMapChange(MapChange change);

protected:
    // Called with the name of an OpenGL extension that should be loaded. Backend implementations
    // must call the API-specific version that obtains the function pointer for this function,
    // or a null pointer if unsupported/unavailable.
    virtual gl::ProcAddress initializeExtension(const char*) = 0;

    // Called when the backend's GL context needs to be made active or inactive. These are called,
    // as a matched pair, in four situations:
    //
    //   1. When releasing GL resources during Map destruction
    //   2. When calling a CustomLayerInitializeFunction, during Map::addLayer
    //   3. When calling a CustomLayerDeinitializeFunction, during Map::removeLayer
    //   4. When rendering for Map::renderStill
    //
    // They are *not* called for Map::render; it is assumed that the correct context is already
    // activated prior to calling Map::render.
    virtual void activate() = 0;
    virtual void deactivate() = 0;

protected:
    std::unique_ptr<gl::Context> context;

private:
    std::once_flag initialized;

    friend class BackendScope;
};

} // namespace mbgl
