#include "global.hpp"
#include "calc.hpp"

// 	// Строим дерево по ТКоду
// // -----------------------------------------------------------------------------
// verr GenerateTMode() {
// 	verr ret;
// 	ret = build_from_tcode();
// 	return ret;
// }
// // -----------------------------------------------------------------------------
// verr build_from_tcode() {
// 	return verror(1);
// 	CalculateTreeGfx();
// 	RenderFinalDocument();


// -----------------------------------------------------------------------------
std::string genOutFNameTcd() {
	std::string pfx = "tcode_";
	if( iparams.param_partn.size () ) { pfx += "P" + iparams.param_partn  + "_"; }
	if( iparams.param_blockn.size() ) { pfx += "B" + iparams.param_blockn + "_"; }
	std::string r = pfx + iparams.param_tcode + ".svg";
	return r; }

// -----------------------------------------------------------------------------
// TODO
// -----------------------------------------------------------------------------
verr GenerateTcdMode() {
	oparams.outfname = genOutFNameTcd();
	return verror(1); }
