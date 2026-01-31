#include "global.hpp"

VHTree              tree;
TreeGenParamsIn     iparams;
TreeGenParamsOut    oparams;
VHSVG               svg;


VHRect				gfxrect_ramka;
VHRect				gfxrect_header;


VHRect				rectCaption1;
LayerArea			layerarea1;
VHRect				gfxrect_shadows1;

VHRect				rectCaption2;
LayerArea			layerarea2;
VHRect				gfxrect_shadows2;

VHRect				rectCaption3;
LayerArea			layerarea3;
VHRect				gfxrect_shadows3;


VHRect				gfxrect_shadows;
VHRect				gfxrect_bitfield;
VHRect				gfxrect_spectrum;

verr verrmsg(int x, std::string strerr) {
	std::cout << strerr << std::endl;
	return x; }
