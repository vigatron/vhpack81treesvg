#include "draw.hpp"

// -------------------------------------------------------------------------------------------------
static void draw_layers_nrs() {

    int depthmax = tree.arch().depthmax();

    for(int i=0; i <= depthmax; i++) {
        int cx = gfxrect_ramka.sx + 18;
        int cy = svg_getlayer_posyc(i);
        svg.circ(cx + 4, cy-2, iparams.svg_elm_width/2, 1, colors::nyell, colors::myell );
        svg.text(cx, cy, "L" + std::to_string(i), iparams.fntSans, 9, colors::lgray ); } }

// -------------------------------------------------------------------------------------------------
void draw_layers_back() {

    // Draw layers back
    for(int ll=0; ll <= tree.arch().depthmax(); ll++) {
        int x = gfxrect_ramka.sx;
        int y = svg_getlayer_posy(ll);
        std::string color = (ll & 1) ? colors::yellowll : colors::yellowl;
        svg.rect(x, y, gfxrect_ramka.w, iparams.svg_layerh, 0, color, color );

		int ymid = y + iparams.svg_layerh/2;
        std::string clrm = (ll & 1) ? "#F8E8C0" : "#F0E0A0";
		svg.line(x, ymid, x + gfxrect_ramka.w, ymid, 1, clrm, "15,10");
	}

    draw_layers_nrs(); }
