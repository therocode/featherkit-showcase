#pragma once

enum ButtonType { B_DEFAULT = 0, B_INTERACTIVE, B_COLOUR_BLEND, B_PARALLAX, B_ANIMATION, B_TEXT, B_CAMERA };

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
