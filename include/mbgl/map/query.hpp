#pragma once

#include <mbgl/util/optional.hpp>
#include <mbgl/style/filter.hpp>

#include <string>

namespace mbgl {

class SourceQueryOptions {
public:
    // Required for VectorSource, ignored for GeoJSONSource
    optional<std::string> sourceLayer;

    optional<style::Filter> filter;
};

}
