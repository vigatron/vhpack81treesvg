#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>


struct stnode    { int id; int tt; };
struct stattach  { int idx; int tt; int lay; };

class VHTree {

    public:

        VHTree() { }
        VHTree                                  ( std::string str)                      { fromstr(str); }
        void                        set         ( const std::vector<stnode> & vect)      { scode = vect; }
        void                        fromstr     (std::string strscode)                  { scode = fromstri(strscode); }
        const std::vector<stnode> &  nodes       ()                                      { return scode; }

        void build() {

            for(int i=0; i<255; i++) {
                left [i] = 0xFFFF;
                righ [i] = 0xFFFF;
                up   [i] = 0xFFFF;
                lay  [i] = 0xFFFF; }

            recurse1pass(0, 0);
            attachelms();
            asm("nop");
        }

        int cntall() { return scode[0].id; }

        void dump() {
            int i=0;
            for( const stnode & n : scode) { printf("#%d  %2d:%d\n", i++, n.id, n.tt); }
            int minid = find_min_id();
            printf("cnt = %d\n", minid); }

        void dumplr() {
            for( const stnode & n : scode) { 
                printf("%2d:%d L=%2d R=%2d\n", n.id, n.tt, left[n.id], righ[n.id]); } }

        void dumpnodes() {
            int total = cntall();
            for(int i=0; i<=total;i++) {
                printf("#%2d LY=%d\n", i, lay[i]); } }
        
        int scandepth() {
            int r = 0;
            int total = cntall();
            for(int i=0;i<total;i++) { if(lay[i] != 0xFFFF) if(lay[i]>r) r = lay[i]; }
            return r; }

        std::vector<int> findbylay(int ll) {
            std::vector<int> r;
            int total = cntall();
            for(int i=0;i<=total;i++) {
                if( lay[i] == ll ) r.push_back(i); }
            return r; }

        const std::vector<int> & seqlay(int lay) { return seqlays[lay]; }

        enum enNodeType {
            eNodeF = 0,     // F Fin
            eNodeL = 1,     // L Left
            eNodeR = 2,     // R Right
            eNodeB = 3      // B Both
        };

        void savetosvg(std::string fname) {
            std::vector<std::string> content = savetosvgi();
            
            for( const std::string & s : content) {
                printf("%s\n", s.c_str() ); }

            // Create and open a text file for writing
            std::ofstream output_file(fname); 

                // Write each string followed by a newline
                if (output_file.is_open()) {
                    for (const std::string& line : content) {
                        output_file << line << std::endl; }
                    output_file.close(); // Close the file
                    std::cout << "Data successfully saved to " << fname << std::endl;
                } else { 
                    std::cerr << "Error: Unable to open file for writing " << fname << std::endl; } }

    private:


        std::vector<stnode>      scode;             // Binary tree scode
        std::vector<stattach>    sattach;          // Link elms
        std::vector<int>         seqlays[16];       // 2D array layers sequence

        int left[256];
        int righ[256];
        int up  [256];
        int lay [256];

        // SVG related
        int gfxpos_x[256];
        int gfxpos_y[256];

        std::vector<stnode> fromstri(std::string scode) {

            std::vector<int>     lbo; // [
            std::vector<int>     lsp; // :
            std::vector<int>     lbc; // ]
            std::vector<stnode>   r;

            for(int i=0;i<scode.size();i++) {
                char s = scode[i];
                if(s == '[') lbo.push_back(i);
                else if(s == ':') lsp.push_back(i);
                else if(s == ']') lbc.push_back(i);
                else if( ! std::isdigit(s)) { i=scode.size(); return r;} }

            // Check syntax pass 1 cnt
            if( ( lbo.size() != lsp.size() ) || ( lsp.size() != lbc.size() ) )
                return r;

            // Check syntax pass 2 order
            for(int i=0; i< lbo.size();i++) {
                if( ! ( (lsp[i] - 1) - (lbo[i]) >= 1 ) ) return r;
                if( ! ( (lbc[i] - 1) - (lsp[i]) >= 1 ) ) return r; }

            for(int i=0; i< lbo.size();i++) {
                std::string sid = scode.substr(lbo[i]+1, lsp[i] - lbo[i] - 1);
                std::string sit = scode.substr(lsp[i]+1, lbc[i] - lsp[i] - 1);
                stnode nn = { .id = std::stoi(sid), .tt = std::stoi(sit) };
                r.push_back(nn); }

            return r; }

