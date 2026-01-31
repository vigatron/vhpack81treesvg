#include "global.hpp"
#include "args.hpp"
#include "parse_args.hpp"

using namespace std;

// -----------------------------------------------------------------------------
std::vector<int> ParseSpectrum( std::string strspc ) {
    std::vector<std::string>    spcvals = split(strspc, '.');
    std::vector<int> r;
    for( std::string s : spcvals) {
        if(!check_str_digit(s)) {
            verrmsg(1, "Invalid spectrum values");
            exit(1); }
        r.push_back( std::stoi(s) ); }
    return r; }

// -----------------------------------------------------------------------------
std::vector<int> ParseRotation( std::string rotation ) {
    std::vector<int> r;
    std::vector<std::string> rotvals = split(rotation, '^');
    for( std::string s : rotvals) {
        if(!check_str_digit(s)) {
            verrmsg(2, "Invalid rotation values");
            exit(1); }
        r.push_back( std::stoi(s) ); }
    return r; }

// -----------------------------------------------------------------------------
std::vector<int> ParseSvyazki( std::string strsvzk ) {
    std::vector<int> r;
    std::vector<std::string> svzpairs = split(strsvzk, ',');
    for( std::string s : svzpairs ) {
        std::vector<std::string> spl = split(s, '.');
        if(spl.size() == 2) {
            if(!check_str_digit(spl[0]) || !check_str_digit(spl[1]) ) {
                verrmsg(2, "Invalid bind values");
                exit(1); }
            int v1 = std::stoi( spl[0] );
            int v2 = std::stoi( spl[1] );
            r.push_back( v1 > v2 ? v2 : v1 );
            r.push_back( v1 > v2 ? v1 : v2 ); } }
    return r; }

// -----------------------------------------------------------------------------
std::vector<int> ParseInjected( std::string txt ) {
	std::vector<int> r;
	std::vector<std::string> spl = split(txt, '.');
	for( std::string s : spl) {
		if(!check_str_digit(s)) {
			verrmsg(2, "Invalid injection values");
			exit(1); }
			r.push_back( std::stoi( s )); }
	return r; }


// -----------------------------------------------------------------------------
static int ParseInt( std::string param) {
	return param.size() ? std::stoi(param) : -1; }

// -----------------------------------------------------------------------------
verr ParseIParams( int argc, char * argv[], VHArgsParser & argsparser ) {

	argsparser.setappname("pack81treesvg");

	argsparser.addopt(	"t",		"Build from TCode");
	argsparser.addopt(	"v",		"Build from spectrum values, for example '1.2.3'");
	argsparser.addopt(	"w",		"Rotate nodes, for example '1^2^3'");
	argsparser.addopt(	"z",		"Bind   nodes, for example '1.2,3.4'");
	argsparser.addopt(	"injc",		"Injected nodes, for example '19.21'");
	argsparser.addopt(	"cntr",		"Container number" );
	argsparser.addopt(	"prtn",		"Part  number ( optional )");
	argsparser.addopt(	"blkn",		"Block number ( optional )");
	argsparser.addopt(	"mix",		"Mixed mode");


	if( vok != argsparser.ParseArgs(argc, argv)) {
		std::cout << "Parse args issue" << std::endl;
		argsparser.Usage();
		return 1; }

	//
	iparams.callparams		= argsparser.listparams();

	iparams.param_tcode		= argsparser.getopt("t");
	iparams.param_partn		= argsparser.getopt("prtn");
	iparams.param_blockn	= argsparser.getopt("blkn");

	iparams.param_spcints	= ParseSpectrum( argsparser.getopt("v") );
	iparams.param_rotints	= ParseRotation( argsparser.getopt("w") );
	iparams.param_svzints	= ParseSvyazki ( argsparser.getopt("z") );
	iparams.param_injected	= ParseInjected( argsparser.getopt("injc") );
	iparams.param_cntrint	= ParseInt( argsparser.getopt("cntr") );
	iparams.param_mix		= ParseInt( argsparser.getopt("mix") );

	return vok; }
