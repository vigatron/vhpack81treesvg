#include "global.hpp"

VHTree              tree;
TreeGenParamsIn     iparams;
TreeGenParamsOut    oparams;
VHSVG               svg;


VHRect              gfx_ramka;

verr verrmsg(int x, std::string strerr) {
    std::cout << strerr << std::endl;
    return x; }