        int find_min_id() {
            int minval    = scode[0].id;
            for( int i=1; i < scode.size();i++) {
                if(scode[i].id < minval) minval = scode[i].id; }
            return minval; }

        // int _numi;
        // int inum() { 
        //     int curnum = _numi++;
        //     return curnum; }

        void LinkLeft(int idx, int parent, int curlay) {
            left    [parent]    = idx;
            lay     [idx]       = curlay + 1;
            up      [idx]       = parent;
            // seqlays [lay[idx]].push_back(idx);
            printf("LinkLeft #%d <- %d L%d\n", idx, parent, curlay);
        }

        void LinkRigh(int idx, int parent, int curlay) {
            righ    [parent]    = idx;
            lay     [idx]       = curlay + 1;
            up      [idx]       = parent;
            // seqlays [lay[idx]].push_back(idx);
            printf("LinkRigh #%d <- %d L%d\n", idx, parent, curlay);
        }

        // -----------------------------------------------------------------------------

        void pushattach(int idx, int tt, int lay) {
            stattach s = { .idx = idx, .tt = tt, .lay=lay };
            sattach.push_back(s); }

        // -----------------------------------------------------------------------------
        int recurse1pass(int i, int layn) {

            int tid = scode[i].id;
            int tt  = scode[i].tt;
            int r   = i;

            if(!i) {
                lay[tid] = 0;
                // seqlays[layn].push_back(tid);
            }

            printf("Pass1 Enter >   #%2d LAY=%2d ( %d:%d ) \n", i, layn, tid, tt);

            // if(tid == 12) {
            //     asm("nop"); }

            switch(tt) {
                case eNodeL: {
                    LinkLeft(scode[i+1].id, tid, layn);
                    r = recurse1pass(r+1, layn+1);
                    pushattach(tid, tt, layn);
                    } break;
                case eNodeR: {
                    LinkRigh(scode[i+1].id, tid, layn);
                    r = recurse1pass(i+1, layn+1);
                    pushattach(tid, tt, layn);
                } break;
                case eNodeB: {
                    LinkLeft(scode[i+1].id, tid, layn);
                    r = recurse1pass(i+1, layn+1);
                    LinkRigh(scode[r+1].id, tid, layn);
                    r = recurse1pass(r+1, layn+1);
                    } break;
                default: {
                    pushattach(tid, tt, layn);
                    } break; }

            printf("Pass1 Exit  <   #%2d LAY=%2d ( %d:%d ) \n", i, layn, tid, tt);
            return r; }

        // -----------------------------------------------------------------------------

        void attachelms() {
            int maxdepth = scandepth() + 1;
            int nidx = 0;
            for(int i = maxdepth ; i >= 0 ; i--) {
                for(const stattach & item : sattach) {
                    if(item.lay == i) {
                        if((item.tt == eNodeR) || (item.tt == eNodeF)) { LinkLeft(nidx, item.idx, item.lay); nidx++; }
                        if((item.tt == eNodeL) || (item.tt == eNodeF)) { LinkRigh(nidx, item.idx, item.lay); nidx++; }
                    } } } }

    // -------------------------------------------------------------------------------------------------
    // SVG Related
    // -------------------------------------------------------------------------------------------------
    int     svg_width  = 2000;
    int     svg_height = 1500;

    int     svg_depthmax;
    int     svg_layerh;


