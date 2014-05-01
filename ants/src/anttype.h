#pragma once
#include <functional>

enum AntType
{
    NORMAL = 0,
    DIGGING,
    RED,
    GREEN,
    BLUE
};

namespace std
{
template<>
    struct hash<AntType>
    {
        public:
            size_t operator()(const AntType& antType) const 
            {
                return (size_t)antType;
            }
    };
}
