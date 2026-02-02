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

TreeArea			treearea[3];

VHRect				gfxrect_spectrum;

verr verrmsg(int x, std::string strerr) {
	std::cout << "Err code : " << x << " ErrMsg: " << strerr << std::endl;
	return x; }
