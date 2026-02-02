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

	svgrect( treearea[2].rectTree, 1, color );
	svgrect( treearea[2].rectShadows, 1, color );
	svgrect( treearea[2].rectBitfield,	1, color );
	svgrect( treearea[2].rectDescript,	1, color );

}

// -------------------------------------------------------------------------------------------------
void draw_separator( const VHRect & r) {
	svg.line( r.sx, r.midy(), r.ex, r.midy(), 2, colors::mgray, "5,5" ); }

// -------------------------------------------------------------------------------------------------
void RenderFinalDocument( int layscount ) {

	svg.begin( oparams.svg_width, oparams.svg_height );

	// Full Paper
	svg.rect(0, 0, oparams.svg_width, oparams.svg_height, 0, colors::white, colors::white );

	{
		TreeArea & a = treearea[0];
		layerarea1.draw_layers_back( a.rectTree, layscount );

		draw_scode			( tree1 , a.rectHeader );
		draw_caption		( a.rectCaption, "1) Сортировка входных данных");
		draw_links			( tree1 , tree1gfx , tree1.cntall() );
		draw_shadows		( tree1 , tree1gfx , a.rectShadows );
		draw_elems			( tree1 , tree1gfx , tree1.cntall() , true );
		draw_bitpath_back	( a.rectBitfield );
		draw_bitpaths		( tree1 , a.rectBitfield , true );
		draw_separator		( a.rectSeparator );
	}

	{
		TreeArea & a = treearea[1];
		layerarea2.draw_layers_back( a.rectTree, layscount );

		draw_scode			( tree2 , a.rectHeader );
		draw_caption		( a.rectCaption, "2) Трансформация");
		draw_links			( tree2 , tree2gfx , tree2.cntall() );
		draw_shadows		( tree2 , tree2gfx , a.rectShadows );
		draw_elems			( tree2 , tree2gfx , tree2.cntall() , true );
		draw_bitpath_back	( a.rectBitfield );
		draw_bitpaths		( tree2, a.rectBitfield , true );
		draw_separator		( a.rectSeparator );
	}

	{
		TreeArea & a = treearea[2];
		layerarea3.draw_layers_back( a.rectTree, layscount );

		draw_scode			( tree3 , a.rectHeader );
		draw_caption		( a.rectCaption, "3) Префиксный формат дерева");
		draw_links			( tree3 , tree3gfx , tree3.cntall() );
		draw_shadows		( tree3 , tree3gfx , a.rectShadows );
		draw_elems			( tree3 , tree3gfx , tree3.cntall() , false );
		draw_bitpath_back	( a.rectBitfield );
		draw_bitpaths		( tree3, a.rectBitfield , false );
		draw_separator		( a.rectSeparator );
	}

	draw_tstamp();
	draw_callparams();

	draw_spectrum();
	// draw_debug();
	draw_ramka();

	svg.end(); }
