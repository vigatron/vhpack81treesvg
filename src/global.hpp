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

// x3 Layers

extern VHRect				rectCaption1;
extern LayerArea			layerarea1;
extern VHRect				gfxrect_shadows1;
extern VHRect				gfxrect_bitfield1;


extern VHRect				rectCaption2;
extern LayerArea			layerarea2;
extern VHRect				gfxrect_shadows2;
extern VHRect				gfxrect_bitfield2;


extern VHRect				rectCaption3;
extern LayerArea			layerarea3;
extern VHRect				gfxrect_shadows3;


extern VHRect				gfxrect_spectrum;


// int svg_getlayer_posy (int layn);
// int svg_getlayer_posyc(int layn);

void CalculateTreeGfx();
void RenderTreeGfx();
