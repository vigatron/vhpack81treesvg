#include "draw.hpp"

using namespace std;

// -------------------------------------------------------------------------------------------------
void draw_elm_value(int idx) {

    VHTree::stobj * pooo    = tree[idx];
    int             fntsz   = 6;

    string  str     = "S" + std::to_string(pooo->v);
    int     mdx     = font_align_pixels(str, fntsz); // (str1.size() < 2) ? fntsz/3 : fntsz*2/3;

    int     x  = oparams.gfxpos_x[idx] - mdx;
    int     y  = oparams.gfxpos_y[idx] + fntsz + 2;
    string  fnt = iparams.fntSans;

    svg.text(x, y, str, fnt, fntsz, "gray" );
}

// -------------------------------------------------------------------------------------------------

void draw_elm(int idx) {

    bool flagroot   = idx == tree.rootidx();
    bool flagsym    = tree.issym(idx);

    int cx = oparams.gfxpos_x[idx];
    int cy = oparams.gfxpos_y[idx];

    int qx = oparams.gfxpos_x[idx] - iparams.svg_elm_width/2;
    int qy = oparams.gfxpos_y[idx] - iparams.svg_elm_width/2;
    int th = 1.5;

    int fontw   = iparams.svg_elm_fntsz * 3 / 4;
    int fonth   = iparams.svg_elm_fntsz * 4 / 7;
    string fnt  = iparams.fntSans;

    std::string colf = flagsym ? colors::mblue : colors::dgreen;
    std::string colb = flagsym ? colors::sblue : colors::lgreen;
    std::string cols = flagsym ? colors::mblue : colors::mgreen;  // separator
    
    if(flagroot) {
        colf    = colors::orangel;
        colb    = colors::nyell;
        cols    = colf;
    }

    // Debug staff : Rectangle [WL|WR]
    if( iparams.show_width_elmslr ) {
        int sx  = oparams.gfxpos_x[idx] - oparams.gfx_nodewl[idx];
        int ww  = oparams.gfx_nodewl[idx] + oparams.gfx_nodewr[idx];
        int th  = iparams.svg_elm_width;
        svg.rect(sx, qy, ww, th, 1, "black", "orange" ); }


    int     ra  = iparams.svg_elm_width*5/8;
    int     w   = iparams.svg_elm_width;
    int     r   = w/2;

    if(flagsym) {
        svg.rect(qx, qy, w, w, th, colf, colb, w * 0.2 );
    } else {
        svg.circ(cx, cy, ra, th*3/8, colf, "white" );
        svg.circ(cx, cy, w/2, th, colf, colb); }


    int tx      = oparams.gfxpos_x[idx] - ((idx>9) ? (fontw * 3 / 5) : (fontw / 4));
    int ty      = oparams.gfxpos_y[idx] + (fonth / 2);

    // Show counts ?
    if(iparams.from_spectrum) {
        // Separator
        svg.line( cx - r, cy, cx + r, cy, 1, cols);
        ty -= fonth*0.9;
        draw_elm_value(idx);
    }

    // Index
    svg.text(tx, ty, std::to_string(idx), fnt, fontw, colors::gray );

    // Props print: gfx X L:R ( debug )
    if(iparams.show_dbg_xwlwr) {

        // X-Pos
        string str1 = std::to_string(oparams.gfxpos_x[idx]);
        svg.text(tx, ty - 30, str1, fnt, 3, "black" );

        // WL WR
        string strl = std::to_string(oparams.gfx_nodewl[idx]);
        string strr = std::to_string(oparams.gfx_nodewr[idx]);
        string wlwr = strl + ":" + strr;
        svg.text(tx, ty - 20, wlwr, fnt, 3, "black" ); }

}

// -------------------------------------------------------------------------------------------------

void draw_elems(int idx) {
    draw_elm(idx);
    if( tree.isnode(idx) ) {
        draw_elems( tree.getleft(idx) );
        draw_elems( tree.getrigh(idx) ); } }

// -------------------------------------------------------------------------------------------------

void draw_link(int idx1, int idx2) {
    std::vector<std::string> r;
    int x1 = oparams.gfxpos_x[idx1];
    int y1 = oparams.gfxpos_y[idx1];
    int x2 = oparams.gfxpos_x[idx2];
    int y2 = oparams.gfxpos_y[idx2];
    svg.line(x1, y1, x2, y2, iparams.svg_lnkwidth, colors::mgreen); }

// -------------------------------------------------------------------------------------------------
void draw_links(int idx) {
    if(idx >= tree.cntsyms() ) {
        draw_link(idx, tree.getleft(idx));
        draw_link(idx, tree.getrigh(idx));
        draw_links(tree.getleft(idx));
        draw_links(tree.getrigh(idx)); } }
