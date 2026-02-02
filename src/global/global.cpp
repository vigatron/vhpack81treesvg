#include "global.hpp"


TreeGenParamsIn		iparams;
TreeGenParamsOut	oparams;
VHSVG				svg;


VHTree				tree1;
VHTree				tree2;
VHTree				tree3;

VHTreeGfx			tree1gfx;
VHTreeGfx			tree2gfx;
VHTreeGfx			tree3gfx;


VHRect				gfxrect_ramka;

LayerArea			layerarea1;
LayerArea			layerarea2;
LayerArea			layerarea3;

VHRect				rectHeader1;
VHRect				rectCaption1;
VHRect				rectTree1;
VHRect				gfxrect_shadows1;
VHRect				gfxrect_bitfield1;
VHRect				gfxrect_sep1;


VHRect				rectHeader2;
VHRect				rectCaption2;
VHRect				rectTree2;
VHRect				gfxrect_shadows2;
VHRect				gfxrect_bitfield2;
VHRect				gfxrect_sep2;


VHRect				rectHeader3;
VHRect				rectCaption3;
VHRect				rectTree3;
VHRect				gfxrect_shadows3;
VHRect				gfxrect_bitfield3;
VHRect				gfxrect_sep3;


VHRect				gfxrect_spectrum;

verr verrmsg(int x, std::string strerr) {
	std::cout << "Err code : " << x << " ErrMsg: " << strerr << std::endl;
	return x; }
