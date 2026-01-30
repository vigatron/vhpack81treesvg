#include "draw.hpp"

using namespace std;



static int gfx_offstxt_center( std::string txt, int fsize) {
	int r = txt.size() * fsize * 3 / 10;
	//  ((idx>9) ? (fontw * 3 / 5) : (fontw / 4));
	return r; }

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
static void draw_elmform_node( int idx, int x, int y, int elmsize, const sColor *  colors ) {

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
static void draw_elm_form (int idx, int x, int y) {

	bool			isroot		= tree.arch().isroot(idx);
	bool			iscontr		= idx == iparams.param_cntrint;
	bool			issym		= tree.arch().issym (idx);

	int				elmsize     = iparams.svg_elm_width;
	int				r           = elmsize / 2;
	int				th          = 1.5;
	const sColor *	colors      = elm_color( issym, isroot, iscontr );
	bool			separator	= iparams.from_spectrum;

	if(issym)	{ draw_elmform_sym	( idx, x, y, elmsize, colors ); }
	else		{ draw_elmform_node	( idx, x, y, elmsize, colors ); }

	if(separator) {
		svg.line( x - r, y, x + r, y, 1, colors->cols); }
}

// -------------------------------------------------------------------------------------------------
static void draw_elm_value(int idx) {

	VHTreeArch::stobj * pooo    = tree[idx];

	bool			isroot		= tree.arch().isroot(idx);
	bool			iscontr		= idx == iparams.param_cntrint;
	bool			issym		= tree.arch().issym (idx);

	int             fntsz   = 5;
	string          str     = "S" + std::to_string(pooo->v) + ":" + std::to_string(pooo->d);
	int             mdx     = font_align_pixels(str, fntsz);
	int             x       = oparams.gfxpos_x[idx] - mdx;
	int             y       = oparams.gfxpos_y[idx] + fntsz + 1;
	string          fnt     = iparams.fntSans;
	
	
	const sColor *  colors  = elm_color( issym, isroot, iscontr );

	svg.text(x, y, str, fnt, fntsz, colors->coltxt ); }

// -------------------------------------------------------------------------------------------------
void draw_elm(int idx) {

	bool	isroot		= tree.arch().isroot(idx);
	bool	iscontr		= idx == iparams.param_cntrint;
	bool	issym		= tree.arch().issym (idx);

	int     cx          = oparams.gfxpos_x[idx];
	int     cy          = oparams.gfxpos_y[idx];
	int     qx          = oparams.gfxpos_x[idx] - iparams.svg_elm_width/2;
	int     qy          = oparams.gfxpos_y[idx] - iparams.svg_elm_width/2;
	int     fontw       = iparams.svg_elm_fntsz * 3 / 4;
	int     fonth       = iparams.svg_elm_fntsz * 4 / 7;
	string  fnt         = iparams.fntSans;

	const sColor * colors = elm_color(issym, isroot, iscontr);

	// Debug staff : Rectangle [WL|WR]
	if( iparams.show_width_elmslr ) { draw_dbg_LW(idx); }

	draw_elm_form(idx, cx, cy);

	// Index
	std::string		stridx	= std::to_string(idx);
	int				tx		= oparams.gfxpos_x[idx] - gfx_offstxt_center( stridx, fontw );
	int				ty		= oparams.gfxpos_y[idx] + (fonth / 2);

	// Show counts ?
	if(iparams.from_spectrum) { ty -= fonth*0.9; draw_elm_value(idx); }

	svg.text(tx, ty, stridx, fnt, fontw, colors::gray );

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
static bool isbind( int idx1, int idx2 ) {

    std::vector<int> arr = iparams.param_svzints;

    std::vector<int>::iterator it1 = std::find(arr.begin(), arr.end(), idx1);
    if( it1 == arr.end() ) return false;

    std::vector<int>::iterator it2 = std::find(arr.begin(), arr.end(), idx2);
    if( it2 == arr.end() ) return false;

    int pos1 = std::distance(arr.begin(), it1);
    int pos2 = std::distance(arr.begin(), it2);

    return pos2 +1 == pos1;
}

// -------------------------------------------------------------------------------------------------
void draw_link(int idx1, int idx2) {

    bool bindflag = isbind( idx1, idx2);

    int x1 = oparams.gfxpos_x[idx1], y1 = oparams.gfxpos_y[idx1];
    int x2 = oparams.gfxpos_x[idx2], y2 = oparams.gfxpos_y[idx2];

    int wdt = bindflag ? iparams.svg_lnkwidth2 : iparams.svg_lnkwidth;

    svg.line(x1, y1, x2, y2, wdt, colors::mgreen);

    if(bindflag) {
        int s = 3;
        int wx = x2 - x1; if(wx < 0) wx *= -1;
        int wy = y2 - y1; if(wy < 0) wy *= -1;
        int kx = (wx > wy) ? 0:1;
        int ky = (wx > wy) ? 1:0;

        x1 -= s * kx * 1; x2 -= s * kx * 1; y1 -= s*ky*1; y2 -= s*ky*1;
        svg.line(x1, y1, x2, y2, wdt, colors::mgreen);

        x1 += s * kx * 2; x2 += s * kx * 2; y1 += s*ky*2; y2 += s*ky*2;
        svg.line(x1, y1, x2, y2, wdt, colors::mgreen); }
}


// -------------------------------------------------------------------------------------------------
void draw_links(int idx) {
    if(idx >= tree.arch().cntsyms() ) {
        draw_link(idx, tree.getleft(idx));
        draw_link(idx, tree.getrigh(idx));
        draw_links(tree.getleft(idx));
        draw_links(tree.getrigh(idx)); } }

// -- node shadow
// string  colsh   = "#F0FFF0";
// svg.circ(x+2, y+2, ra, th*3/8, colsh, colsh ); // Shadow
