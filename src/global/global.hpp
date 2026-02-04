#pragma once

#include "strtools.hpp"
#include "tree.hpp"
#include "svg.hpp"

#include "layerarea.hpp"
#include "treegfx.hpp"
#include "treearea.hpp"


extern VHTree				tree1;
extern VHTree				tree2;
extern VHTree				tree3;

extern VHTreeGfx			tree1gfx;
extern VHTreeGfx			tree2gfx;
extern VHTreeGfx			tree3gfx;


extern TreeGenParamsIn		iparams;
extern TreeGenParamsOut		oparams;

extern VHRect				gfxrect_ramka;


extern LayerArea			layerarea1;
extern LayerArea			layerarea2;
extern LayerArea			layerarea3;


// x3 Layers
extern TreeArea				treearea[3];


extern VHRect				gfxrect_spectrum;


void RenderFinalDocument( int layscount );
