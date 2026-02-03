#include "draw.hpp"

using namespace std;


// -----------------------------------------------------------------------------

static void draw_prop ( const VHRect & rect, VHTree & tree , VHTreeGfx & tgfx, int idx ) {

	// Text props
	int fntsz	= 10;
	std::string props = "0:0";
	int fdx		= font_align_pixels(props, fntsz);
	int tx		= rect.sx + rect.w/2 - fdx + 1; //  - fdx;
	int ty		= rect.sy + fntsz + 1;

	svg.text( tx, ty, props, iparams.fntSans, fntsz, "#C0C0C0" );
}

// -----------------------------------------------------------------------------

void draw_shadows( VHTree & tree , VHTreeGfx & tgfx , VHRect & rect ) {

	int idx = tree.rootidx();

	while( tree.arch().isnode(idx) ) {

		int i = tree.arch().getlay( idx );
		int x = tgfx.posx(idx) , y = tgfx.posy(idx);

		int     yy          = rect.sy + i * iparams.svg_shadowh;
		int     wl          = tgfx.nodewl(idx) - iparams.svg_elm_width/2;
		int     wr          = tgfx.nodewr(idx) - iparams.svg_elm_width/2;

		// вертикальная тень
		svg.line( x, y, x, yy, 1, "#D0E0D0", "1,8");

		// горизонталь
		svg.line( x - wl, yy, x + wr, yy, 2, "#E8E8E8" );

		// узел
		svg.circ( x, yy, 1, 1, "#D0D0D0", "none");

		//
		int qw = 30;
		std::string strk = colors::sgray; // "gray"
		int qx = x - qw/2;
		int qy = rect.sy - 1.3f * qw;
		svg.rect( qx, qy, qw, qw/2, 1, strk, "white" , 4 );

		VHRect proprect( qx, qy, qw, qw/2 );
		draw_prop( proprect , tree , tgfx, idx );

		idx--; }
}