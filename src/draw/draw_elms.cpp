#include "draw.hpp"

using namespace std;


// -------------------------------------------------------------------------------------------------
static int gfx_offstxt_center( std::string txt, int fsize) {
	int r = txt.size() * fsize * 3 / 10;
	//  ((idx>9) ? (fontw * 3 / 5) : (fontw / 4));
	return r; }

// -------------------------------------------------------------------------------------------------
static void draw_dbg_LW ( VHTreeGfx & tgfx , int idx) {
	int     x           = tgfx.posx(idx) - tgfx.nodewl(idx);
	int     y           = tgfx.posy(idx) - iparams.svg_elm_width/2;
	int     ww          = tgfx.nodewl(idx) + tgfx.nodewr(idx);
	int     th          = iparams.svg_elm_width;
	svg.rect(x, y, ww, th, 1, "black", "orange" ); }

// -------------------------------------------------------------------------------------------------
static void draw_dbg_txt_LW( VHTreeGfx & tgfx , int idx) {
	int     x           = tgfx.posx(idx);
	int     y           = tgfx.posy(idx);
	string  fnt         = iparams.fntSans;

	// X-Pos value
	string str1 = std::to_string( x );
	svg.text(x, y - 30, str1, fnt, 3, "black" );

	// WL WR values
	string strl = std::to_string( tgfx.nodewl(idx) );
	string strr = std::to_string( tgfx.nodewr(idx) );
	string wlwr = strl + ":" + strr;
	svg.text(x, y - 20, wlwr, fnt, 3, "black" ); }

// -------------------------------------------------------------------------------------------------
static void draw_elmform_node( 
	VHTree & tree , int idx,
	int x, int y,
	int elmsize,
	const sColor *  colors ) {

	int		th				= 1;
	int		ra				= iparams.svg_elm_width*5/8;
	bool 	rotated			= tree.getswap(idx);
	std::vector<int> arri	= iparams.param_injected;
	bool		injected	= arri.size() ? ( std::find( arri.begin(), arri.end(), idx) != arri.end() ) : false;
	std::string dot			= injected ? std::string("1,3") : std::string(""); 

	svg.circ(x, y, ra, th, colors->colf, colors->color_shadow_b );

	if(dot.size()) {
		svg.circ(x, y, ra+2, th, colors->colf, "none", dot); }

	if( rotated ) {
		svg.arc(x, y, 14, 4, colors->colf); }

	svg.circ(x, y, elmsize / 2, th, colors->colf, colors->colb);

}

// -------------------------------------------------------------------------------------------------
static void draw_elmform_sym( int idx, int x, int y, int elmsize, const sColor *  colors ) {

	int th = 1;
	int rx = x - elmsize/2, ry = y - elmsize/2;

	// Shadow
	svg.rect( rx + 4, ry + 5,
		elmsize, elmsize, th,
		colors->color_shadow_s,
		colors->color_shadow_b, elmsize * 0.2 );

	// Form
	svg.rect( rx, ry,
		elmsize, elmsize, th,
		colors->colf, colors->colb, elmsize * 0.2 );
}

// -------------------------------------------------------------------------------------------------
static void draw_elm_form ( VHTree & tree , VHTreeGfx & tgfx , int idx, int x, int y , bool separator) {

	bool			isroot		= tree.arch().isroot(idx);
	bool			iscontr		= idx == iparams.param_cntrint;
	bool			issym		= tree.arch().issym (idx);

	int				elmsize     = iparams.svg_elm_width;
	int				r           = elmsize / 2;
	int				th          = 1.5;
	const sColor *	colors      = elm_color( issym, isroot, iscontr );

	if(issym)	{ draw_elmform_sym	( idx, x, y, elmsize, colors ); }
	else		{ draw_elmform_node	( tree , idx, x, y, elmsize, colors ); }
	if(separator) { svg.line( x - r, y, x + r, y, 1, colors->cols); }
}

// -------------------------------------------------------------------------------------------------
static void draw_elm_value( VHTree & tree , VHTreeGfx & tgfx , int idx) {

	VHTreeArch::stobj * pooo    = tree[idx];

	bool			isroot		= tree.arch().isroot(idx);
	bool			iscontr		= idx == iparams.param_cntrint;
	bool			issym		= tree.arch().issym (idx);
	int				fntsz		= 5;
	string			str			= "S" + std::to_string(pooo->v) + ":" + std::to_string(pooo->d);
	int				mdx			= font_align_pixels(str, fntsz);
	int				x			= tgfx.posx(idx) - mdx;
	int				y			= tgfx.posy(idx) + fntsz + 1;
	string			fnt			= iparams.fntSans;
	const sColor *	colors		= elm_color( issym, isroot, iscontr );

	svg.text(x, y, str, fnt, fntsz, colors->coltxt ); }

// -------------------------------------------------------------------------------------------------
void draw_elm( VHTree & tree , VHTreeGfx & tgfx , int idx , bool stat) {

	bool	isroot		= tree.arch().isroot(idx);
	bool	iscontr		= idx == iparams.param_cntrint;
	bool	issym		= tree.arch().issym (idx);

	int     cx          = tgfx.posx(idx);
	int     cy          = tgfx.posy(idx);
	int     qx          = cx - iparams.svg_elm_width/2;
	int     qy          = cy - iparams.svg_elm_width/2;
	int     fontw       = iparams.svg_elm_fntsz * 3 / 4;
	int     fonth       = iparams.svg_elm_fntsz * 4 / 7;
	string  fnt         = iparams.fntSans;

	const sColor * colors = elm_color(issym, isroot, iscontr);

	// Debug staff : Rectangle [WL|WR]
	if( iparams.show_width_elmslr ) { draw_dbg_LW( tgfx , idx); }

	draw_elm_form( tree , tgfx , idx, cx, cy , stat);

	// Index
	std::string		stridx	= std::to_string(idx);
	int				tx		= cx - gfx_offstxt_center( stridx, fontw );
	int				ty		= cy + (fonth / 2);

	// Show counts ?
	if( stat ) {
		ty -= fonth*0.9;
		draw_elm_value( tree , tgfx , idx ); }

	svg.text(tx, ty, stridx, fnt, fontw, colors::gray );

	// Props print: gfx X L:R ( debug )
	if(iparams.show_dbg_xwlwr) {
		draw_dbg_txt_LW( tgfx , idx ); }

}

// -------------------------------------------------------------------------------------------------
void draw_elems( VHTree & tree , VHTreeGfx & tgfx , int idx, bool statmode ) {
	draw_elm( tree , tgfx , idx , statmode );
	if( tree.arch().isnode(idx) ) {
		draw_elems( tree , tgfx , tree.getleft(idx) , statmode );
		draw_elems( tree , tgfx , tree.getrigh(idx) , statmode ); } }

// -- node shadow
// string  colsh   = "#F0FFF0";
// svg.circ(x+2, y+2, ra, th*3/8, colsh, colsh ); // Shadow