    std::string yellowl         = "#FFFFE0";
    std::string yellowll        = "#FFFFF0";

    std::string col_lgreen      = "#E0FFE0";
    std::string col_dgreen      = "#00AF00";

    std::string col_lblue       = "#F0F0FF";
    std::string col_sblue       = "#E0E0FF";
    std::string col_mblue       = "#C0C0FF";
    std::string col_dblue       = "#0000B0";

    std::string col_sgray       = "#E0E0E0";
    std::string col_mgray       = "#C0C0C0";
    std::string col_lgray       = "#A0A0A0";
    std::string col_gray        = "#303030";

    // -------------------------------------------------------------------------------------------------

    std::string svg_param(std::string pn, std::string pv )  { return pn + "=\"" + pv + "\" "; }

    // -------------------------------------------------------------------------------------------------
    
    std::string svg_param(std::string pn, int pv )          { return pn + "=\"" + std::to_string(pv) + "\" "; }

    // -------------------------------------------------------------------------------------------------

    std::string svg_line(std::string pfx, std::string content, bool close=true) {
        return "<" + pfx + " " + content + (close ? "/" : "") + ">"; }

    // -------------------------------------------------------------------------------------------------

    std::string svg_wnd() {
        std::string txt = "";
        txt += svg_param("width",  svg_width  );
        txt += svg_param("height", svg_height );
        txt += svg_param("xmlns", "http://www.w3.org/2000/svg");
        return svg_line("svg", txt, false); }

    // -------------------------------------------------------------------------------------------------

    std::string svg_line(int x1, int y1, int x2, int y2, int wdt, std::string col ) {
        std::string txt = "";
        txt += svg_param("x1", x1);
        txt += svg_param("y1", y1);
        txt += svg_param("x2", x2);
        txt += svg_param("y2", y2);
        if(wdt) txt += svg_param("stroke-width", wdt);
        if(!col.empty()) txt += svg_param("stroke", col);
        return svg_line("line", txt); }

    // -------------------------------------------------------------------------------------------------

    std::string svg_circ(int cx, int cy, int r, int wdt, std::string colf, std::string colb ) {
        std::string txt = "";
        txt += svg_param("cx", cx);
        txt += svg_param("cy", cy);
        txt += svg_param("r",  r);
        if(wdt) txt += svg_param("stroke-width", wdt);
        if(!colf.empty()) txt += svg_param("stroke", colf);
        if(!colb.empty()) txt += svg_param("fill", colb);
        return svg_line("circle", txt); }

    // -------------------------------------------------------------------------------------------------

    std::string svg_rect(int x, int y, int width, int height, int wdt, std::string colf, std::string colb ) {
        std::string txt = "";
        txt += svg_param("x", x);
        txt += svg_param("y", y);
        txt += svg_param("width",  width);
        txt += svg_param("height", height);
        if(wdt) txt += svg_param("stroke-width", wdt);
        if(!colf.empty()) txt += svg_param("stroke", colf);
        if(!colb.empty()) txt += svg_param("fill", colb);
        return svg_line("rect", txt); }

    // -------------------------------------------------------------------------------------------------

    std::string svg_text(int x, int y, std::string text, std::string fontfam, int fontsz, std::string col ) {
        std::string txt = "";
        txt += svg_param("x", x);
        txt += svg_param("y", y);
        txt += svg_param("font-family", fontfam);
        txt += svg_param("font-size", std::to_string(fontsz) + "px" );
        // if(wdt) txt += svg_param("stroke-width", wdt);
        // if(!colf.empty()) txt += svg_param("stroke", colf);
        if(!col.empty()) txt += svg_param("fill", col);
        return svg_line("text", txt, false) + text + "</text>"; }

    // -------------------------------------------------------------------------------------------------

    std::vector<std::string> draw_background() {
        std::vector<std::string> r;
        for(int ll=0; ll <= svg_depthmax; ll++) {
            int posy = ll * svg_layerh;
            std::string color = (ll & 1) ? yellowll : yellowl;
            std::string fig   = svg_rect(0, posy, svg_width, svg_layerh, 0, color, color );
            r.push_back( fig ); }
        return r; }

