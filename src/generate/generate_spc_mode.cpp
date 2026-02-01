#include "global.hpp"
#include "calc.hpp"

// 	// Строим дерево по спектру
// // -----------------------------------------------------------------------------
// verr GenerateSpcMode() {
// 	verr ret;
// 	ret = build_from_spc  ();
// 	return ret;
// }
// // -----------------------------------------------------------------------------
// verr build_from_spc() {
// 	// Generate file name
// 	std::string fname = genoutfname(false);
// 	// Save results
// 	svg.savetosvg( fname );
// 	return vok; }


// -----------------------------------------------------------------------------
std::string genOutFNameSpc() {
	std::string pfx = "spctr_";
	if( iparams.param_partn.size () ) { pfx += "P" + iparams.param_partn  + "_"; }
	if( iparams.param_blockn.size() ) { pfx += "B" + iparams.param_blockn + "_"; }
	std::string r = pfx + "spc" + ".svg";
	return r; }

// -----------------------------------------------------------------------------
// TODO
// -----------------------------------------------------------------------------
verr GenerateSpcMode() {
	oparams.outfname = genOutFNameSpc();
	return verror(1); }
