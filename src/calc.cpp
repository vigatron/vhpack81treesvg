#include "global.hpp"



// -------------------------------------------------------------------------------------------------
// SVG Calculations
// -------------------------------------------------------------------------------------------------

// oparams.gfx_ramka_height / (tree.depthmax() + 2);

int svgcalc_centery         () { return iparams.svg_height /2; }

int svgcalc_layer_height    () { int r = iparams.svg_def_line_height_; return r; }


int svgcalc_layer_posy      (int layn)  {
    int posy =  svgcalc_centery() - oparams.gfx_ramka_height/2  + layn * svgcalc_layer_height();
    return posy; }

int svgcalc_layer_centerposy(int layn) {
    return svgcalc_layer_posy(layn) + iparams.svg_def_line_height_ / 2; }

    
int svgcalc_ramka_width     () {
    return oparams.gfx_ramka_x2 - oparams.gfx_ramka_x1; }

int svgcalc_ramka_height    () {
    return oparams.gfx_ramka_y2 - oparams.gfx_ramka_y1; }

// -------------------------------------------------------------------------------------------------

void svgcalc_ramka() {

    int minleft     = iparams.svg_width;
    int maxright    = 0;
    int mintop      = iparams.svg_height;
    int maxbot      = 0;

    for(int i=0; i <= tree.cntall(); i++ ) {
        if( oparams.gfxpos_x[i] < minleft    ) { minleft  = oparams.gfxpos_x[i]; }
        if( oparams.gfxpos_x[i] > maxright   ) { maxright = oparams.gfxpos_x[i]; }
        if( oparams.gfxpos_y[i] < mintop     ) { mintop   = oparams.gfxpos_y[i]; }
        if( oparams.gfxpos_y[i] > maxbot     ) { maxbot   = oparams.gfxpos_y[i]; } }

    while( (maxright - minleft) < 400 ) {
        minleft     -= 10;
        maxright    += 10; }

    int dx = 80;

    minleft     -= dx;
    maxright    += dx;
    mintop      -= dx;
    maxbot      += dx;

    // copy
    oparams.gfx_ramka_x1    = minleft;
    oparams.gfx_ramka_x2    = maxright;
    oparams.gfx_ramka_y1    = mintop;
    oparams.gfx_ramka_y2    = maxbot;

}

void svgcalc_w(int idx, int * retwl, int * retwr) {

    int wl, wr;

    int idxl = tree.getleft(idx);
    if( tree.issym(idxl) ) {
        int ws = iparams.svg_node_xdist + iparams.svg_node_spacr;
        oparams.gfx_nodewl[idx ] = ws;
        oparams.gfx_nodewl[idxl] = ws / 2;
        oparams.gfx_nodewr[idxl] = ws / 2;
    } else {
        svgcalc_w(idxl, &wl, &wr);
        oparams.gfx_nodewl[idx] = wl + wr; }

    int idxr = tree.getrigh(idx);
    if( tree.issym(idxr) ) {
        int ws = iparams.svg_node_xdist + iparams.svg_node_spacr;
        oparams.gfx_nodewr[idx ] = ws;
        oparams.gfx_nodewl[idxr] = ws / 2;
        oparams.gfx_nodewr[idxr] = ws / 2;
    } else {
        svgcalc_w(idxr, &wl, &wr);
        oparams.gfx_nodewr[idx]  = wl + wr; }

    *retwl = oparams.gfx_nodewl[idx];
    *retwr = oparams.gfx_nodewr[idx];
    printf("W%2d  (L:%d R:%d) S:%d\n", idx, *retwl, *retwr, *retwl + *retwr); }


void svgcalc_set_coords(int idx, int cx) {

    oparams.gfxpos_x[idx] = cx;
    oparams.gfxpos_y[idx] = svgcalc_layer_centerposy( tree.getlay(idx));

    if( tree.isnode( idx ) ) {
        int idxl = tree.getleft(idx);
        int lnodex = cx - oparams.gfx_nodewr[ idxl ];
        svgcalc_set_coords( idxl, lnodex );

        int idxr = tree.getrigh(idx);
        int rnodex = cx + oparams.gfx_nodewl[ idxr ];
        svgcalc_set_coords( idxr, rnodex ); } }

void svg_calc_transform() {

    int border  = 40;
    int dx      = oparams.gfx_ramka_x1 - border;
    int dy      = oparams.gfx_ramka_y1 - border;

    oparams.gfx_ramka_x1 -= dx;
    oparams.gfx_ramka_x2 -= dx;
    oparams.gfx_ramka_y1 -= dy;
    oparams.gfx_ramka_y2 -= dy;

    for(int i=0; i<256;i++) { oparams.gfxpos_x[i] -= dx; oparams.gfxpos_y[i] -= dy; }

    // Final Paper dims
    iparams.svg_width   = oparams.gfx_ramka_x2 + border;
    iparams.svg_height  = oparams.gfx_ramka_y2 + border + 140;
}

void CalculateTreeGfx() {
    
    // Recalculate vertical cut area + header & footer
    oparams.gfx_ramka_height = iparams.svg_def_line_height_ * (tree.depthmax() + 2);

    // Recurse calculate nodes widths pxls ( L & R )
    int wl, wr;
    svgcalc_w( tree.rootidx(), & wl, & wr);

    // recurse, start from root
    svgcalc_set_coords( tree.rootidx() , iparams.svg_width / 2);

    svgcalc_ramka();

    // Move to top left corner
    // svg_calc_transform();
}
