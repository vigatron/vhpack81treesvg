#include "draw.hpp"

void draw_reidxtbl( const VHRect & rect ) {
	
	// svgrect( rect, 1, "red");

	std::vector<int> arr = oparams.tblreidx;

	int rowhh	= rect.h / 8;
	int rest	= arr.size() % 8;
	int cols	= ( arr.size() / 8 ) + (rest ? 1:0);
	int rows	= 8;

	for( int i = 0; i < arr.size(); i++) {
		
		int col = i / 8;
		int row = i % 8;
		
		std::string s = "IDX-H #" + std::to_string( i ) + " : IDX-T #" + std::to_string( arr[i] );

		int x = rect.sx + col * rect.w / cols ;
		int y = rect.sy + row * rowhh; 

		std::string clr = "#606060";

		svg.text( x + 20, y + rowhh * 0.75, s, iparams.fntSans, 11, clr );
		svg.circ( x + 10, y + rowhh/2, 1, 1, clr);
	}
}