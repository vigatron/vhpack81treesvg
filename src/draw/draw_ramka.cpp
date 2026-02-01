#include "draw.hpp"

// -------------------------------------------------------------------------------------------------
void draw_ramka() {

    int w = gfxrect_ramka.w;
    int h = gfxrect_ramka.h;

    std::string str1 = "Визуализатор деревьев Хаффмана";
    std::string str2 = "V1.00 V01G04A81 (C) 2025, 2026";

    int x1 = gfxrect_ramka.sx;
    int x2 = gfxrect_ramka.ex;
    int y  = gfxrect_ramka.sy;

    svg.rect(x1, y, w, h, 1.5, colors::nblue, "none", 12 );
    svg.text(x1 +  10, y - 10, str1, iparams.fntSans, 15, colors::lgray );
    svg.text(x2 - 170, y - 20, str2, iparams.fntSans, 10, colors::lgray ); }
