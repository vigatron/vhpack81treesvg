#pragma once

#include "strtools.hpp"
#include "vhrect.hpp"
#include "tree.hpp"
#include "svg.hpp"

#include "layerarea.hpp"


extern VHTree				tree;

extern TreeGenParamsIn		iparams;
extern TreeGenParamsOut		oparams;

extern VHRect				gfxrect_ramka;
extern VHRect				gfxrect_header;

extern VHRect               gfxrect_bitfield;
extern VHRect               gfxrect_spectrum;

// x3 Layers

extern VHRect				rectCaption1;
extern LayerArea			layerarea1;
extern VHRect				gfxrect_shadows1;

extern VHRect				rectCaption2;
extern LayerArea			layerarea2;
extern VHRect				gfxrect_shadows2;

extern VHRect				rectCaption3;
extern LayerArea			layerarea3;
extern VHRect				gfxrect_shadows3;



// int svg_getlayer_posy (int layn);
// int svg_getlayer_posyc(int layn);

void CalculateTreeGfx();
void RenderTreeGfx();
