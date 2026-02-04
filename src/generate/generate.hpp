#pragma once

#include "global.hpp"
#include "calc.hpp"

verr GenerateBuildTrees();
verr GenerateCalculateSizes();
verr GenerateCommon();

void CalcTreeGfx (
	const VHRect & rect,
	VHTree & tree , VHTreeGfx & tgfx ,
	LayerArea & larea , int layscount );
