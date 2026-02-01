#include "global.hpp"
#include "args.hpp"
#include "parse_args.hpp"
#include "calc.hpp"

using namespace std;


verr GenerateMixMode();
verr GenerateSpcMode();
verr GenerateTcdMode();

// -----------------------------------------------------------------------------
verr check_t( VHArgsParser & argsparser ) {
	if( !argsparser.checkopt("t") ) {
		std::cout << "No valid tcode data : "
			<< argsparser.listparams() << std::endl;
		argsparser.Usage(); }
	return verror(1); }

// -----------------------------------------------------------------------------
verr check_s( VHArgsParser & argsparser ) {
	if( !argsparser.checkopt("s") ) {
		std::cout << "No valid spectrum data : "
			<< argsparser.listparams() << std::endl;
		argsparser.Usage(); }
	return verror(1); }

// -----------------------------------------------------------------------------
int main( int argc, char * argv[] ) {

	// Parse args : input values or TCode
	VHArgsParser argsparser;

	verr rparse = ParseIParams( argc, argv, argsparser );
	if( vok != rparse) return 1;

	//
	verr ret = 1;

	//
	if( iparams.param_mix ) {
		ret = GenerateMixMode();
	} else {
		// ret = argsparser.checkopt("v") ? GenerateSpcMode() : GenerateTMode();
		// if(! iparams.param_tcode.size() || !check_str_ishex(iparams.param_tcode)) { 
		// 	return verrmsg(1, "Invalid TCode"); }
		ret = verror(100);
	}

	//
	if(vok == ret) {
		svg.savetosvg( oparams.outfname ); }
	else {
		std::cout <<
		"Final result: generation process failed !"
		<< std::endl; }

	return (vok == ret) ? 0 : 1; }


// >>> Done >>>
// DATE : TIME
// Call Params
// Ramka up
// BPath rates %


