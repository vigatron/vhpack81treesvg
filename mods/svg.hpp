#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>


class VHSVG {

    public:

        void append( const std::string & content ) {
            svg_content.push_back(content); }

        void append( const std::vector<std::string> & content) {
            svg_content.insert(svg_content.end(), content.begin(), content.end() ); }

        void savetosvg(std::string fname) {

            // Create and open a text file for writing
            std::ofstream output_file(fname); 

            // Write each string followed by a newline
            if (output_file.is_open()) {
                for (const std::string& line : svg_content) {
                    output_file << line << std::endl; }
                output_file.close(); // Close the file
                std::cout << "Data successfully saved to " << fname << std::endl;
            } else { 
                std::cerr << "Error: Unable to open file for writing " << fname << std::endl; } }


    // -------------------------------------------------------------------------------------------------

    std::string svg_param(std::string pn, std::string pv )  { return pn + "=\"" + pv + "\" "; }

    // -------------------------------------------------------------------------------------------------
    
    std::string svg_param(std::string pn, int pv )          { return pn + "=\"" + std::to_string(pv) + "\" "; }

    // -------------------------------------------------------------------------------------------------

    std::string svg_tline(std::string pfx, std::string content, bool close=true) {
        return "<" + pfx + " " + content + (close ? "/" : "") + ">"; }

    // -------------------------------------------------------------------------------------------------

    void line(int x1, int y1, int x2, int y2, int wdt, std::string col ) {
        std::string txt = "";
        txt += svg_param("x1", x1);
        txt += svg_param("y1", y1);
        txt += svg_param("x2", x2);
        txt += svg_param("y2", y2);
        if(wdt) txt += svg_param("stroke-width", wdt);
        if(!col.empty()) txt += svg_param("stroke", col);
        append( svg_tline("line", txt) ); }

    // -------------------------------------------------------------------------------------------------

    void circ(int cx, int cy, int r, int wdt, std::string colf, std::string colb ) {
        std::string txt = "";
        txt += svg_param("cx", cx);
        txt += svg_param("cy", cy);
        txt += svg_param( "r",  r);
        if(wdt) txt += svg_param("stroke-width", wdt);
        if(!colf.empty()) txt += svg_param("stroke", colf);
        if(!colb.empty()) txt += svg_param("fill", colb);
        append( svg_tline("circle", txt) ); }

    // -------------------------------------------------------------------------------------------------

    void rect(int x, int y, int width, int height, int wdt, std::string colf, std::string colb = "none", int rr = 0 ) {
        std::string txt = "";
        txt += svg_param("x", x);
        txt += svg_param("y", y);
        txt += svg_param("width",  width);
        txt += svg_param("height", height);
        if(rr) { txt += svg_param("rx", rr); txt += svg_param("ry", rr); }
        if(wdt) txt += svg_param("stroke-width", wdt);
        if(!colf.empty()) txt += svg_param("stroke", colf);
        if(!colb.empty()) txt += svg_param("fill", colb);
        append( svg_tline("rect", txt) ); }

    // -------------------------------------------------------------------------------------------------

    void text(int x, int y, std::string text, std::string fontfam, int fontsz, std::string col ) {
        std::string txt = "";
        txt += svg_param("x", x);
        txt += svg_param("y", y);
        txt += svg_param("font-family", fontfam);
        txt += svg_param("font-size", std::to_string(fontsz) + "px" );
        // if(wdt) txt += svg_param("stroke-width", wdt);
        // if(!colf.empty()) txt += svg_param("stroke", colf);
        if(!col.empty()) txt += svg_param("fill", col);
        append( svg_tline("text", txt, false) + text + "</text>" ); }

    // -------------------------------------------------------------------------------------------------

    void begin( int svg_width , int svg_height ) {
        std::string txt = "";
        txt += svg_param("width",  svg_width  );
        txt += svg_param("height", svg_height );
        txt += svg_param("xmlns", "http://www.w3.org/2000/svg");
        append( svg_tline("svg", txt, false) ); }

    void end() { 
        append("</svg>"); }

    private:

        std::vector<std::string>    svg_content;

};
