#include "global.hpp"


static int svgcalc_ramka_w( int width ) {
	int minw    = iparams.svg_ramka_min_w;
	int dxbrd   = 2 * iparams.svg_ramka_border;             // отступ по бокам
	int r       = dxbrd + (width < minw ? minw : width);    // с учетом минимальной ширины
	return r; }


static int svgcalc_ramka_h( int w , int	layscount ) {

	int r		= iparams.svg_paper_border;
	int x		= iparams.svg_paper_border;
	int capth	= iparams.svg_captionh;
	int spcrh	= iparams.svg_spacer_h;

	// высота заголовка
	int hh_header = iparams.svg_headerh;
	gfxrect_header.set(x, r, w, hh_header);
	r += hh_header;
	r += spcrh;

	// высота слоев
	int			hh_layers	= layscount * iparams.svg_layerh;
	int			hh_shadows	= iparams.svg_shadowh * layscount;
	int			hh_bitfield	= iparams.svg_bitfieldh * 8;

	VHRect		arearect;


	// #1 Caption
	rectCaption1.set(x, r, w, capth);
	r += capth;
	r += spcrh;

	// #1 Tree
	arearect.set(x, r, w, hh_layers);
	layerarea1.set( arearect, layscount );
	r += hh_layers;
	r += spcrh * 2;

	// #1 высота теней
	gfxrect_shadows1.set(x, r, w, hh_shadows);
	r += hh_shadows;
	r += spcrh;

	// #1 высота битовых полей
	gfxrect_bitfield1.set(x, r, w, hh_bitfield );
	r += hh_bitfield;
	r += spcrh;



	// #2 Caption
	rectCaption2.set(x, r, w, capth);
	r += capth;
	r += spcrh;

	// #2 Tree
	arearect.set(x, r, w, hh_layers);
	layerarea2.set( arearect, layscount );
	r += hh_layers;
	r += spcrh * 2;

	// #2 высота теней
	gfxrect_shadows2.set(x, r, w, hh_shadows);
	r += hh_shadows;
	r += spcrh;

	// #2 высота битовых полей
	gfxrect_bitfield2.set(x, r, w, hh_bitfield );
	r += hh_bitfield;
	r += spcrh;


	// #3 Caption
	rectCaption3.set(x, r, w, capth);
	r += capth;
	r += spcrh;

	// #3 Tree
	arearect.set(x, r, w, hh_layers);
	layerarea3.set( arearect, layscount );
	r += hh_layers;
	r += spcrh * 2;

	// #3 высота теней
	gfxrect_shadows3.set(x, r, w, hh_shadows);
	r += hh_shadows;
	r += spcrh;


	// Spectrum
	int hh_spectrum = 256/2;
	gfxrect_spectrum.set(x, r, w, hh_spectrum);
	gfxrect_spectrum.shrink(20, 0);
	r += hh_spectrum;

	r += spcrh;

	r -= iparams.svg_paper_border;

	return r; }


void svgcalc_ramka( int width , int	layscount ) {

	int x = iparams.svg_paper_border;
	int y = iparams.svg_paper_border;
	int w = svgcalc_ramka_w( width );
	int h = svgcalc_ramka_h( w , layscount );

	gfxrect_ramka.set(x, y, w, h );
	
	// Setup final document size
	oparams.svg_width   = gfxrect_ramka.w + (2 * iparams.svg_paper_border);
	oparams.svg_height  = gfxrect_ramka.h + (2 * iparams.svg_paper_border);
}


// return oparams.gfx_ramka_x2 - oparams.gfx_ramka_x1; 
// return oparams.gfx_ramka_y2 - oparams.gfx_ramka_y1;

// Recalculate vertical cut area + header & footer
// oparams.gfx_ramka_height = iparams.svg_def_line_height_ * (tree.depthmax() + 2);


// int minleft     = iparams.svg_width;
// int maxright    = 0;
// int mintop      = iparams.svg_height;
// int maxbot      = 0;

// for(int i=0; i <= tree.cntall(); i++ ) {
//     if( oparams.gfxpos_x[i] < minleft    ) { minleft  = oparams.gfxpos_x[i]; }
//     if( oparams.gfxpos_x[i] > maxright   ) { maxright = oparams.gfxpos_x[i]; }
//     if( oparams.gfxpos_y[i] < mintop     ) { mintop   = oparams.gfxpos_y[i]; }
//     if( oparams.gfxpos_y[i] > maxbot     ) { maxbot   = oparams.gfxpos_y[i]; } }

// while( (maxright - minleft) < 400 ) {
//     minleft     -= 10;
//     maxright    += 10; }

// int dx = 80;
// minleft     -= dx;
// maxright    += dx;
// mintop      -= dx;
// maxbot      += dx;

// copy
// oparams.gfx_ramka_x1    = minleft;
// oparams.gfx_ramka_x2    = maxright;
// oparams.gfx_ramka_y1    = mintop;
// oparams.gfx_ramka_y2    = maxbot;

// int svgcalc_ramka_width     () { return gfx_ramka.w; }
// int svgcalc_ramka_height    () { return gfx_ramka.h; }
