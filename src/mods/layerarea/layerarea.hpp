#pragma once

#include "vhrect.hpp"
#include "tree.hpp"
#include "svg.hpp"


class LayerArea {

	public:

		LayerArea() { }

		// -------------------------------------------------------------------------------------------------
		int layer_posy (const VHRect & r , int layn, int cntlays) const {
			int layerhh	= r.h / cntlays;
			return r.sy + (layn * layerhh); }

		// -------------------------------------------------------------------------------------------------
		int layer_posyc(const VHRect & r , int layn, int cntlays) const	{
			int layerhh	= r.h / cntlays;
			return layer_posy( r, layn, cntlays) + layerhh / 2; }

		// -------------------------------------------------------------------------------------------------
		void draw_layers_nrs( const VHRect & r , int cntlays ) {
			std::string fntSans = "sans-serif";
			int layerhh	= r.h / cntlays;
			for(int i=0; i < cntlays; i++) {
				int cx = r.sx + 18;
				int cy = layer_posyc( r, i, cntlays);
				svg.circ(cx + 4, cy, layerhh *0.4, 1, colors::nyell, colors::myell );
				svg.text(cx, cy + 2, "L" + std::to_string(i), fntSans, 9, colors::lgray ); } }

		// -------------------------------------------------------------------------------------------------
		void draw_layers_back( const VHRect & r , int cntlays ) {
			int layerhh	= r.h / cntlays;
			// Draw layers back
			for(int ll=0; ll < cntlays; ll++) {
				int x = r.sx;
				int y = layer_posy(r, ll, cntlays);
				std::string color = (ll & 1) ? colors::yellowll : colors::yellowl;
				svg.rect(x, y, r.w, layerhh, 0, color, color );

				int ymid = y + layerhh / 2;
				std::string clrm = (ll & 1) ? "#f8f0dd" : "#f4ebc3";
				svg.line(x, ymid, x + r.w, ymid, 1, clrm, "15,10");
			}

			draw_layers_nrs( r, cntlays); }

	private:

		// int			_layerhh;
		// int			_layerscnt;


};

