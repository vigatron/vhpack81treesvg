#pragma once

#include "global.hpp"
#include "calc.hpp"

int		font_align_pixels(std::string str, int fntsize);

void	draw_links		( VHTree & tree , VHTreeGfx & tgfx , int idx);
void	draw_elems		( VHTree & tree , VHTreeGfx & tgfx , int idx , bool statmode );
void	draw_shadows	( VHTree & tree , VHTreeGfx & tgfx , VHRect & rect );

void	draw_bitpath_back	( const VHRect & rect );
void	draw_bitpaths		( VHTree & tree , const VHRect & rect , bool showrate );

void	draw_figures( int xo, int yo, int ww, int hh );

void	draw_ramka();

void	draw_spectrum();
