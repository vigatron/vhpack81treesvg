#include "global.hpp"
#include "calc.hpp"



// gfxrect_layers.sy
// iparams.svg_layerh

// -------------------------------------------------------------------------------------------------
// SVG Calculations
// -------------------------------------------------------------------------------------------------

// void svgcalc_transform() {

// int border  = 40;
// int dx      = oparams.gfx_ramka_x1 - border;
// int dy      = oparams.gfx_ramka_y1 - border;

// oparams.gfx_ramka_x1 -= dx;
// oparams.gfx_ramka_x2 -= dx;
// oparams.gfx_ramka_y1 -= dy;
// oparams.gfx_ramka_y2 -= dy;

// for(int i=0; i<256;i++) { oparams.gfxpos_x[i] -= dx; oparams.gfxpos_y[i] -= dy; }

// // Final Paper dims
// iparams.svg_width   = oparams.gfx_ramka_x2 + border;
// iparams.svg_height  = oparams.gfx_ramka_y2 + border + 140;

// }



// Move to top left corner
// svgcalc_transform();


// oparams.gfx_ramka_height / (tree.depthmax() + 2);
// int svgcalc_layer_height    () { int r = iparams.svg_def_line_height_; return r; }
// int posy = svgcalc_centery() - oparams.gfx_ramka_height/2  + layn * svgcalc_layer_height();
// int svgcalc_centery ()              { return iparams.svg_height /2; }
