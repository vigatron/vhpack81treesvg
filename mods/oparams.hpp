#pragma once

#include "vhplatform.hpp"

// Calculated values
class TreeGenParamsOut {

    public:

        int     svg_width;
        int     svg_height;

        int     gfxpos_x    [256];    // X-Координата узла
        int     gfxpos_y    [256];    // Y-Координата узла

        int     gfx_nodewl  [256];    // Ширина левой  ветки
        int     gfx_nodewr  [256];    // Ширина правой ветки
};