    // -------------------------------------------------------------------------------------------------

    std::vector<std::string> draw_elm(int idx) {
        std::vector<std::string> r;
        int elms = find_min_id();

        bool fine = idx < elms;

        int cir_d = 120;
        int qua_w = cir_d * 0.8;
        
        int fntsz = 70;
        int fontw = fntsz * 3 / 4;
        int fonth = fntsz * 4 / 7;

        int cx = gfxpos_x[idx];
        int cy = gfxpos_y[idx];

        int qx = gfxpos_x[idx] - qua_w/2;
        int qy = gfxpos_y[idx] - qua_w/2;

        int tx = gfxpos_x[idx] - ((idx>9) ? (fontw * 3 / 5) : (fontw / 4));
        int ty = gfxpos_y[idx] + (fonth / 2);

        int th = 6;

        std::string colf = fine ? col_dblue : col_dgreen;
        std::string colb = fine ? col_sblue : col_lgreen;

        std::string font = "sans-serif";

        std::string fig = fine ?
            svg_rect(qx, qy, qua_w, qua_w,  th, colf, colb ) :
            svg_circ(cx, cy, cir_d/2,       th, colf, colb);

        std::string txt_idx = svg_text(tx, ty, std::to_string(idx), font, fontw, col_gray );

        r.push_back( fig );
        r.push_back( txt_idx );

        return r; }

    // -------------------------------------------------------------------------------------------------

    std::vector<std::string> draw_layerelms(int ll) {
        std::vector<std::string> r;
        // std::vector<int> layids = findbylay(ll);
        std::vector<int> layids = seqlay(ll);
        int gfxstpx = svg_width / (layids.size() + 1);
        for(int i=0; i<layids.size();i++) {
            int idx = layids[i]; // FIX !
            gfxpos_x[idx] = (i+1) * gfxstpx;
            gfxpos_y[idx] = (ll * svg_layerh) + (svg_layerh>>1);
            std::vector<std::string> tmp = draw_elm(idx);
            r.insert(r.end(), tmp.begin(), tmp.end()); }
        return r; }

    // -------------------------------------------------------------------------------------------------

    std::vector<std::string> draw_layerselms() {
        std::vector<std::string> r;
        for(int ll=0; ll <= svg_depthmax; ll++) {
            std::vector<std::string> tmp = draw_layerelms(ll);
            r.insert(r.end(), tmp.begin(), tmp.end()); }
        return r; }

    std::vector<std::string> draw_tstlink(int idx1, int idx2) {
        std::vector<std::string> r;
        int x1 = gfxpos_x[idx1];
        int y1 = gfxpos_y[idx1];
        int x2 = gfxpos_x[idx2];
        int y2 = gfxpos_y[idx2];
        r.push_back( svg_line(x1, y1, x2, y2, 6, col_mgray) );
        return r; }

    // -------------------------------------------------------------------------------------------------

    std::vector<std::string> savetosvgi() {

        std::vector<std::string> r;
        std::vector<std::string> tmp;

        svg_depthmax = scandepth();
        svg_layerh   = svg_height / (svg_depthmax + 1);

        r.push_back( svg_wnd() );
        tmp = draw_background();    r.insert(r.end(), tmp.begin(), tmp.end() );

        tmp = draw_layerselms();    r.insert(r.end(), tmp.begin(), tmp.end() );

        tmp = draw_tstlink(16, 15); r.insert(r.end(), tmp.begin(), tmp.end() );
        tmp = draw_tstlink(16,  8); r.insert(r.end(), tmp.begin(), tmp.end() );
        tmp = draw_tstlink(15, 13); r.insert(r.end(), tmp.begin(), tmp.end() );

        r.push_back("</svg>");
        return r; }
};
