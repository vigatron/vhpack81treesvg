#include "draw.hpp"


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
void draw_link( VHTreeGfx & tgfx , int idx1, int idx2) {

	bool bindflag = isbind( idx1, idx2);

	int x1 = tgfx.posx(idx1), y1 = tgfx.posy(idx1);
	int x2 = tgfx.posx(idx2), y2 = tgfx.posy(idx2);

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
void draw_links( VHTree & tree , VHTreeGfx & tgfx , int idx) {
	if(idx >= tree.arch().cntsyms() ) {
		draw_link( tgfx , idx, tree.getleft(idx));
		draw_link( tgfx , idx, tree.getrigh(idx));
		draw_links( tree , tgfx , tree.getleft(idx));
		draw_links( tree , tgfx , tree.getrigh(idx)); } }
