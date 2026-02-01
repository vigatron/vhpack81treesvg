#include "global.hpp"
#include "calc.hpp"
// #include "tree.hpp"
// #include "treegfx.hpp"


// Комбинированный режим
// 1) Tree by spectrum
// 2) Tree by spectrum + Autorotation
// 3) TCode
// 4) Spectrum analisys

// -----------------------------------------------------------------------------
static verr BuildTrees() {

	verr ret;

	ret = tree1.buildFromSpectrum( iparams.param_spcints , false );
	if(vok != ret) return verrmsg(21, "build tree from spectrum failed");

	ret = tree2.buildFromSpectrum( iparams.param_spcints, true );
	if(vok != ret) return verrmsg(22, "build tree from spectrum rotated failed");

	std::vector<VHTree::stnode> scode = tree2.BuildSCodeFromHuffman();
	std::string strtcode = tree2.SCodeToTCode( scode );

	ret = tree3.buildFromTCode( strtcode , false );
	if( vok != ret ) return verrmsg(1, "Can't build tree from TCode");

	return vok; }


// -----------------------------------------------------------------------------
static verr CalculateSizes() {

	verr ret;
	int node_xdist	= iparams.svg_elm_width;
	int	node_spacr	= node_xdist / 2;

	tree1gfx.CalculateTreeGfx( tree1 , node_xdist , node_spacr );
	tree2gfx.CalculateTreeGfx( tree2 , node_xdist , node_spacr );
	tree3gfx.CalculateTreeGfx( tree3 , node_xdist , node_spacr );

	return vok; }

// -----------------------------------------------------------------------------
std::string genOutFNameMix() {
	std::string pfx = "mix_";
	std::string r = pfx + ".svg";
	return r; }


void CalcTreeGfx	( VHTree & tree , VHTreeGfx & tgfx , LayerArea & larea );

// -----------------------------------------------------------------------------
verr GenerateMixMode() {

	verr ret;

	ret = BuildTrees();
	if( vok != ret) return verrmsg(100, "BuildTrees issue");

	ret = CalculateSizes();
	if( vok != ret) return verrmsg(101, "Calculate LR size issues");

	// Calculate final document size
	int width		= tree2gfx.wdt();
	int	layscount 	= tree2.arch().depthmax() + 1;
	svgcalc_ramka( width, layscount ); 

	// Calc tree nodes coords
	CalcTreeGfx( tree1 , tree1gfx , layerarea1 );
	CalcTreeGfx( tree2 , tree2gfx , layerarea2 );
	CalcTreeGfx( tree3 , tree3gfx , layerarea3 );

	RenderFinalDocument();

	oparams.outfname = genOutFNameMix();
	return ret; }


// iparams.svg_layerh
// iparams.svg_elm_width/2

// int	idx			= tree.rootidx();
// int	nodew		= oparams.gfx_nodewl[ idx ] + oparams.gfx_nodewr[ idx ];
