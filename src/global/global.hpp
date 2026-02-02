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

class TreeArea {
	public:


};

extern LayerArea			layerarea1;
extern LayerArea			layerarea2;
extern LayerArea			layerarea3;


// x3 Layers

extern VHRect				rectHeader1;
extern VHRect				rectCaption1;
extern VHRect				rectTree1;
extern VHRect				gfxrect_shadows1;
extern VHRect				gfxrect_bitfield1;
extern VHRect				gfxrect_sep1;


extern VHRect				rectHeader2;
extern VHRect				rectCaption2;
extern VHRect				rectTree2;
extern VHRect				gfxrect_shadows2;
extern VHRect				gfxrect_bitfield2;
extern VHRect				gfxrect_sep2;


extern VHRect				rectHeader3;
extern VHRect				rectCaption3;
extern VHRect				rectTree3;
extern VHRect				gfxrect_shadows3;
extern VHRect				gfxrect_bitfield3;
extern VHRect				gfxrect_sep3;


extern VHRect				gfxrect_spectrum;


void RenderFinalDocument( int layscount );
