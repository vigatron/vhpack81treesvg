#include "draw.hpp"

void draw_figures( int xo, int yo, int ww, int hh ) {

	int wx	= ww/2, hy = hh * 0.8;
	int qcx	= xo + ww/4;
	int qcy	= yo - hy/2;
	int cx	= xo + ww/2;
	int th	= 1;

	std::string figcol = "#E0E0E0";
	std::string strdot = "2,1";

	svg.path(cx, yo-hy, wx, hy, xo, yo, 0, th, figcol, strdot );			// figure 1
	svg.line(cx, yo-hy, xo, yo, th, figcol, "1,3");							// figure 2
	svg.path(cx, yo-hy, wx, hy, xo, yo, 1, th, figcol, strdot );			// figure 3

	svg.path(qcx, qcy, wx/2, hy/2, xo, yo, 1, th, figcol, strdot );			// figure S1
	svg.path(qcx, qcy, wx/2, hy/2, cx, yo - hy, 1, th, figcol, strdot );

	svg.path(qcx, qcy, wx/2, hy/2, xo, yo, 0, th, figcol, strdot );			// figure S2
	svg.path(qcx, qcy, wx/2, hy/2, cx, yo - hy, 0, th, figcol, strdot );

	int ymid = yo-hy/2;
	svg.line(xo, ymid, cx, ymid, th, figcol, "1,3" );

}
