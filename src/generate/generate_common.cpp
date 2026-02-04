#include "global.hpp"
#include "calc.hpp"
#include "generate.hpp"

// -----------------------------------------------------------------------------
verr GenerateBuildTrees() {

	verr ret;
	bool rotation = false;

	if( iparams.genmode == 1 ) {

		ret = tree1.buildFromTCode( iparams.param_tcode , rotation );
		if( vok != ret ) return verrmsg(91, "Can't build tree from TCode");
		rotation = true;
		ret = tree2.buildFromTCode( iparams.param_tcode , rotation );
		if( vok != ret ) return verrmsg(92, "Can't build tree from TCode");

	} else if( iparams.genmode == 2 ) { 

		ret = tree1.buildFromSpectrum( iparams.param_spcints , rotation );
		if(vok != ret) return verrmsg(93, "build tree from spectrum failed");
		rotation = true;
		ret = tree2.buildFromSpectrum( iparams.param_spcints , rotation );
		if(vok != ret) return verrmsg(94, "build tree from spectrum failed");

	} 
	else if( iparams.genmode == 3 ) {

		ret = tree1.buildFromSpectrum( iparams.param_spcints , rotation );
		if(vok != ret) return verrmsg(96, "build tree from spectrum failed");
		rotation = true;
		ret = tree2.buildFromSpectrum( iparams.param_spcints, rotation );
		if(vok != ret) return verrmsg(97, "build tree from spectrum rotated failed");

		std::vector<VHTree::stnode> scode = tree2.BuildSCodeFromHuffman();
		std::string strtcode = tree2.SCodeToTCode( scode );
		oparams.tblreidx = tree2.reidxtbl();
		rotation = false;
		ret = tree3.buildFromTCode( strtcode , rotation );
		if( vok != ret ) return verrmsg(98, "Can't build tree from TCode");

	} else {
		return verrmsg(110, "Invalid genmode"); }

	return vok; }

// -----------------------------------------------------------------------------
verr GenerateCalculateSizes() {

	verr ret;
	int node_xdist	= iparams.svg_elm_width;
	int	node_spacr	= node_xdist / 2;

	tree1gfx.CalculateTreeGfx( tree1 , node_xdist , node_spacr );
	tree2gfx.CalculateTreeGfx( tree2 , node_xdist , node_spacr );
	if( iparams.genmode == 3 ) {
		tree3gfx.CalculateTreeGfx( tree3 , node_xdist , node_spacr ); }
	return vok; }

// -----------------------------------------------------------------------------
verr GenerateCalculateTreesGfx() {

	int	layscount 	= tree1.arch().depthmax() + 1;

	// Calc tree nodes coords 
	CalcTreeGfx( treearea[0].rectTree, tree1 , tree1gfx , layerarea1 , layscount );
	CalcTreeGfx( treearea[1].rectTree, tree2 , tree2gfx , layerarea2 , layscount );
	if( iparams.genmode == 3 ) {
		CalcTreeGfx( treearea[2].rectTree, tree3 , tree3gfx , layerarea3 , layscount ); }

	return vok; }

// -----------------------------------------------------------------------------
verr GenerateCommon() {

	verr ret;

	ret = GenerateBuildTrees();
	if( vok != ret)
		return verrmsg(100, "BuildTrees issue");

	// Габаритные размеры
	ret = GenerateCalculateSizes();
	if( vok != ret)
		return verrmsg(101, "Calculate LR size issues");

	// Calculate final document size
	int width		= ( iparams.genmode == 3 ) ? tree2gfx.wdt() : tree1gfx.wdt();
	int	layscount	= tree1.arch().depthmax() + 1;
	svgcalc_ramka( width, layscount ); 

	ret = GenerateCalculateTreesGfx();
	if( vok != ret)
		return verrmsg(102, "Calculate LR size issues");

	RenderFinalDocument( layscount );

	return ret; }

// -----------------------------------------------------------------------------
static std::string genOutFNameTcd() {
	std::string pfx = "tcode_";
	if( iparams.param_partn.size () ) { pfx += "P" + iparams.param_partn  + "_"; }
	if( iparams.param_blockn.size() ) { pfx += "B" + iparams.param_blockn + "_"; }
	std::string r = pfx + iparams.param_tcode + ".svg";
	return r; }

// -----------------------------------------------------------------------------
static std::string genOutFNameSpc() {
	std::string pfx = "spctr_";
	if( iparams.param_partn.size () ) { pfx += "P" + iparams.param_partn  + "_"; }
	if( iparams.param_blockn.size() ) { pfx += "B" + iparams.param_blockn + "_"; }
	std::string r = pfx + "spc" + ".svg";
	return r; }

// -----------------------------------------------------------------------------
static std::string genOutFNameMix() {
	std::string pfx = "mix_";
	std::string r = pfx + ".svg";
	return r; }

// -----------------------------------------------------------------------------
// Строим дерево по ТКоду
// -----------------------------------------------------------------------------

verr GenerateTcdMode() {
	verr ret = GenerateCommon();
	oparams.outfname = genOutFNameTcd();
	return ret; }

// -----------------------------------------------------------------------------
// Строим дерево по спектру
// -----------------------------------------------------------------------------

verr GenerateSpcMode() {
	verr ret = GenerateCommon();
	oparams.outfname = genOutFNameSpc();
	return ret; }

// -----------------------------------------------------------------------------
// Комбинированный режим
// 1) Tree by spectrum
// 2) Tree by spectrum + Autorotation
// 3) TCode
// 4) Spectrum analisys
// -----------------------------------------------------------------------------

verr GenerateMixMode() {
	verr ret = GenerateCommon();
	oparams.outfname = genOutFNameMix();
	return ret; }
