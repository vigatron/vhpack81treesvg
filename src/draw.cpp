#include "draw.hpp"

#include <ctime>

using namespace std;

// -------------------------------------------------------------------------------------------------
// SVG GFX Related
// -------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------

void draw_background() {

    // Paper
    svg.rect(0, 0, oparams.svg_width, oparams.svg_height, 0, colors::white, colors::white );

    // Draw layers back
    for(int ll=0; ll <= tree.depthmax(); ll++) {
        int x = gfx_ramka.sx;
        int y = svg_getlayer_posy(ll);
        std::string color = (ll & 1) ? colors::yellowll : colors::yellowl;
        svg.rect(x, y, gfx_ramka.w, iparams.svg_layerh, 0, color, color ); } }

// -------------------------------------------------------------------------------------------------

void draw_layers() {

    int depthmax = tree.scandepth();

    for(int i=0; i <= depthmax; i++) {
        int cx = gfx_ramka.sx + 18;
        int cy = svg_getlayer_posyc(i);
        svg.circ(cx + 4, cy-2, iparams.svg_elm_width/2, 1, colors::nyell, colors::myell );
        svg.text(cx, cy, "L" + std::to_string(i), iparams.fntSans, 9, colors::lgray ); } }

// -------------------------------------------------------------------------------------------------

void draw_scode() {
    
    std::string str = tree.asbin() + " : " + tree.astext(); // TCode : SCode
    int w   = gfx_ramka.w/2;
    int dd  = 4;
    int x   = gfx_ramka.sx;
    int y   = gfx_ramka.sy;
    svg.rect(x + dd, y + dd, w - dd*2, 46, 1, colors::lblue, colors::lblue, 8);
    svg.text(x + 18, y + 32, str, iparams.fntSans, 16, colors::nblue ); }

// -------------------------------------------------------------------------------------------------

void draw_ramka() {

    int w = gfx_ramka.w;
    int h = gfx_ramka.h;

    string str1 = "Визуализатор деревьев Хаффмана";
    string str2 = "V1.00 V01G04A81 (C) 2025, 2026";

    int x1 = gfx_ramka.sx;
    int x2 = gfx_ramka.ex;
    int y  = gfx_ramka.sy;

    svg.rect(x1, y, w, h, 1.5, colors::nblue, "none", 12 );
    svg.text(x1 +  10, y - 10, str1, iparams.fntSans, 15, colors::lgray );
    svg.text(x2 - 170, y - 20, str2, iparams.fntSans, 10, colors::lgray ); }

// -------------------------------------------------------------------------------------------------
int font_align_pixels(std::string str, int fntsize) {
    return str.size() * (fntsize*0.6) / 2; }


// -------------------------------------------------------------------------------------------------

void draw_bitpath_sym(int idx) {

    int colsmin = tree.cntsyms() / 8;
    if( tree.cntsyms() % 8 ) colsmin++;
    if(!colsmin) colsmin++;

    // ширина столбцов зависит от количества узлов дерева
    int         ww          = gfx_ramka.w / colsmin; 

    int         hh          = 14;
    int         symsincol   = 8;
    int         coln        = idx / symsincol;
    int         rown        = idx % symsincol;

    int         symx        = gfx_ramka.sx + 40 + (coln * ww);
    int         symy        = gfx_ramka.ey - (symsincol * hh) + (rown * hh);

    string      strsymn     = "#" + std::to_string(idx) + ":";
    string      bitpath     = tree.bitpath(idx);

    string color = colors::gray;
    svg.text( symx      , symy, strsymn, iparams.fntSans, 11, color);
    svg.text( symx + 30 , symy, bitpath, iparams.fntSans, 11, color);
}

// -------------------------------------------------------------------------------------------------

void draw_bitpaths() { for(int i=0; i < tree.cntsyms();i++) { draw_bitpath_sym(i); } }

// -------------------------------------------------------------------------------------------------

void draw_tstamp() {

    // 1. Get the current time in seconds since the Unix epoch
    std::time_t now = std::time(nullptr);

    // 2. Convert to a local time structure
    std::tm tmb = *std::localtime(&now);

    // 3. Format the time into a string (C++11 and later using streams)
    // Example format: YYYY-MM-DD HH:MM:SS    
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tmb);

    string fnt = iparams.fntSans;
    int x2 = gfx_ramka.ex - 170;
    int y  = gfx_ramka.sy - 7;
    svg.text(x2, y, buffer, fnt, 10, colors::lgray);
}

// -------------------------------------------------------------------------------------------------

void RenderTreeGfx() {
    svg.begin( oparams.svg_width, oparams.svg_height );
    draw_background();
    draw_links( tree.cntall() ); // recurse
    draw_elems( tree.cntall() ); // recurse
    draw_layers();
    draw_scode();
    draw_ramka();
    draw_tstamp();
    draw_bitpaths();
    svg.end(); }
