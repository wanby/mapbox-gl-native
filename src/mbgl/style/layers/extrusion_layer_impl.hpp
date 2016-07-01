#pragma once

#include <mbgl/style/layer_impl.hpp>
#include <mbgl/style/layers/extrusion_layer.hpp>
#include <mbgl/style/layers/extrusion_layer_properties.hpp>

namespace mbgl {
    namespace style {

        class ExtrusionLayer::Impl : public Layer::Impl {
        public:
            std::unique_ptr<Layer> clone() const override;

            void parseLayout(const JSValue&) override {};
            void parsePaints(const JSValue&) override;

            void cascade(const CascadeParameters&) override;
            bool recalculate(const CalculationParameters&) override;

            std::unique_ptr<Bucket> createBucket(BucketParameters&) const override;

            float getQueryRadius() const override;
            bool queryIntersectsGeometry(
                                         const GeometryCollection& queryGeometry,
                                         const GeometryCollection& geometry,
                                         const float bearing,
                                         const float pixelsToTileUnits) const override;
            
            ExtrusionPaintProperties paint;
        };
        
    } // namespace style
} // namespace mbgl
