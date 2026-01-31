#include "global.hpp"
#include "args.hpp"
#include "parse_args.hpp"

using namespace std;

// >>> Done >>>
// DATE : TIME
// Call Params
// Ramka up
// BPath rates %

// argsparser.checkopt("t")

// else {
// 	std::cout << "No valid input data : " << argsparser.listparams() << std::endl;
// 	argsparser.Usage(); ret = 1; }


// -----------------------------------------------------------------------------
std::string genoutfname(bool tcd) {

	std::string pfx = tcd ? "tcode_" : "spctr_";
	if( iparams.param_partn.size () ) { pfx += "P" + iparams.param_partn  + "_"; }
	if( iparams.param_blockn.size() ) { pfx += "B" + iparams.param_blockn + "_"; }

	std::string r = pfx + (tcd ? iparams.param_tcode : "spc" ) + ".svg";

	return r; }

// -----------------------------------------------------------------------------
verr build_from_tcode() {

	if(! iparams.param_tcode.size() || !check_str_ishex(iparams.param_tcode)) { 
		return verrmsg(1, "Invalid TCode"); }

	if( vok != tree.buildFromTCode( iparams.param_tcode , true ) )
		return verrmsg(1, "Can't build tree from TCode");

	CalculateTreeGfx();
	RenderTreeGfx();

	// Generate file name
	std::string fname = genoutfname(true);

	// Save results
	svg.savetosvg( fname );

	return vok; }

// -----------------------------------------------------------------------------
verr build_from_spc() {

	if(vok != tree.buildFromSpectrum( iparams.param_spcints , true )) {
		return verrmsg(2, "build tree from spectrum failed"); }

	iparams.from_spectrum = true;

	CalculateTreeGfx();
	RenderTreeGfx();

	// Generate file name
	std::string fname = genoutfname(false);

	// Save results
	svg.savetosvg( fname );

	return vok; }

// -----------------------------------------------------------------------------
verr GenerateMixMode() {

	verr ret;

	iparams.optAutoRotate = 0;
	ret = build_from_spc  ();

	iparams.optAutoRotate = 1;

	return ret;
}

// -----------------------------------------------------------------------------
verr GenerateSpcMode() {

	verr ret;

	iparams.optAutoRotate = 1;

	// Строим дерево по спектру
	ret = build_from_spc  ();

	return ret;
}

// -----------------------------------------------------------------------------
verr GenerateTMode() {

	verr ret;

	iparams.optAutoRotate = 1;

	// Строим дерево по ТКоду
	ret = build_from_tcode();

	return ret;
}

// -----------------------------------------------------------------------------
int main( int argc, char * argv[] ) {

	// Parse args : input values or TCode
	VHArgsParser argsparser;

	verr rparse = ParseIParams( argc, argv, argsparser );
	if( vok != rparse)
		return 1;

	// Комбинированный режим
	// 1) Spectrum
	// 2) By Spectrum + Autorotation
	// 3) TCode

	verr ret = 1;

	if( iparams.param_mix ) {
		ret = GenerateMixMode();
	} else {
		ret = argsparser.checkopt("v") ? GenerateSpcMode() : GenerateTMode();
	}

	if(vok != ret) { std::cout << "Final result: generation process failed !" << std::endl; }

	return (vok == ret) ? 0 : 1; }
