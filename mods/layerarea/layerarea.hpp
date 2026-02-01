#pragma once

#include "vhrect.hpp"
#include "tree.hpp"
#include "svg.hpp"


class LayerArea {

	public:

		LayerArea() { }

		void set( const VHRect & r , int layers ) {
			_rect		= r;
			_layerscnt	= layers;
			_layerhh	= _rect.h / _layerscnt; }

		int layer_posy (int layn)		{ return _rect.sy + (layn * layerh()); }
		int layer_posyc(int layn)		{ return layer_posy(layn) + layerh() / 2; }
		int layerh()					{ return _layerhh; }
		const VHRect & rect() const		{ return _rect; }


		// -------------------------------------------------------------------------------------------------
		void draw_layers_nrs() {

			for(int i=0; i < _layerscnt; i++) {
				int cx = _rect.sx + 18;
				int cy = layer_posyc(i);
				svg.circ(cx + 4, cy, _layerhh *0.4, 1, colors::nyell, colors::myell );
				svg.text(cx, cy + 2, "L" + std::to_string(i), fntSans, 9, colors::lgray ); } }

		// -------------------------------------------------------------------------------------------------
		void draw_layers_back() {

			// Draw layers back
			for(int ll=0; ll < _layerscnt; ll++) {
				int x = _rect.sx;
				int y = layer_posy(ll);
				std::string color = (ll & 1) ? colors::yellowll : colors::yellowl;
				svg.rect(x, y, _rect.w, _layerhh, 0, color, color );

				int ymid = y + _layerhh / 2;
				std::string clrm = (ll & 1) ? "#f8f0dd" : "#f4ebc3";
				svg.line(x, ymid, x + _rect.w, ymid, 1, clrm, "15,10");
			}

			draw_layers_nrs(); }

	private:

		VHRect		_rect;
		int			_layerhh;
		int			_layerscnt;

		std::string fntSans = "sans-serif";

};

