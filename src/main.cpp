#include <string>
#include <vector>

#include "global.hpp"

using namespace std;


// -------------------------------------------------------------------------------------------------

// r.push_back( svg_rect( 0, 0, svg_elm_width,   svg_elm_width, 1, col_gray, "none" ));
// r.push_back( svg_circ(10,10, svg_elm_width/2,                1, col_gray, "none" ));


int main(int argc, char * argv[]) {

    vector<string> dbkeys;

    dbkeys.push_back("[16:1][15:3][13:3][11:2][9:0][12:1][10:0][14:0]");
    // dbkeys.push_back("[12:2][11:1][10:3][8:2][7:0][9:0]");
    // dbkeys.push_back("[14:2][13:2][12:3][10:0][11:1][9:1][8:0]");
    // dbkeys.push_back("[18:3][16:0][17:2][15:3][13:3][10:0][11:0][14:1][12:0]");
    // dbkeys.push_back("[18:3][16:1][14:3][12:2][10:0][13:1][11:0][17:1][15:0]");
    // dbkeys.push_back("[14:2][13:1][12:2][11:3][9:2][8:0][10:0]");
    // dbkeys.push_back("[16:3][14:1][12:1][10:0][15:1][13:1][11:2][9:0]");
    // dbkeys.push_back("[12:1][11:2][10:2][9:2][8:2][7:0])";

    // Custom
    // string strscode  = "[8:1][7:3][5:0][6:0]";
    // string strscode  = "[16:1][15:3][13:3][9:0][10:0][14:3][11:0][12:0]";

    tree.fromstr(dbkeys[0]);
    tree.dump();
    tree.build();
    tree.dumplr();
    tree.dumpnodes();

    CalculateTreeGfx();
    RenderTreeGfx();

    svg.savetosvg("pic.svg");

    return 0;
}
