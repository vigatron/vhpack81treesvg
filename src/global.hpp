#pragma once

#include "vhrect.hpp"
#include "tree.hpp"
#include "svg.hpp"



extern VHTree           tree;
extern VHSVG            svg;
extern TreeGenParamsIn  iparams;
extern TreeGenParamsOut oparams;

extern VHRect gfx_ramka;

void CalculateTreeGfx();
void RenderTreeGfx();
