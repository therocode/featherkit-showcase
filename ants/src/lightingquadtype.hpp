#pragma once

enum LightingQuadType { LIGHT, DARK, LARGE_HALO, SMALL_HALO };

namespace std
{
template<>
    struct hash<LightingQuadType>
    {
        public:
            size_t operator()(const LightingQuadType& lightingQuadType) const 
            {
                return (size_t)lightingQuadType;
            }
    };
}
