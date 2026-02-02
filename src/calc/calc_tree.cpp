#include "global.hpp"
#include "calc.hpp"

// recurse, start from root
void CalcTreeGfx(
	const VHRect & rect,
	VHTree & tree ,
	VHTreeGfx & tgfx ,
	LayerArea & larea,
	int layscount ) {

	//  Centering root and start recursive X/Y enumeration
	int ramka_centerx   = gfxrect_ramka.sx + (gfxrect_ramka.w/2);
	int root_width      = tgfx.wdt();
	int cx              = ramka_centerx + root_width/2 - tgfx.wdtr();
	tgfx.svgcalc_set_coords( rect, tree, larea , tree.rootidx() , cx , layscount );
}
