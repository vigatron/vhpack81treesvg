#include "draw.hpp"

#include <algorithm>
#include <functional>

#define X_OFFS  40


static VHRect rects[4];


static void split_rects() {
	
	int x   = gfxrect_spectrum.sx;
	int y   = gfxrect_spectrum.sy;
	int w   = gfxrect_spectrum.w;
	int h   = gfxrect_spectrum.h;
	int dx  = w/4;

	for(int i=0; i<4; i++) {
		VHRect * p = & rects[i];
		p->set(x + i*dx, y, dx, h);
		p->shrink(10, 10);
		/* svg.rect( p->sx, p->sy, p->w, p->h, 1, "red" ); */ }
}


static void draw_axis( int x, int y, int xl, int yl) {

	std::string col_axis = "gray";

	// int x2  = x + xl;
	// int y2  = y - yl;
	// svg.line ( x, y, x2, y , 1, col_axis);  // X-Axis
	// svg.line ( x, y, x , y2, 1, col_axis);  // Ys-Axis

	svg.rect( x, y - yl, xl, yl, 1, "#E0E0E0");
}

static void draw_spc( int xo, int yo, int ww, int hh, std::vector<int> arr, int tp) {

	std::string     fname       = iparams.fntSans;
	int             fsize       = 7;
	std::string     fcol        = "gray";
	std::string     col_sep     = "#B0B0B0";
	int             th = 1;

	draw_axis( xo, yo, ww, hh);

	std::vector<int>::iterator minv_it = std::min_element(arr.begin(), arr.end());
	std::vector<int>::iterator maxv_it = std::max_element(arr.begin(), arr.end());

	int minv = * minv_it;
	int maxv = * maxv_it;

	// Draw max marker
	int ymax = yo - hh * 0.8;
	svg.line( xo, ymax, xo + ww, ymax, 1, col_sep, "3,3" );

	// quarter vertical separator

	if(tp == 1) {
		int wx = ww/2, hy = hh * 0.8;
		int qcx = xo + ww/4;
		int qcy = yo - hy/2;
		// svg.line(qcx, yo, qcx, yo - hy, th, col_sep, "1,5");
		std::string colb = "#F4F4F4";
		svg.rect(xo + 1, yo - hy, ww/4 - 1, hy, th, colb, colb ); }

	float dx = ww * 1.0f / arr.size();

	for(int i = 0; i < arr.size() ; i++ ) {
		int     x           = xo + int(i * dx);
		int     elmh        = hh * 0.8 * arr[i] / maxv;
		int     sy          = yo - elmh;
		bool    fhalf       = i < arr.size() / 2;
		bool    flagcolor   =  !tp ? false : ( fhalf ? true : false );
		std::string colorf   = !flagcolor ? "#9090F0" : "#B0B0B0";
		std::string colorb   = !flagcolor ? "#D0D0FF" : "#C0C0C0";
		svg.rect( x, sy, int(dx), elmh, 1, colorf, colorb ); }

	std::string minmax = "min: " + std::to_string(minv) + " / max: " + std::to_string(maxv);
	svg.text( xo + 4, ymax - 4, minmax, fname, fsize*1.5, fcol );

	// Show zero
	int cx = xo + ww/2 - 1 - dx/2;
	int yords   = yo + fsize * 1.5;
	svg.text( (!tp ? xo: cx) , yords, "0", fname, fsize, fcol);

	// Show max
	{   int xmax = xo + ww - fsize*1.5;
		std::string txtmax = std::to_string( arr.size() / (!tp ? 1 : 2) );
		svg.text( xmax , yords, txtmax, fname, fsize, fcol ); }

	if(tp == 1) { draw_figures( xo, yo, ww, hh ); }
}


void draw_spectrum() {

	split_rects();

	if(!iparams.param_spcints.size()) return;

	std::vector<int>    arr1 = iparams.param_spcints;
	std::vector<int>    arrd = arr1;	// descent
	std::vector<int>    arrg = arr1;	// grow
	std::vector<int>    arrs;			// combined

	std::sort(arrd.begin(), arrd.end());
	std::sort(arrg.begin(), arrg.end(), std::greater<int>());

	arrs.insert(arrs.end(), arrd.begin(), arrd.end());
	arrs.insert(arrs.end(), arrg.begin(), arrg.end());

	{ VHRect * r = & rects [0]; draw_spc( r->sx, r->ey, r->w, r->h, arr1, 0 ); } // Draw original
	{ VHRect * r = & rects [1]; draw_spc( r->sx, r->ey, r->w, r->h, arrs, 1 ); } // Draw sorted

}

	// for(int i = 0; i < arr2.size() ; i++ ) {
	//     int x   = gfxrect_spectrum.sx + X_OFFS + (64 + arr2.size() + i)*4;
	//     int hh  = arr2[i];
	//     int sy  = gfxrect_spectrum.ey - hh - 128;
	//     svg.rect( x, sy, 2, hh, 1, "blue" ); }

	// // Draw sorted
	// for(int i = 0; i < arr3.size() ; i++ ) {
	//     int x   = gfxrect_spectrum.sx + X_OFFS + (64 + i)*4;
	//     int hh  = arr3[i];
	//     int sy  = gfxrect_spectrum.ey - hh - 128;
	//     svg.rect( x, sy, 2, hh, 1, "gray" ); }
