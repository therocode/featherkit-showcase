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

template<>
    struct std::hash<AntType>
    {
        public:
            std::size_t operator()(const AntType& antType) const 
            {
                return (size_t)antType;
            }
    };
