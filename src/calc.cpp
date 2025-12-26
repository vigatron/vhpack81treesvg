#include "global.hpp"



// -------------------------------------------------------------------------------------------------
// SVG Calculations
// -------------------------------------------------------------------------------------------------

int svgcalc_layer_height    ()          {
    int activeh = inparams.svg_height - inparams.svg_height_cut;
    return activeh / (tree.depthmax() + 1); }

int svgcalc_layer_posy(int layn)  {
    int posy = inparams.svg_height_cut/2 + layn * svgcalc_layer_height();
    return posy; }

int svgcalc_layer_centerposy(int layn) {
    return svgcalc_layer_posy(layn) + svgcalc_layer_height()/2; }

int svgcalc_ramka_width     () {
    return oparams.gfx_ramka_x2 - oparams.gfx_ramka_x1; }

int svgcalc_ramka_height    () {
    return oparams.gfx_ramka_y2 - oparams.gfx_ramka_y1; }

// -------------------------------------------------------------------------------------------------

void svgcalc_ramka() {

    int minleft     = inparams.svg_width;
    int maxright    = 0;
    int mintop      = inparams.svg_height;
    int maxbot      = 0;

    for(int i=0; i <= tree.cntall(); i++ ) {
        if( oparams.gfxpos_x[i] < minleft    ) { minleft  = oparams.gfxpos_x[i]; }
        if( oparams.gfxpos_x[i] > maxright   ) { maxright = oparams.gfxpos_x[i]; }
        if( oparams.gfxpos_y[i] < mintop     ) { mintop   = oparams.gfxpos_y[i]; }
        if( oparams.gfxpos_y[i] > maxbot     ) { maxbot   = oparams.gfxpos_y[i]; } }

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
        int ws = inparams.svg_node_xdist + inparams.svg_node_spacr;
        oparams.gfx_nodewl[idx ] = ws;
        oparams.gfx_nodewl[idxl] = ws / 2;
        oparams.gfx_nodewr[idxl] = ws / 2;
    } else {
        svgcalc_w(idxl, &wl, &wr);
        oparams.gfx_nodewl[idx] = wl + wr; }

    int idxr = tree.getrigh(idx);
    if( tree.issym(idxr) ) {
        int ws = inparams.svg_node_xdist + inparams.svg_node_spacr;
        oparams.gfx_nodewr[idx ] = ws;
        oparams.gfx_nodewl[idxr] = ws / 2;
        oparams.gfx_nodewr[idxr] = ws / 2;
    } else {
        svgcalc_w(idxr, &wl, &wr);
        oparams.gfx_nodewr[idx]  = wl + wr; }

    *retwl = oparams.gfx_nodewl[idx];
    *retwr = oparams.gfx_nodewr[idx];
    printf("W%2d  (L:%d R:%d) S:%d\n", idx, *retwl, *retwr, *retwl + *retwr); }


void svgcalc_set_coords(int idx, int nodex) {

    oparams.gfxpos_x[idx] = nodex;
    oparams.gfxpos_y[idx] = svgcalc_layer_centerposy( tree.getlay(idx));

    if( tree.isnode( idx ) ) {
        int idxl = tree.getleft(idx);
        int lnodex = nodex - oparams.gfx_nodewr[ idxl ];
        svgcalc_set_coords( idxl, lnodex );

        int idxr = tree.getrigh(idx);
        int rnodex = nodex + oparams.gfx_nodewl[ idxr ];
        svgcalc_set_coords( idxr, rnodex ); } }


void CalculateTreeGfx() {
    
    // recurse calculate nodes widths pxls ( L & R )
    int wl, wr;
    svgcalc_w( tree.rootidx(), & wl, & wr);

    // recurse, start from root
    svgcalc_set_coords( tree.rootidx() , inparams.svg_width / 2);
    svgcalc_ramka();
}
