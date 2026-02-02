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

		// вертикальная тень
		svg.line( x, y, x, yy, 1, "#D0E0D0", "1,8");

		// горизонталь
		svg.line( x - wl, yy, x + wr, yy, 2, "#E8E8E8" );

		// узел
		svg.circ( x, yy, 1, 1, "#D0D0D0", "none");

		//
		int qw = 30;
		int yq = rect.sy - 1.6f * qw;

		std::string strk = colors::sgray; // "gray"

		svg.rect( x - qw/2, yq, qw, qw, 1, strk, "white" , 4 );
		
		idx--; }
}