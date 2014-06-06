#pragma once

enum ButtonType { B_DEFAULT = 0, B_ZOOM, B_ZOOM2 };

namespace std {

    template <>
        struct hash<ButtonType>
        {
            std::size_t operator()(const ButtonType& b) const
            {
                return std::hash<int32_t>()((int32_t) b);
            }
        };

}
