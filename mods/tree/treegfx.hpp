#pragma once

#include "vhplatform.hpp"
#include "tree.hpp"
#include "layerarea.hpp"


class VHTreeGfx {

	public:

		// -------------------------------------------------------------------------------------------------
		// Recurse calculate nodes widths pxls ( L & R )
		// -------------------------------------------------------------------------------------------------
		void CalculateTreeGfx( VHTree & tree , int node_xdist, int node_spacr) {
			svg_node_xdist = node_xdist;
			svg_node_spacr = node_spacr;
			calc_w( tree , tree.rootidx(), & wl, & wr); }

		int posx(int idx)	{ return gfxpos_x[idx]; }
		int posy(int idx)	{ return gfxpos_y[idx]; }
		int wdtl()			{ return wl; }
		int wdtr()			{ return wr; }
		int wdt ()			{ return wl + wr; }
		int nodewl(int idx)	{ return gfx_nodewl[idx]; }
		int nodewr(int idx) { return gfx_nodewr[idx]; }

		// -------------------------------------------------------------------------------------------------
		void svgcalc_set_coords(
			const VHRect & rect,
			VHTree & tree,
			LayerArea & layerarea,
			int idx, int cx, int layscnt) {

			gfxpos_x[idx] = cx;
			gfxpos_y[idx] = layerarea.layer_posyc( rect, tree.getlay(idx), layscnt);

			if( tree.arch().isnode( idx ) ) {
				int idxl = tree.getleft(idx);
				int lnodex = cx - gfx_nodewr[ idxl ];
				svgcalc_set_coords( rect, tree, layerarea, idxl, lnodex, layscnt );

				int idxr = tree.getrigh(idx);
				int rnodex = cx + gfx_nodewl[ idxr ];
				svgcalc_set_coords( rect, tree, layerarea, idxr, rnodex, layscnt ); } }

	private:

		int		wl, wr;

		int		gfxpos_x    [256];		// X-Координата узла
		int		gfxpos_y    [256];		// Y-Координата узла
		int		gfx_nodewl  [256];		// Ширина левой  ветки
		int		gfx_nodewr  [256];		// Ширина правой ветки

		int		svg_node_xdist;
		int		svg_node_spacr;

		int		ws() { int ws = svg_node_xdist + svg_node_spacr; return ws; }

		// -------------------------------------------------------------------------------------------------
		void calc_w( VHTree & tree , int idx, int * retwl, int * retwr) {

			int wl, wr;

			int idxl = tree.getleft(idx);
			if( tree.arch().issym(idxl) ) {
				gfx_nodewl[idx ] = ws();
				gfx_nodewl[idxl] = ws() / 2;
				gfx_nodewr[idxl] = ws() / 2;
			} else {
				calc_w( tree , idxl, &wl, &wr);
				gfx_nodewl[idx] = wl + wr; }

			int idxr = tree.getrigh(idx);
			if( tree.arch().issym(idxr) ) {
				gfx_nodewr[idx ] = ws();
				gfx_nodewl[idxr] = ws() / 2;
				gfx_nodewr[idxr] = ws() / 2;
			} else {
				calc_w( tree , idxr, &wl, &wr);
				gfx_nodewr[idx]  = wl + wr; }

			*retwl = gfx_nodewl[idx];
			*retwr = gfx_nodewr[idx];
			printf("calc_w W%2d  (L:%d R:%d) S:%d\n", idx, *retwl, *retwr, *retwl + *retwr); }


};
