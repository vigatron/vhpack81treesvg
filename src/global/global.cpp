#include "global.hpp"


TreeGenParamsIn     iparams;
TreeGenParamsOut    oparams;
VHSVG               svg;


VHTree				tree1;
VHTree				tree2;
VHTree				tree3;

VHTreeGfx			tree1gfx;
VHTreeGfx			tree2gfx;
VHTreeGfx			tree3gfx;


VHRect				gfxrect_ramka;
VHRect				gfxrect_header;


VHRect				rectCaption1;
LayerArea			layerarea1;
VHRect				gfxrect_shadows1;
VHRect				gfxrect_bitfield1;

VHRect				rectCaption2;
LayerArea			layerarea2;
VHRect				gfxrect_shadows2;
VHRect				gfxrect_bitfield2;

VHRect				rectCaption3;
LayerArea			layerarea3;
VHRect				gfxrect_shadows3;


VHRect				gfxrect_spectrum;

verr verrmsg(int x, std::string strerr) {
	std::cout << strerr << std::endl;
	return x; }
