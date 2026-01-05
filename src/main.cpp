#include <string>
#include <vector>

#include "global.hpp"
#include "args.hpp"

using namespace std;

// DATE : TIME
// Call Params
// Ramka up
// BPath rates %

// -----------------------------------------------------------------------------

verr build_from_tcode( std::string strtcode, std::string blkn, std::vector<int> rotints) {

    if(!strtcode.size() || !check_str_ishex(strtcode)) { 
        return verrmsg(1, "Invalid TCode"); }

    if( vok != tree.buildFromTCode(strtcode) )
        return verrmsg(1, "Can't build tree from TCode");

    CalculateTreeGfx();
    RenderTreeGfx();

    // Generate file name
    string strout = "tcode_";
    if(blkn.size()) { strout += "B" + blkn + "_"; }
    strout += strtcode;

    // Save results
    svg.savetosvg( strout + ".svg");

    return vok; }

// -----------------------------------------------------------------------------
verr build_from_spc( std::string strspc, std::string blkn, std::vector<int> rotints ) {

    std::vector<std::string>    spcvals = split(strspc, '.');
    std::vector<int>            spcints;

    for( std::string s : spcvals) {
        if(!check_str_digit(s))
            return verrmsg(1, "Invalid spectrum values");
        spcints.push_back( std::stoi(s) ); }

    if(vok != tree.buildFromSpectrum(spcints)) {
        return verrmsg(2, "build tree from spectrum failed"); }

    iparams.from_spectrum = true;

    CalculateTreeGfx();
    RenderTreeGfx();

    // Generate file name
    string strout = "spectrum_";
    if(blkn.size()) { strout += "B" + blkn + "_"; }
    
    // Spectrum string for outfname
    string strspfx = strspc;
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
    argsparser.addopt(  "blkn", "Block number ( optional)");

    if( vok != argsparser.ParseArgs(argc, argv)) {
        std::cout << "Parse args issue" << std::endl;
        argsparser.Usage();
        return 1; }

    iparams.callparams = argsparser.listparams();


    std::string blockn   = argsparser.checkopt("blkn")  ? argsparser.getopt("blkn") : "";
    std::string rotation = argsparser.checkopt("w")     ? argsparser.getopt("w")    : "";

    std::vector<std::string>    rotvals = split(rotation, '^');
    std::vector<int>            rotints;

    for( std::string s : rotvals) {
        if(!check_str_digit(s))
            return verrmsg(2, "Invalid rotation values");
        rotints.push_back( std::stoi(s) ); }


    // Строим дерево по спектру либо по ТКоду

    verr ret;


    if(argsparser.checkopt("t"))        { ret = build_from_tcode( argsparser.getopt("t"), blockn, rotints );
    } else if(argsparser.checkopt("v")) { ret = build_from_spc(   argsparser.getopt("v"), blockn, rotints );
    } else {
        std::cout << "No valid input data : " << argsparser.listparams() << std::endl;
        argsparser.Usage(); ret = 1; }

    if(vok != ret) {
        std::cout << "Final result: generation process failed !" << std::endl; }

    return (vok == ret) ? 0 : 1; }
