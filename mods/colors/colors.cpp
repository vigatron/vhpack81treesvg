#include "colors.hpp"

std::string colors::white       = "#FFFFFF";
std::string colors::orangel     = "#FFD000";

std::string colors::yellowl     = "#FFFFE0";
std::string colors::yellowll    = "#FFFFF0";
std::string colors::myell       = "#FFFFD0";
std::string colors::nyell       = "#FFFFA0";

std::string colors::lgreen      = "#E0FFE0";
std::string colors::mgreen      = "#90C090";
std::string colors::ngreen      = "#80AF80";
std::string colors::dgreen      = "#00AF00";

std::string colors::lblue       = "#F0F0FF";
std::string colors::sblue       = "#E0E0FF";
std::string colors::mblue       = "#C0C0FF";
std::string colors::nblue       = "#8080FF";
std::string colors::dblue       = "#0000B0";

std::string colors::sgray       = "#E0E0E0";
std::string colors::mgray       = "#C0C0C0";
std::string colors::lgray       = "#A0A0A0";
std::string colors::gray        = "#303030";

std::string colors::magental	= "#ecc9ea";
std::string colors::magenta		= "#e1a8de";


// Sym Node Root Extra
const sColor  arrcolors[4] = {

	// Sym
	{	colors::mblue,
		colors::sblue,
		colors::mblue,
		"#5a72c6",
		// shadow
		std::string( "#e6e6f3" ), std::string( "#ececf0" )
	},

	// node
	{	colors::dgreen,
		colors::lgreen,
		colors::mgreen,
		"gray",
		// shadow
		"#BBFFBB", "gray",
	},

	// Root
	{	colors::orangel,
		colors::nyell,
		colors::orangel,
		"#f19253",
		// shadow
		"#f9e77f", "gray",
	 },

	// Container
	{	colors::magenta,
		colors::magental,
		colors::magenta,
		"gray",
		// shadow
		std::string( "#f4e8f4" ), std::string( "#fde7ff" ),
	 }

};

// -------------------------------------------------------------------------------------------------
const sColor * elm_color( bool issym, bool isroot, bool iscontr ) {
	if( isroot	)	return & arrcolors[2];
	if( iscontr )	return & arrcolors[3];
	if( issym	)	return & arrcolors[0];
	return & arrcolors[1]; }
