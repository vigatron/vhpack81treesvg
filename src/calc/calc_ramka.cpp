#include "global.hpp"

// -----------------------------------------------------------------------------
static int svgcalc_ramka_w( int width ) {
	int minw    = iparams.svg_ramka_min_w;
	int dxbrd   = 2 * iparams.svg_ramka_border;             // отступ по бокам
	int r       = dxbrd + (width < minw ? minw : width);    // с учетом минимальной ширины
	return r; }

// -----------------------------------------------------------------------------
static void splitrecth( const VHRect r, VHRect & r1, VHRect & r2 , int dx = 0 ) {
	r1.set	( r.sx + dx, r.sy, r.w/2 - dx*2, r.h );
	r2.set	( r.sx + r.w/2 + dx, r.sy, r.w/2 - dx*2, r.h); }

// -----------------------------------------------------------------------------
static void splitrectv( const VHRect r, VHRect & r1, VHRect & r2 , int dy = 0 ) {
	r1.set	( r.sx, r.sy + dy, r.w, r.h / 2 - 2*dy );
	r2.set	( r.sx, r.sy + r.h / 2 + dy, r.w, r.h / 2 - 2 * dy ); }

// -----------------------------------------------------------------------------
static int svgcalc_treearea( TreeArea & treearea , int r , int w , int	layscount ) {

	int		x			= iparams.svg_paper_border;
	int		spcrh		= iparams.svg_spacer_h;
	int		hh_capth	= iparams.svg_captionh;				// высота заголовка
	int		hh_layers	= layscount * iparams.svg_layerh;	// высота слоев
	int		xdx			= 12;
	int		hh_shadows	= iparams.svg_shadowh * layscount;
	int		hh_bitfield	= iparams.svg_bitfieldh * 8;

	// -----------------------------------------------------------------------------

	// #1 Header + Caption
	{
		VHRect srcrect( x, r, w, hh_capth );
		VHRect rightrect;

		splitrecth ( srcrect , treearea.rectHeader , rightrect, xdx );
		splitrectv ( rightrect , treearea.rectCaptionLine1 , treearea.rectCaptionLine2 );
	}

	r += hh_capth; r += spcrh * 2;

	// #1 Tree
	treearea.rectTree.set(x, r, w, hh_layers); r += hh_layers;
	r += spcrh * 2;

	// #1 высота теней
	r += 2 * spcrh;
	treearea.rectShadows.set(x, r, w, hh_shadows); r += hh_shadows;
	r += spcrh;

	// #1 высота битовых полей
	{	VHRect rect( x, r, w, hh_bitfield );
		splitrecth( rect, treearea.rectBitfield , treearea.rectDescript, xdx ); }

	r += hh_bitfield; r += spcrh;

	return r;
}

// -----------------------------------------------------------------------------
static int svgcalc_ramka_h( int w , int	layscount ) {

	int		r		= iparams.svg_paper_border;
	int		x		= iparams.svg_paper_border;
	int		spcrh	= iparams.svg_spacer_h;

	r += spcrh;

	// -----------------------------------------------------------------------------
	r = svgcalc_treearea( treearea[0] , r , w , layscount );
	treearea[0].rectSeparator.set( x, r, w , spcrh ); r += spcrh*2;

	r = svgcalc_treearea( treearea[1] , r , w , layscount );

	if( iparams.genmode > 1 )
		treearea[1].rectSeparator.set( x, r, w , spcrh ); r += spcrh*2;

	if( iparams.genmode == 3 ) {
		r = svgcalc_treearea( treearea[2] , r , w , layscount );
		treearea[2].rectSeparator.set( x, r, w , spcrh ); r += spcrh*2; }

	// Spectrum
	if( iparams.genmode > 1 ) {
		int hh_spectrum = 256/2;
		gfxrect_spectrum.set(x, r, w, hh_spectrum);
		gfxrect_spectrum.shrink(20, 0);
		r += hh_spectrum; }

	r += spcrh;

	r -= iparams.svg_paper_border;

	return r; }

// -----------------------------------------------------------------------------
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
