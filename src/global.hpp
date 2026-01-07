#pragma once

#include "strtools.hpp"
#include "vhrect.hpp"
#include "tree.hpp"
#include "svg.hpp"



extern VHTree           tree;
extern VHSVG            svg;
extern TreeGenParamsIn  iparams;
extern TreeGenParamsOut oparams;

extern VHRect               gfxrect_ramka;

extern VHRect               gfxrect_header;
extern VHRect               gfxrect_layers;
extern VHRect               gfxrect_shadows;
extern VHRect               gfxrect_bitfield;


void CalculateTreeGfx();
void RenderTreeGfx();
