#pragma once

#include "global.hpp"
#include "calc.hpp"

int     font_align_pixels(std::string str, int fntsize);

void    draw_elm_value(int idx);
void    draw_elm(int idx);
void    draw_elems(int idx);
void    draw_links(int idx);

void    draw_bitpath_back();
void    draw_bitpaths();
