#include "draw.hpp"

using namespace std;

#define SYMSINCON 8

static VHRect   rectrows[SYMSINCON];
static int      symsincol    = SYMSINCON;

static int get_line_y( const VHRect & rect, int i) {
	return rect.sy + (i * iparams.svg_bitfieldh); }

// -------------------------------------------------------------------------------------------------
static void draw_bitpath_sym( VHTree & tree , const VHRect & rect, int idx , bool showrate) {

	int colsmin = tree.arch().cntsyms() / 8;
	if( tree.arch().cntsyms() % 8 ) colsmin++;
	if(!colsmin) colsmin++;

	// ширина столбцов зависит от количества узлов дерева
	int			ww			= rect.w / colsmin; 
	int			coln		= idx / symsincol;
	int			rown		= idx % symsincol;
	int			symx		= rect.sx + 20 + (coln * ww);
	int			yoffstxt	= iparams.svg_bitfieldh - iparams.svg_bitfieldh*0.25;
	int			symy		= get_line_y( rect , rown) + yoffstxt;
	string		strsymn		= "#" + std::to_string(idx); // + ":";
	string		bitpath		= tree.bitpath(idx);
	string		color		= colors::gray;

	svg.text( symx      , symy, strsymn, iparams.fntSans,  9, color);
	svg.text( symx + 50 , symy, bitpath, iparams.fntSans, 10, color);

	if( showrate ) {
	string bitrate     = tree.symrate(idx);
	svg.text( symx + 20 , symy, bitrate, iparams.fntSans, 7, "#A0A0A0");
	}
}

// -------------------------------------------------------------------------------------------------
void draw_bitpaths( VHTree & tree , const VHRect & rect , bool showrate ) {
	for(int i=0; i < tree.arch().cntsyms();i++) {
		draw_bitpath_sym( tree , rect , i , showrate ); } }

// -------------------------------------------------------------------------------------------------
void draw_bitpath_backline(const VHRect & rect , int rown, std::string fcol) {
	int dx = 2;
	int x = rect.sx + dx;
	int y = get_line_y(rect, rown);
	svg.rect( x, y, rect.w - dx*2, iparams.svg_bitfieldh, 1, fcol, fcol ); }

// -------------------------------------------------------------------------------------------------
void draw_bitpath_back( const VHRect & rect ) {
	for(int i=0; i<SYMSINCON; i++) {
		std::string bcol = (i%2)? "#F8F8F8" : "#F4F4F4";
		draw_bitpath_backline(rect , i, bcol); } }
