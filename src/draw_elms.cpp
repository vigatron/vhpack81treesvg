#include "draw.hpp"

using namespace std;


struct sColor {
    std::string colf;   // Front
    std::string colb;   // Background
    std::string cols;   // Separator
    std::string colt;   // Text
};

// Sym Node Root Extra
const sColor  arrcolors[3] = {
    { colors::mblue     , colors::sblue     , colors::mblue     , "gray" },
    { colors::dgreen    , colors::lgreen    , colors::mgreen    , "gray" },
    { colors::orangel   , colors::nyell     , colors::orangel   , "gray" } };

// -------------------------------------------------------------------------------------------------
static const sColor * elm_color(int idx) {
    if( tree.arch().isroot(idx) )  return & arrcolors[2];
    if( tree.arch().issym (idx) )  return & arrcolors[0];
    return & arrcolors[1]; }

// -------------------------------------------------------------------------------------------------
static void draw_dbg_LW (int idx) {
    int     x           = oparams.gfxpos_x[idx]     - oparams.gfx_nodewl[idx];
    int     y           = oparams.gfxpos_y[idx]     - iparams.svg_elm_width/2;
    int     ww          = oparams.gfx_nodewl[idx]   + oparams.gfx_nodewr[idx];
    int     th          = iparams.svg_elm_width;
    svg.rect(x, y, ww, th, 1, "black", "orange" ); }

// -------------------------------------------------------------------------------------------------
static void draw_dbg_txt_LW(int idx) {
    int     x           = oparams.gfxpos_x[idx];
    int     y           = oparams.gfxpos_y[idx];
    string  fnt         = iparams.fntSans;

    // X-Pos value
    string str1 = std::to_string(oparams.gfxpos_x[idx]);
    svg.text(x, y - 30, str1, fnt, 3, "black" );

    // WL WR values
    string strl = std::to_string(oparams.gfx_nodewl[idx]);
    string strr = std::to_string(oparams.gfx_nodewr[idx]);
    string wlwr = strl + ":" + strr;
    svg.text(x, y - 20, wlwr, fnt, 3, "black" ); }

// -------------------------------------------------------------------------------------------------
static void draw_elm_form (int idx, int x, int y) {

    bool            flagsym     = tree.arch().issym(idx);
    int             elmsize     = iparams.svg_elm_width;
    int             r           = elmsize / 2;
    int             th          = 1.5;
    const sColor *  colors      = elm_color(idx);

    if(flagsym) {

        int rx = x - elmsize/2, ry = y - elmsize/2;
        string colf     = colors->colf, colb    = colors->colb;
        string colbf    = "#F8F8FF", colbb   = "#EAEAFF" ; 

        svg.rect( rx + 4, ry + 4, elmsize, elmsize, th, colbf, colbb, elmsize * 0.2 );
        svg.rect( rx, ry, elmsize, elmsize, th, colf, colb, elmsize * 0.2 );

    } else {
        int     ra      = iparams.svg_elm_width*5/8;
        // string  colsh   = "#F0FFF0";
        // svg.circ(x+2, y+2, ra, th*3/8, colsh, colsh ); // Shadow

        svg.circ(x, y, ra, th*3/8, colors->colf, "white" );
        if( tree.getswap(idx)) { svg.arc(x, y, 14, 4, colors->colf); }
        svg.circ(x, y, elmsize / 2, th, colors->colf, colors->colb); }

     // Separator
    if(iparams.from_spectrum) { svg.line( x - r, y, x + r, y, 1, colors->cols); }
}

// -------------------------------------------------------------------------------------------------
static void draw_elm_value(int idx) {

    VHTreeArch::stobj * pooo    = tree[idx];

    int             fntsz   = 6;
    string          str     = "S" + std::to_string(pooo->v);
    int             mdx     = font_align_pixels(str, fntsz);
    int             x       = oparams.gfxpos_x[idx] - mdx;
    int             y       = oparams.gfxpos_y[idx] + fntsz + 2;
    string          fnt     = iparams.fntSans;

    svg.text(x, y, str, fnt, fntsz, "gray" ); }

// -------------------------------------------------------------------------------------------------
void draw_elm(int idx) {

    bool    flagroot    = tree.arch().isroot(idx);
    bool    flagsym     = tree.arch().issym(idx);
    int     cx          = oparams.gfxpos_x[idx];
    int     cy          = oparams.gfxpos_y[idx];
    int     qx          = oparams.gfxpos_x[idx] - iparams.svg_elm_width/2;
    int     qy          = oparams.gfxpos_y[idx] - iparams.svg_elm_width/2;
    int     fontw       = iparams.svg_elm_fntsz * 3 / 4;
    int     fonth       = iparams.svg_elm_fntsz * 4 / 7;
    string  fnt         = iparams.fntSans;

    const sColor * colors = elm_color(idx);

    // Debug staff : Rectangle [WL|WR]
    if( iparams.show_width_elmslr ) { draw_dbg_LW(idx); }

    draw_elm_form(idx, cx, cy);

    int     tx  = oparams.gfxpos_x[idx] - ((idx>9) ? (fontw * 3 / 5) : (fontw / 4));
    int     ty  = oparams.gfxpos_y[idx] + (fonth / 2);

    // Show counts ?
    if(iparams.from_spectrum) { ty -= fonth*0.9; draw_elm_value(idx); }

    // Index
    svg.text(tx, ty, std::to_string(idx), fnt, fontw, colors::gray );

    // Props print: gfx X L:R ( debug )
    if(iparams.show_dbg_xwlwr) { draw_dbg_txt_LW(idx); }

}

// -------------------------------------------------------------------------------------------------

void draw_elems(int idx) {
    draw_elm(idx);
    if( tree.arch().isnode(idx) ) {
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
    if(idx >= tree.arch().cntsyms() ) {
        draw_link(idx, tree.getleft(idx));
        draw_link(idx, tree.getrigh(idx));
        draw_links(tree.getleft(idx));
        draw_links(tree.getrigh(idx)); } }
