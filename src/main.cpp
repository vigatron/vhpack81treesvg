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
verr SetRunMode() {

	int sztcode = iparams.param_tcode.size();
	int szspc	= iparams.param_spcints.size();

	if( !sztcode && !szspc )
			return verrmsg(100, "Need TCode or SPC array to process");

	// Default mode : tcode or spcmode ?
	if(iparams.param_mix == -1) {
		if( sztcode )			{ iparams.genmode = 1; }
		else if( szspc )		{ iparams.genmode = 2; }
	} else { // Mix mode
		iparams.genmode = 3;
		if(!szspc)
			return verrmsg(100, "Need SPC array to process"); }

	return vok; }

// -----------------------------------------------------------------------------
int main( int argc, char * argv[] ) {

	VHArgsParser argsparser;

	// Parse command line params
	verr rparse = ParseIParams( argc, argv, argsparser );
	if( vok != rparse ) return 1;

	// Recognize mode and check params
	verr rmode = SetRunMode();
	if( vok != rmode )
		return verrmsg(80, "RunMode issue, input parameters maybe");

	verr ret = 1;
	switch( iparams.genmode ) {
		case 1 : { ret = GenerateTcdMode(); } break;
		case 2 : { ret = GenerateSpcMode(); } break;
		case 3 : { ret = GenerateMixMode(); } break;
		default: { } }

	// Save results
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
