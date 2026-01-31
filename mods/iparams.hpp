#pragma once

#include "vhplatform.hpp"

// Input : Configuration Parameters
class TreeGenParamsIn {

	public:

		std::string			param_tcode;

		std::string			param_partn;
		std::string			param_blockn;

		std::vector<int>	param_spcints;		//
		std::vector<int>	param_rotints;		//
		std::vector<int>	param_svzints;		//
		std::vector<int>	param_injected;		//
		int					param_cntrint;		// Container idx
		int					param_mix;			// Mixed mode

		int	optAutoRotate;

		// расстояние между сегментами
		int     svg_spacer_h	= 16;

		// Vertical segments height
		int		svg_headerh		= 56;
		int		svg_captionh	= 38;
		int		svg_layerh		= 36;
		int		svg_shadowh		=  3;
		int		svg_bitfieldh	= 14;

		// Elms GFX props
		int		svg_elm_width	= 26;
		int		svg_elm_fntsz	= 12;

		int		svg_node_xdist	= svg_elm_width;
		int		svg_node_spacr	= svg_node_xdist/2;
		int		svg_lnkwidth	= 2;
		int		svg_lnkwidth2	= 1;

		std::string fntSans		= "sans-serif";


		// Ramka related
		int     svg_ramka_min_w  = 600;
		int     svg_ramka_border = 100;

		// Final document border
		int     svg_paper_border = 40;

		// int     svg_paper_underh = 120; 

		bool    show_width_elmslr = false;
		bool    show_cnt          = true;

		bool    show_dbg_xwlwr    = false;

		// Mode : Tree created from Spectrum
		bool    from_spectrum     = false;

		// 
		std::string                 callparams;
};
