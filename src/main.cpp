#include <string>
#include <vector>

#include "global.hpp"
#include "args.hpp"

using namespace std;



verr verrmsg(int x, std::string strerr) {
    std::cout << strerr << endl;
    return x; }

verr build_from_tcode( std::string strtcode, std::string blkn) {

    if(!strtcode.size() || !check_str_ishex(strtcode)) { 
        return verrmsg(1, "Invalid TCode"); }

    tree.initFromTCode(strtcode);
    CalculateTreeGfx();
    RenderTreeGfx();

    // Generate file name
    string strout = "tcode_";
    if(blkn.size()) { strout += blkn + "_"; }
    strout += strtcode;

    // Save results
    svg.savetosvg( strout + ".svg");

    return vok; }

verr build_from_spc( std::string strspc ) {

    std::vector<std::string> spcvals = split(strspc, '.');

    return verror(1); }

// argsparser.Usage();

int main( int argc, char * argv[] ) {

    // Parse args : input values or TCode
    VHArgsParser argsparser;

    argsparser.setappname("pack81treesvg");

    argsparser.addopt(  "t",    "Build from TCode");
    argsparser.addopt(  "v",    "Build from spectrum values, for example '1+2+3'");
    argsparser.addopt(  "blkn", "Block number ( optional)");

    if( vok != argsparser.ParseArgs(argc, argv)) {
        std::cout << "Parse args issue" << std::endl;
        argsparser.Usage();
        return 1; }

    verr ret;

    if(argsparser.checkopt("t")) {

        ret = build_from_tcode(
            argsparser.getopt("t"),
            argsparser.getopt("blkn") );

    } else if(argsparser    .checkopt("v")) {

        ret = build_from_spc( 
            argsparser.getopt("v") );
    } else {
        ret = 1;
        std::cout << "No valid input data : " << argsparser.listparams() << std::endl;
        argsparser.Usage();
    }

    if(vok != ret) { std::cout << "SVG generation issue!" << std::endl; }

    return vok == ret; }
