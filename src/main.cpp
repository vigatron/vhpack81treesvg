#include "global.hpp"
#include "args.hpp"
#include "parse_args.hpp"

using namespace std;

// DATE : TIME
// Call Params
// Ramka up
// BPath rates %

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
    string strout = "tcode_";
    if( iparams.param_blockn.size()) { strout += "B" + iparams.param_blockn + "_"; }
    strout += iparams.param_tcode;

    // Save results
    svg.savetosvg( strout + ".svg");

    return vok; }

// -----------------------------------------------------------------------------
verr build_from_spc() {

    if(vok != tree.buildFromSpectrum()) {
        return verrmsg(2, "build tree from spectrum failed"); }

    iparams.from_spectrum = true;

    CalculateTreeGfx();
    RenderTreeGfx();

    // Generate file name
    string strout = "spectrum_";
    if(iparams.param_blockn.size()) { strout += "B" + iparams.param_blockn + "_"; }
    
    // Spectrum string for outfname
    string strspfx = iparams.param_strspc;
    for( int i=0; i < strspfx.size(); i++ ) { if(strspfx[i] == '.') strspfx[i] = '_'; }
    strout += strspfx;

    // Save results
    svg.savetosvg( strout + ".svg");

    return vok; }

// -----------------------------------------------------------------------------
int main( int argc, char * argv[] ) {

    // Parse args : input values or TCode
    VHArgsParser argsparser;

    argsparser.setappname("pack81treesvg");

    argsparser.addopt(  "t",    "Build from TCode");
    argsparser.addopt(  "v",    "Build from spectrum values, for example '1.2.3'");
    argsparser.addopt(  "w",    "Rotate nodes, for example '1^2^3'");
    argsparser.addopt(  "z",    "Bind   nodes, for example '1.2,3.4'");
    argsparser.addopt(  "blkn", "Block number ( optional)");

    if( vok != argsparser.ParseArgs(argc, argv)) {
        std::cout << "Parse args issue" << std::endl;
        argsparser.Usage();
        return 1; }

    iparams.callparams = argsparser.listparams();

    iparams.param_tcode     = argsparser.getopt("t");
    iparams.param_strspc    = argsparser.getopt("v");
    iparams.param_blockn    = argsparser.getopt("blkn");
    iparams.param_strrot    = argsparser.getopt("w");
    iparams.param_strsvz    = argsparser.getopt("z");

    iparams.param_spcints   = ParseSpectrum( iparams.param_strspc );
    iparams.param_rotints   = ParseRotation( iparams.param_strrot );
    iparams.param_svzints   = ParseSvyazki ( iparams.param_strsvz );

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
