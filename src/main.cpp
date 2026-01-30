#include "global.hpp"
#include "args.hpp"
#include "parse_args.hpp"

using namespace std;

// DATE : TIME
// Call Params
// Ramka up
// BPath rates %

std::string genoutfname(bool tcd) {

	std::string pfx = tcd ? "tcode_" : "spctr_";
	if( iparams.param_partn.size () ) { pfx += "P" + iparams.param_partn  + "_"; }
	if( iparams.param_blockn.size() ) { pfx += "B" + iparams.param_blockn + "_"; }

	std::string r = pfx + (tcd ? iparams.param_tcode : "spc" ) + ".svg";

	// // Spectrum string for outfname
	// string strspfx = iparams.param_strspc;
	// for( int i=0; i < strspfx.size(); i++ ) { if(strspfx[i] == '.') strspfx[i] = '_'; }
	return r; }

// -----------------------------------------------------------------------------
// std::string strtcode, std::string blkn, std::vector<int> rotints
verr build_from_tcode() {

	if(! iparams.param_tcode.size() || !check_str_ishex(iparams.param_tcode)) { 
		return verrmsg(1, "Invalid TCode"); }

	if( vok != tree.buildFromTCode() )
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

	if(vok != tree.buildFromSpectrum()) {
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
int ParseInt( std::string param) {
	return param.size() ? std::stoi(param) : -1; }

// -----------------------------------------------------------------------------
verr ParseIParams( int argc, char * argv[], VHArgsParser & argsparser ) {

	argsparser.setappname("pack81treesvg");

	argsparser.addopt(  "t",    "Build from TCode");
	argsparser.addopt(  "v",    "Build from spectrum values, for example '1.2.3'");
	argsparser.addopt(  "w",    "Rotate nodes, for example '1^2^3'");
	argsparser.addopt(  "z",    "Bind   nodes, for example '1.2,3.4'");
	argsparser.addopt(	"injc",	"Injected nodes, for example '19.21'");
	argsparser.addopt(	"cntr",	"Container number" );
	argsparser.addopt(	"prtn",	"Part  number ( optional )");
	argsparser.addopt(  "blkn", "Block number ( optional )");

	if( vok != argsparser.ParseArgs(argc, argv)) {
		std::cout << "Parse args issue" << std::endl;
		argsparser.Usage();
		return 1; }

	//

	iparams.callparams		= argsparser.listparams();

	iparams.param_tcode		= argsparser.getopt("t");
	iparams.param_cntr		= argsparser.getopt("cntr");

	iparams.param_partn		= argsparser.getopt("prtn");
	iparams.param_blockn	= argsparser.getopt("blkn");

	iparams.param_spcints	= ParseSpectrum( argsparser.getopt("v") );
	iparams.param_rotints	= ParseRotation( argsparser.getopt("w") );
	iparams.param_svzints	= ParseSvyazki ( argsparser.getopt("z") );
	iparams.param_injected	= ParseInjected( argsparser.getopt("injc") );
	iparams.param_cntrint	= ParseInt( iparams.param_cntr );

	return vok; }

// -----------------------------------------------------------------------------
int main( int argc, char * argv[] ) {

	// Parse args : input values or TCode
	VHArgsParser argsparser;

	verr rparse = ParseIParams( argc, argv, argsparser );
	if( vok != rparse)
		return 1;

	// Строим дерево по спектру либо по ТКоду
	verr ret;
	if(argsparser.checkopt("t"))        { ret = build_from_tcode();
	} else if(argsparser.checkopt("v")) { ret = build_from_spc  ();
	} else {
		std::cout << "No valid input data : " << argsparser.listparams() << std::endl;
		argsparser.Usage(); ret = 1; }

	if(vok != ret) {
		std::cout << "Final result: generation process failed !" << std::endl; }

	return (vok == ret) ? 0 : 1; }
