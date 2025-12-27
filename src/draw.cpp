#include "global.hpp"

using namespace std;

// -------------------------------------------------------------------------------------------------
// SVG GFX Related
// -------------------------------------------------------------------------------------------------


// -------------------------------------------------------------------------------------------------

void draw_background() {

    svg.rect(0, 0, inparams.svg_width, inparams.svg_height, 0, colors::white, colors::white );

    int w = svgcalc_ramka_width();

    for(int ll=0; ll <= tree.depthmax(); ll++) {
        int layposy = svgcalc_layer_posy(ll);
        std::string color = (ll & 1) ? colors::yellowll : colors::yellowl;
        int x = oparams.gfx_ramka_x1;
        int y = svgcalc_layer_posy(ll);
        svg.rect(x, y, w, svgcalc_layer_height(), 0, color, color ); } }

// -------------------------------------------------------------------------------------------------

void draw_scode() {
    
    std::string str = tree.asbin() + " : " + tree.astext(); // TCode : SCode
    int w   = ( svgcalc_ramka_width() )/2;
    int dd  = 4;
    int x   = oparams.gfx_ramka_x1;
    int y   = oparams.gfx_ramka_y1;
    svg.rect(x + dd, y + dd, w - dd*2, 46, 1, colors::lblue, colors::lblue, 8);
    svg.text(x + 18, y + 32, str, inparams.fntSans, 16, colors::nblue ); }

// -------------------------------------------------------------------------------------------------

void draw_ramka() {

    int w = svgcalc_ramka_width();
    int h = svgcalc_ramka_height();

    string str1 = "Визуализатор деревьев Хаффмана";
    string str2 = "V1.00 V01G04A81 (C) 2025";

    int x1 = oparams.gfx_ramka_x1;
    int x2 = oparams.gfx_ramka_x2;
    int y  = oparams.gfx_ramka_y1;

    svg.rect(x1, y, w, h, 1.5, colors::nblue, "none", 12 );
    svg.text(x1 +  10, y - 10, str1, inparams.fntSans, 15, colors::lgray );
    svg.text(x2 - 140, y - 10, str2, inparams.fntSans, 10, colors::lgray ); }

// -------------------------------------------------------------------------------------------------

void draw_layers() {

    int depthmax = tree.scandepth();

    for(int i=0; i <= depthmax; i++) {
        int cx = oparams.gfx_ramka_x1 + 18;
        int cy = svgcalc_layer_posy(i) + svgcalc_layer_height()/2;
        svg.circ(cx + 4, cy-2, inparams.svg_elm_width/2, 1, colors::nyell, colors::myell );
        svg.text(cx, cy, "L" + std::to_string(i), inparams.fntSans, 9, colors::lgray ); } }

// -------------------------------------------------------------------------------------------------

void draw_link(int idx1, int idx2) {
    std::vector<std::string> r;
    int x1 = oparams.gfxpos_x[idx1];
    int y1 = oparams.gfxpos_y[idx1];
    int x2 = oparams.gfxpos_x[idx2];
    int y2 = oparams.gfxpos_y[idx2];
    svg.line(x1, y1, x2, y2, inparams.svg_lnkwidth, colors::mgreen); }

// -------------------------------------------------------------------------------------------------
void draw_links(int idx) {
    if(idx >= tree.cntsyms() ) {
        draw_link(idx, tree.getleft(idx));
        draw_link(idx, tree.getrigh(idx));
        draw_links(tree.getleft(idx));
        draw_links(tree.getrigh(idx)); }
}

// -------------------------------------------------------------------------------------------------

void draw_elm(int idx) {

    bool flagsym = tree.issym(idx);

    int cx = oparams.gfxpos_x[idx];
    int cy = oparams.gfxpos_y[idx];

    int qx = oparams.gfxpos_x[idx] - inparams.svg_elm_width/2;
    int qy = oparams.gfxpos_y[idx] - inparams.svg_elm_width/2;
    int th = 1.5;

    int fontw   = inparams.svg_elm_fntsz * 3 / 4;
    int fonth   = inparams.svg_elm_fntsz * 4 / 7;
    int tx      = oparams.gfxpos_x[idx] - ((idx>9) ? (fontw * 3 / 5) : (fontw / 4));
    int ty      = oparams.gfxpos_y[idx] + (fonth / 2);

    string fnt  = inparams.fntSans;

    std::string colf = flagsym ? colors::mblue : colors::dgreen;
    std::string colb = flagsym ? colors::sblue : colors::lgreen;

    // Debug staff : Rectangle [WL|WR]
    bool show_wl_range = false;
    if(show_wl_range) {
        int sx  = oparams.gfxpos_x[idx] - oparams.gfx_nodewl[idx];
        int ww  = oparams.gfx_nodewl[idx] + oparams.gfx_nodewr[idx];
        int th  = inparams.svg_elm_width;
        svg.rect(sx, qy, ww, th, 1, "black", "orange" ); }

    if(!flagsym) {
        int r = inparams.svg_elm_width*5/8;
        svg.circ(cx, cy, r, th*3/8, colf, "white"); }

    int w = inparams.svg_elm_width;
    if(flagsym) { svg.rect(qx, qy, w, w, th, colf, colb, w * 0.2 );
    } else { svg.circ(cx, cy, w/2, th, colf, colb); }

    svg.text(tx, ty, std::to_string(idx), fnt, fontw, colors::gray );

    // Props print
    bool show_props = false;
    if(show_props) {
        string str1 = std::to_string(oparams.gfxpos_x[idx]);
        svg.text(tx, ty - 30, str1, fnt, 10, "black" );
        string wlwr = std::to_string(oparams.gfx_nodewl[idx]) + ":" + std::to_string(oparams.gfx_nodewr[idx]);
        svg.text(tx, ty - 20, wlwr, fnt, 10, "black" ); } }

// -------------------------------------------------------------------------------------------------

void draw_bitpath_sym(int idx) {
    int         ww          = (oparams.gfx_ramka_x2 - oparams.gfx_ramka_x1) / 2;
    int         hh          = 14;
    int         symsincol   = 8;
    int         coln        = idx / symsincol;
    int         rown        = idx % symsincol;
    int         symx        = oparams.gfx_ramka_x1 + 40 + (coln * ww);
    int         symy        = oparams.gfx_ramka_y2 + 40 + (rown * hh);
    string      strsymn     = "#" + std::to_string(idx) + ":";
    string      bitpath     = tree.bitpath(idx);

    string color = colors::gray;
    svg.text( symx      , symy, strsymn, inparams.fntSans, 10, color);
    svg.text( symx + 40 , symy, bitpath, inparams.fntSans, 10, color);
}

void draw_bitpaths() { for(int i=0; i < tree.cntsyms();i++) { draw_bitpath_sym(i); } }

// -------------------------------------------------------------------------------------------------

void draw_elems(int idx) {
    draw_elm(idx);
    if( tree.isnode(idx) ) {
        draw_elems( tree.getleft(idx) );
        draw_elems( tree.getrigh(idx) ); } }

void RenderTreeGfx() {
    svg.begin( inparams.svg_width, inparams.svg_height );
    draw_background();
    draw_links( tree.cntall() ); // recurse
    draw_elems( tree.cntall() ); // recurse
    draw_layers();
    draw_scode();
    draw_ramka();
    draw_bitpaths();
    svg.end(); }
