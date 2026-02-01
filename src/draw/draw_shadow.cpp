#include "draw.hpp"

using namespace std;

void draw_shadows( VHTree & tree , VHTreeGfx & tgfx , VHRect & rect ) {

	int idx = tree.rootidx();

	while( tree.arch().isnode(idx) ) {

		int i = tree.arch().getlay( idx );
		int x = tgfx.posx(idx) , y = tgfx.posy(idx);

		int     yy          = rect.sy + i * iparams.svg_shadowh;
		int     wl          = tgfx.nodewl(idx) - iparams.svg_elm_width/2;
		int     wr          = tgfx.nodewr(idx) - iparams.svg_elm_width/2;

		svg.line( x, y, x, yy, 1, "#D0E0D0", "1,8");
		svg.line( x - wl, yy, x + wr, yy, 1, "#E8E8E8" );
		svg.circ( x, yy, 1, 1, "#E0E0E0", "none");
		idx--;
	}
}