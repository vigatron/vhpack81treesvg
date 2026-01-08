#include "global.hpp"

VHTree              tree;
TreeGenParamsIn     iparams;
TreeGenParamsOut    oparams;
VHSVG               svg;


VHRect              gfxrect_ramka;

VHRect              gfxrect_header;
VHRect              gfxrect_layers;
VHRect              gfxrect_shadows;
VHRect              gfxrect_bitfield;
VHRect              gfxrect_spectrum;

verr verrmsg(int x, std::string strerr) {
    std::cout << strerr << std::endl;
    return x; }
