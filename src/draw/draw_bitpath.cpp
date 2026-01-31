#include "draw.hpp"

using namespace std;

#define SYMSINCON 8

static VHRect   rectrows[SYMSINCON];

static int      symsincol    = SYMSINCON;

static int get_line_y( const VHRect & rect, int i) {
	return rect.sy + (i * iparams.svg_bitfieldh); }

// -------------------------------------------------------------------------------------------------
static void draw_bitpath_sym(const VHRect & rect, int idx) {

	int colsmin = tree.arch().cntsyms() / 8;
	if( tree.arch().cntsyms() % 8 ) colsmin++;
	if(!colsmin) colsmin++;

	// ширина столбцов зависит от количества узлов дерева
	int			ww          = gfxrect_ramka.w / colsmin; 

	int			coln        = idx / symsincol;
	int			rown        = idx % symsincol;

	int			symx        = gfxrect_ramka.sx + 40 + (coln * ww);
	int			yoffstxt	= iparams.svg_bitfieldh - iparams.svg_bitfieldh*0.25;
	int			symy        = get_line_y( rect , rown) + yoffstxt;

	string		strsymn     = "#" + std::to_string(idx); // + ":";
	string		bitpath     = tree.bitpath(idx);

	string color = colors::gray;
	svg.text( symx      , symy, strsymn, iparams.fntSans,  9, color);
	svg.text( symx + 50 , symy, bitpath, iparams.fntSans, 10, color);

	if(iparams.from_spectrum) {
	string      bitrate     = tree.symrate(idx);
	svg.text( symx + 20 , symy, bitrate, iparams.fntSans, 7, "#A0A0A0"); }
}

// -------------------------------------------------------------------------------------------------
void draw_bitpaths( const VHRect & rect ) {
	for(int i=0; i < tree.arch().cntsyms();i++) { draw_bitpath_sym( rect , i ); } }

// -------------------------------------------------------------------------------------------------
void draw_bitpath_backline(const VHRect & rect , int rown, std::string fcol) {
	int dx = 2;
	int x = gfxrect_ramka.sx + dx;
	int y = get_line_y(rect, rown);
	svg.rect( x, y, gfxrect_ramka.w - dx*2, iparams.svg_bitfieldh, 1, fcol, fcol ); }

// -------------------------------------------------------------------------------------------------
void draw_bitpath_back( const VHRect & rect ) {
	for(int i=0; i<SYMSINCON; i++) {
		std::string bcol = (i%2)? "#FAFAFA" : "#F4F4F4";
		draw_bitpath_backline(rect , i, bcol); } }
