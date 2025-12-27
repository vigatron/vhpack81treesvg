#pragma once

#include "tree.hpp"
#include "svg.hpp"


class colors {

    public:

    static  std::string white;       // = "#FFFFFF";
    static  std::string yellowl;     // = "#FFFFE0";
    static  std::string yellowll;    // = "#FFFFF0";
    static  std::string myell;       // = "#FFFFD0";
    static  std::string nyell;       // = "#FFFFA0";

    static  std::string lgreen;      // = "#E0FFE0";
    static  std::string mgreen;      // = "#90C090";
    static  std::string ngreen;      // = "#80AF80";
    static  std::string dgreen;      // = "#00AF00";
 
    static  std::string lblue;       // = "#F0F0FF";
    static  std::string sblue;       // = "#E0E0FF";
    static  std::string mblue;       // = "#C0C0FF";
    static  std::string nblue;       // = "#8080FF";
    static  std::string dblue;       // = "#0000B0";

    static  std::string sgray;       // = "#E0E0E0";
    static  std::string mgray;       // = "#C0C0C0";
    static  std::string lgray;       // = "#A0A0A0";
    static  std::string gray;        // = "#303030";
};

// Input : Configuration Parameters
class TreeGenParamsIn {

    public:

        int     svg_width       = 2000 * 4;
        int     svg_height      = 1200 * 4;
        int     svg_height_cut  = svg_height * 7 / 8;
        int     svg_elm_width   = 26;
        int     svg_elm_fntsz   = 12;
        int     svg_node_xdist  = svg_elm_width;
        int     svg_node_spacr  = svg_node_xdist; //  / 2;
        int     svg_lnkwidth    = 2;

        std::string fntSans         = "sans-serif";
};

// Calculated values
class TreeGenParamsOut {

    public:

        int     gfx_ramka_x1;
        int     gfx_ramka_x2;
        int     gfx_ramka_y1;
        int     gfx_ramka_y2;

        int     gfxpos_x    [256];    // X-Координата узла
        int     gfxpos_y    [256];    // Y-Координата узла
        int     gfx_nodewl  [256];    // Ширина левой  ветки
        int     gfx_nodewr  [256];    // Ширина правой ветки
};

extern VHTree           tree;
extern VHSVG            svg;
extern TreeGenParamsIn  inparams;
extern TreeGenParamsOut oparams;


int svgcalc_layer_posy      (int layn);
int svgcalc_layer_height    ();

int svgcalc_ramka_width     ();
int svgcalc_ramka_height    ();

void CalculateTreeGfx();
void RenderTreeGfx();
