#include "draw.hpp"

using namespace std;

void draw_shadows() {

	int idx = tree.rootidx();

	while( tree.arch().isnode(idx) ) {

		int     i           = tree.arch().getlay( idx );
		int     x           = oparams.gfxpos_x[idx];
		int     y           = oparams.gfxpos_y[idx];
		int     yy          = gfxrect_shadows1.sy + i * iparams.svg_shadowh;
		int     wl          = oparams.gfx_nodewl[idx] - iparams.svg_elm_width/2;
		int     wr          = oparams.gfx_nodewr[idx] - iparams.svg_elm_width/2;

		svg.line( x, y, x, yy, 1, "#D0E0D0", "1,8");
		svg.line( x - wl, yy, x + wr, yy, 1, "#E8E8E8" );
		svg.circ( x, yy, 1, 1, "#E0E0E0", "none");
		idx--;
	}
}