#pragma once

#include "strtools.hpp"
#include "vhrect.hpp"
#include "tree.hpp"
#include "svg.hpp"

#include "layerarea.hpp"
#include "treegfx.hpp"


extern VHTree				tree1;
extern VHTree				tree2;
extern VHTree				tree3;

extern VHTreeGfx			tree1gfx;
extern VHTreeGfx			tree2gfx;
extern VHTreeGfx			tree3gfx;


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


void RenderFinalDocument();
