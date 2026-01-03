#include "global.hpp"


void svgcalc_ramka() {

    int min_allowed_w               = 400;
    int description_field_height    = 80; // для битовых полей

    int idx = tree.rootidx();
    int nodew = oparams.gfx_nodewl[ idx ] + oparams.gfx_nodewr[ idx ];

    int x = iparams.svg_paper_border;
    int y = iparams.svg_paper_border;

    int w = (nodew < min_allowed_w ? min_allowed_w : nodew);    // с учетом минимальной ширины
    w += 2 * iparams.svg_ramka_border;                          // отступ по бокам

    int h = (tree.depthmax() + 1 + 4) * iparams.svg_layerh;

    gfx_ramka.set(x, y, w, h + description_field_height);
    
    oparams.svg_width   = gfx_ramka.w + (2 * iparams.svg_paper_border);
    oparams.svg_height  = gfx_ramka.h + (2 * iparams.svg_paper_border);

}

// return oparams.gfx_ramka_x2 - oparams.gfx_ramka_x1; 
// return oparams.gfx_ramka_y2 - oparams.gfx_ramka_y1;

// Recalculate vertical cut area + header & footer
// oparams.gfx_ramka_height = iparams.svg_def_line_height_ * (tree.depthmax() + 2);


// int minleft     = iparams.svg_width;
// int maxright    = 0;
// int mintop      = iparams.svg_height;
// int maxbot      = 0;

// for(int i=0; i <= tree.cntall(); i++ ) {
//     if( oparams.gfxpos_x[i] < minleft    ) { minleft  = oparams.gfxpos_x[i]; }
//     if( oparams.gfxpos_x[i] > maxright   ) { maxright = oparams.gfxpos_x[i]; }
//     if( oparams.gfxpos_y[i] < mintop     ) { mintop   = oparams.gfxpos_y[i]; }
//     if( oparams.gfxpos_y[i] > maxbot     ) { maxbot   = oparams.gfxpos_y[i]; } }

// while( (maxright - minleft) < 400 ) {
//     minleft     -= 10;
//     maxright    += 10; }

// int dx = 80;
// minleft     -= dx;
// maxright    += dx;
// mintop      -= dx;
// maxbot      += dx;

// copy
// oparams.gfx_ramka_x1    = minleft;
// oparams.gfx_ramka_x2    = maxright;
// oparams.gfx_ramka_y1    = mintop;
// oparams.gfx_ramka_y2    = maxbot;

// int svgcalc_ramka_width     () { return gfx_ramka.w; }
// int svgcalc_ramka_height    () { return gfx_ramka.h; }
