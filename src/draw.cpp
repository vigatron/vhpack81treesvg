#include "draw.hpp"

#include <ctime>

using namespace std;

// -------------------------------------------------------------------------------------------------
// SVG GFX Related
// -------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------
void draw_scode() {
    
    std::string str = tree.SCodeToTCode() + " : " + tree.SCodeToText(); // TCode : SCode
    int w   = gfxrect_ramka.w/2;
    int dd  = 4;
    int x   = gfxrect_ramka.sx;
    int y   = gfxrect_ramka.sy;
    svg.rect(x + dd, y + dd, w - dd*2, 46, 1, colors::lblue, colors::lblue, 8);
    svg.text(x + 18, y + 32, str, iparams.fntSans, 16, colors::nblue ); }

// -------------------------------------------------------------------------------------------------
void draw_ramka() {

    int w = gfxrect_ramka.w;
    int h = gfxrect_ramka.h;

    string str1 = "Визуализатор деревьев Хаффмана";
    string str2 = "V1.00 V01G04A81 (C) 2025, 2026";

    int x1 = gfxrect_ramka.sx;
    int x2 = gfxrect_ramka.ex;
    int y  = gfxrect_ramka.sy;

    svg.rect(x1, y, w, h, 1.5, colors::nblue, "none", 12 );
    svg.text(x1 +  10, y - 10, str1, iparams.fntSans, 15, colors::lgray );
    svg.text(x2 - 170, y - 20, str2, iparams.fntSans, 10, colors::lgray ); }

// -------------------------------------------------------------------------------------------------
int font_align_pixels(std::string str, int fntsize) {
    return str.size() * (fntsize*0.6) / 2; }


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
    int x2 = gfxrect_ramka.ex - 170;
    int y  = gfxrect_ramka.sy - 7;
    svg.text(x2, y, buffer, fnt, 10, colors::lgray);
}

// -------------------------------------------------------------------------------------------------
void draw_callparams() {

    std::string callParams = "Call parameters : " + iparams.callparams;

    string fnt = iparams.fntSans;
    int x2 = gfxrect_ramka.sx + 10;
    int y  = gfxrect_ramka.ey + 16;
    svg.text(x2, y, callParams, fnt, 8, colors::lgray);

}

void draw_debug() {

    return;

    { VHRect * prect = & gfxrect_header;
    svg.rect( prect->sx, prect->sy, prect->w, prect->h, 1, "red" ); }

    { VHRect * prect = & gfxrect_layers;
    svg.rect( prect->sx, prect->sy, prect->w, prect->h, 1, "red" ); }

    { VHRect * prect = & gfxrect_shadows;
    svg.rect( prect->sx, prect->sy, prect->w, prect->h, 1, "red" ); }

    { VHRect * prect = & gfxrect_bitfield;
    svg.rect( prect->sx, prect->sy, prect->w, prect->h, 1, "red" ); }

}

// -------------------------------------------------------------------------------------------------
void RenderTreeGfx() {

    svg.begin( oparams.svg_width, oparams.svg_height );

    // Full Paper
    svg.rect(0, 0, oparams.svg_width, oparams.svg_height, 0, colors::white, colors::white );

    draw_layers_back();
    draw_shadows();
    draw_links( tree.cntall() ); // recurse
    draw_elems( tree.cntall() ); // recurse
    draw_scode();
    draw_tstamp();
    draw_callparams();
    draw_bitpath_back();
    draw_bitpaths();
    draw_debug();
    draw_ramka();

    svg.end(); }
