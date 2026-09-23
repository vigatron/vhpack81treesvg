#pragma once

#include "vhplatform.hpp"

class colors {

    public:

    static  std::string white;       // = "#FFFFFF";
    static  std::string orangel;

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

	static std::string magental;
	static std::string magenta;
};

struct sColor {
	std::string colf;			// Front
	std::string colb;			// Background
	std::string cols;			// Separator
	std::string coltxt;			// Text
	std::string color_shadow_b;
	std::string color_shadow_s;
};

const sColor * elm_color( bool issym, bool isroot, bool iscontr );