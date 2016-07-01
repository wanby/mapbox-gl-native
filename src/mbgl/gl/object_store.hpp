#pragma once

#include <mbgl/gl/gl.hpp>
#include <mbgl/util/noncopyable.hpp>

#include <unique_resource.hpp>

#include <array>
#include <algorithm>
#include <memory>
#include <vector>

namespace mbgl {
namespace gl {

constexpr GLsizei TextureMax = 64;

class ObjectStore;

struct ProgramDeleter {
    ObjectStore* store;
    void operator()(GLuint) const;
};

struct ShaderDeleter {
    ObjectStore* store;
    void operator()(GLuint) const;
};

struct BufferDeleter {
    ObjectStore* store;
    void operator()(GLuint) const;
};

struct TextureDeleter {
    ObjectStore* store;
    void operator()(GLuint) const;
};

struct VAODeleter {
    ObjectStore* store;
    void operator()(GLuint) const;
};

struct FramebufferDeleter {
    ObjectStore* store;
    void operator()(GLuint) const;
};

struct RenderbufferDeleter {
    ObjectStore* store;
    void operator()(GLuint) const;
};

using ObjectPool = std::array<GLuint, TextureMax>;

struct TexturePoolDeleter {
    ObjectStore* store;
    void operator()(ObjectPool) const;
};

using UniqueProgram = std_experimental::unique_resource<GLuint, ProgramDeleter>;
using UniqueShader = std_experimental::unique_resource<GLuint, ShaderDeleter>;
using UniqueBuffer = std_experimental::unique_resource<GLuint, BufferDeleter>;
using UniqueTexture = std_experimental::unique_resource<GLuint, TextureDeleter>;
using UniqueVAO = std_experimental::unique_resource<GLuint, VAODeleter>;
using UniqueTexturePool = std_experimental::unique_resource<ObjectPool, TexturePoolDeleter>;
using UniqueFramebuffer = std_experimental::unique_resource<GLuint, FramebufferDeleter>;
using UniqueRenderbuffer = std_experimental::unique_resource<GLuint, RenderbufferDeleter>;

class ObjectStore : private util::noncopyable {
public:
    ~ObjectStore();

    UniqueProgram createProgram() {
        return UniqueProgram { MBGL_CHECK_ERROR(glCreateProgram()), { this } };
    }

    UniqueShader createShader(GLenum type) {
        return UniqueShader { MBGL_CHECK_ERROR(glCreateShader(type)), { this } };
    }

    UniqueBuffer createBuffer() {
        GLuint id = 0;
        MBGL_CHECK_ERROR(glGenBuffers(1, &id));
        return UniqueBuffer { std::move(id), { this } };
    }

    UniqueTexture createTexture() {
        GLuint id = 0;
        MBGL_CHECK_ERROR(glGenTextures(1, &id));
        return UniqueTexture { std::move(id), { this } };
    }

    UniqueVAO createVAO() {
        GLuint id = 0;
        MBGL_CHECK_ERROR(gl::GenVertexArrays(1, &id));
        return UniqueVAO { std::move(id), { this } };
    }

    UniqueTexturePool createTexturePool() {
        ObjectPool ids;
        MBGL_CHECK_ERROR(glGenTextures(TextureMax, ids.data()));
        static_assert(ids.size() == size_t(TextureMax), "Texture ids size mismatch");
        return UniqueTexturePool { std::move(ids), { this } };
    }

    UniqueFramebuffer createFramebuffer() {
        GLuint id = 0;
        MBGL_CHECK_ERROR(glGenFramebuffers(1, &id));
        return UniqueFramebuffer { std::move(id), { this } };
    }

    UniqueRenderbuffer createRenderbuffer() {
        GLuint id = 0;
        MBGL_CHECK_ERROR(glGenRenderbuffers(1, &id));
        return UniqueRenderbuffer { std::move(id), { this } };
    }

    // Actually remove the objects we marked as abandoned with the above methods.
    // Only call this while the OpenGL context is exclusive to this thread.
    void performCleanup();

    bool empty() const {
        return abandonedPrograms.empty()
            && abandonedShaders.empty()
            && abandonedBuffers.empty()
            && abandonedTextures.empty()
            && abandonedVAOs.empty()
            && abandonedFramebuffers.empty()
            && abandonedRenderbuffers.empty();
    }

private:
    friend ProgramDeleter;
    friend ShaderDeleter;
    friend BufferDeleter;
    friend TextureDeleter;
    friend VAODeleter;
    friend TexturePoolDeleter;
    friend FramebufferDeleter;
    friend RenderbufferDeleter;

    std::vector<GLuint> abandonedPrograms;
    std::vector<GLuint> abandonedShaders;
    std::vector<GLuint> abandonedBuffers;
    std::vector<GLuint> abandonedTextures;
    std::vector<GLuint> abandonedVAOs;
    std::vector<GLuint> abandonedFramebuffers;
    std::vector<GLuint> abandonedRenderbuffers;
};

} // namespace gl
} // namespace mbgl
