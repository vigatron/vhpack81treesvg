#include "global.hpp"
#include "calc.hpp"


// -------------------------------------------------------------------------------------------------
// SVG Calculations
// -------------------------------------------------------------------------------------------------

// oparams.gfx_ramka_height / (tree.depthmax() + 2);
// int svgcalc_layer_height    () { int r = iparams.svg_def_line_height_; return r; }
// int posy = svgcalc_centery() - oparams.gfx_ramka_height/2  + layn * svgcalc_layer_height();

// int svgcalc_centery ()              { return iparams.svg_height /2; }

int svg_getlayer_posy (int layn)    { return gfx_ramka.sy + (layn + 2) * iparams.svg_layerh; }

int svg_getlayer_posyc(int layn)    { return svg_getlayer_posy(layn) + iparams.svg_layerh / 2; }

// -------------------------------------------------------------------------------------------------

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
    oparams.gfxpos_y[idx] = svg_getlayer_posyc( tree.getlay(idx));

    if( tree.isnode( idx ) ) {
        int idxl = tree.getleft(idx);
        int lnodex = cx - oparams.gfx_nodewr[ idxl ];
        svgcalc_set_coords( idxl, lnodex );

        int idxr = tree.getrigh(idx);
        int rnodex = cx + oparams.gfx_nodewl[ idxr ];
        svgcalc_set_coords( idxr, rnodex ); } }


void CalculateTreeGfx() {

    // Recurse calculate nodes widths pxls ( L & R )
    int wl, wr;
    svgcalc_w( tree.rootidx(), & wl, & wr);

    svgcalc_ramka(); 

    // recurse, start from root
    int rootidx = tree.rootidx();
    svgcalc_set_coords( rootidx , gfx_ramka.sx + (gfx_ramka.w/2) - oparams.gfx_nodewl[rootidx]/2 );

    // Move to top left corner
    svgcalc_transform();
}
