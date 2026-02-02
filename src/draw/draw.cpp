#include "draw.hpp"

#include <ctime>

using namespace std;

// -------------------------------------------------------------------------------------------------
// SVG GFX Related
// -------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------
static void svgrect( const VHRect & rect, int wdt, std::string color) {
	svg.rect( rect.sx, rect.sy, rect.w, rect.h, wdt, color ); }

// -------------------------------------------------------------------------------------------------
int font_align_pixels(std::string str, int fntsize) {
	return str.size() * (fntsize*0.55) / 2; }


// -------------------------------------------------------------------------------------------------
void draw_scode( VHTree & tree , const VHRect & rect ) {

	auto scd = tree.scode();
	std::string str = tree.SCodeToTCode( scd );
	str += " : " + tree.SCodeToText( scd );

	int dd  = 4;

	int w   = rect.w;	// gfxrect_ramka.w/2;
	int x   = rect.sx;	// gfxrect_ramka.sx;
	int y   = rect.sy;	// gfxrect_ramka.sy;

	std::string clrfont = colors::mgray; // colors::nblue;
	std::string clrfone = "#F0F0F0";

	svg.rect(x + dd, y + dd, w - dd*2, rect.h, 1, clrfone, clrfone, 8);
	svg.text(x + 18, y + 32, str, iparams.fntSans, 16, clrfont ); }


// -------------------------------------------------------------------------------------------------
void draw_caption( const VHRect & rect , std::string txt) {
	int x = rect.sx +  24;
	int y = rect.ey - rect.h * 0.3;
	int fnsize = rect.h * 0.3;
	svg.text(x, y, txt, iparams.fntSans, fnsize, colors::lgray ); }

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
	svg.text(x2, y, callParams, fnt, 8, colors::lgray); }


// -------------------------------------------------------------------------------------------------
void draw_debug() {

	std::string color = "#FFAAAA";

	svgrect( layerarea1.rect(), 1, color );
	svgrect( gfxrect_shadows1,	1, color );
	svgrect( gfxrect_bitfield1,	1, color );

	svgrect( layerarea2.rect(), 1, color );
	svgrect( gfxrect_shadows2,	1, color );
	svgrect( gfxrect_bitfield2,	1, color );

	svgrect( layerarea3.rect(), 1, color );
}

// -------------------------------------------------------------------------------------------------
void draw_separator( const VHRect & r) {
	svg.line( r.sx, r.midy(), r.ex, r.midy(), 2, colors::mgray, "5,5" ); }

// -------------------------------------------------------------------------------------------------
void RenderFinalDocument() {

	svg.begin( oparams.svg_width, oparams.svg_height );

	// Full Paper
	svg.rect(0, 0, oparams.svg_width, oparams.svg_height, 0, colors::white, colors::white );

	draw_scode			( tree1 , rectHeader1 );
	draw_caption		( rectCaption1, "1) Сортировка входных данных");
	layerarea1.draw_layers_back();
	draw_links			( tree1 , tree1gfx , tree1.cntall() );
	draw_shadows		( tree1 , tree1gfx , gfxrect_shadows1 );
	draw_elems			( tree1 , tree1gfx , tree1.cntall() , true );
	draw_bitpath_back	( gfxrect_bitfield1 );
	draw_bitpaths		( tree1 , gfxrect_bitfield1 , true );
	draw_separator		( gfxrect_sep1 );

	draw_scode			( tree2 , rectHeader2 );
	draw_caption		( rectCaption2, "2) Трансформация");
	layerarea2.draw_layers_back();
	draw_links			( tree2 , tree2gfx , tree2.cntall() );
	draw_shadows		( tree2 , tree2gfx , gfxrect_shadows2 );
	draw_elems			( tree2 , tree2gfx , tree2.cntall() , true );
	draw_bitpath_back	( gfxrect_bitfield2 );
	draw_bitpaths		( tree2, gfxrect_bitfield2 , true );
	draw_separator		( gfxrect_sep2 );

	draw_scode			( tree3 , rectHeader3 );
	draw_caption		( rectCaption3, "3) Префиксный формат дерева");
	layerarea3.draw_layers_back();
	draw_links			( tree3 , tree3gfx , tree3.cntall() );
	draw_shadows		( tree3 , tree3gfx , gfxrect_shadows3 );
	draw_elems			( tree3 , tree3gfx , tree3.cntall() , false );
	draw_bitpath_back	( gfxrect_bitfield3 );
	draw_bitpaths		( tree3, gfxrect_bitfield3 , false );
	draw_separator		( gfxrect_sep3 );


	draw_tstamp();
	draw_callparams();

	draw_spectrum();
	// draw_debug();
	draw_ramka();

	svg.end(); }
