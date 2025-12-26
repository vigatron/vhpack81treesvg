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
        void                            set     ( const std::vector<stnode> & vect)     { scode = vect; }
        void                            fromstr (std::string strscode)                  { scode = fromstri(strscode); }
        const std::vector<stnode> &     nodes   ( )                                     { return scode; }

        void clrnode(int i) {
            left [i] = 0xFFFF; righ [i] = 0xFFFF; up [i] = 0xFFFF; lay [i] = 0xFFFF; }

        const char * szhex = "0123456789ABCDEF";

        std::string astext() {
            std::string r;
            r.push_back(szhex[ scode.size() - 1]);
            for(const stnode & item :scode) {
                r += '0' + item.tt; }
            return r; }

        std::string bintohex( const std::vector<unsigned char> & arr ) {
            std::string r;
            for( const unsigned char s : arr) {
                r.push_back(szhex[ (s>>4) & 0xF ]);
                r.push_back(szhex[ (s>>0) & 0xF ]); }
            return r; }

        std::string asbin() {
            std::vector<unsigned char> r;
            unsigned char v = (scode.size() - 1) << 4;
            unsigned char msk = 8;

            for(const stnode & item :scode) {
                unsigned char b = item.tt;
                
                switch(msk) {
                    case 0x80: v |= b << 6; break;
                    case 0x40: v |= b << 5; break;
                    case 0x20: v |= b << 4; break;
                    case 0x10: v |= b << 3; break;
                    case 0x08: v |= b << 2; break;
                    case 0x04: v |= b << 1; break;
                    case 0x02: v |= b << 0; break;
                    case 0x01: v |= b << 0; break;
                    default: break; }

                msk >>= 2;
                if(!msk) {
                    msk = 0x80;
                    r.push_back(v);
                    v = 0; } }
            
            if(msk != 0x80) r.push_back(v);
            return bintohex(r); }

        void build() {
            for(int i=0; i<255; i++) { clrnode(i); }
            _cntlow = find_min_id();
            recurse1pass(0, 0);
            attachelms();
            asm("nop"); }

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

        void dump_str_array(std::vector<std::string> & content) {
            for( const std::string & s : content) {
                printf("%s\n", s.c_str() ); } }

        void savetosvg(std::string fname) {
            std::vector<std::string> content = savetosvgi();

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
        std::vector<stattach>    sattach;           // Link elms
        std::vector<int>         seqlays[16];       // 2D array layers sequence

        int left[256];
        int righ[256];
        int up  [256];
        int lay [256];
        int _cntlow;


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
    int     svg_width       = 2000;
    int     svg_height      = 1200;
    int     svg_height_cut  = svg_height * 6 / 8;

    int     svg_elm_width   = 26;
    int     svg_elm_fntsz   = 12;

    int     svg_node_xdist = svg_elm_width;
    int     svg_node_spacr = svg_node_xdist;
    int     svg_lnkwidth   = 2;

    int     svg_depthmax;

    int     gfxpos_x[256];
    int     gfxpos_y[256];

    int     gfx_nodewl[256]; // Ширина левой  ветки
    int     gfx_nodewr[256]; // Ширина правой ветки

    int     gfx_ramka_x1;
    int     gfx_ramka_x2;
    int     gfx_ramka_y1;
    int     gfx_ramka_y2;

    // -------------------------------------------------------------------------------------------------
    // SVG Calculations
    // -------------------------------------------------------------------------------------------------

    int svgcalc_layer_height    ()          {
        int activeh = svg_height - svg_height_cut;
        return activeh / (svg_depthmax + 1); }

    int svgcalc_layer_posy(int layn)  {
        int posy = svg_height_cut/2 + layn * svgcalc_layer_height();
        return posy; }

    int svgcalc_layer_centerposy(int layn) {
        return svgcalc_layer_posy(layn) + svgcalc_layer_height()/2; }

    std::string col_white       = "#FFFFFF";

    std::string yellowl         = "#FFFFE0";
    std::string yellowll        = "#FFFFF0";
    std::string col_myell       = "#FFFFD0";
    std::string col_nyell       = "#FFFFA0";

    std::string col_lgreen      = "#E0FFE0";
    std::string col_mgreen      = "#90C090";
    std::string col_ngreen      = "#80AF80";
    std::string col_dgreen      = "#00AF00";

    std::string col_lblue       = "#F0F0FF";
    std::string col_sblue       = "#E0E0FF";
    std::string col_mblue       = "#C0C0FF";
    std::string col_nblue       = "#8080FF";
    std::string col_dblue       = "#0000B0";

    std::string col_sgray       = "#E0E0E0";
    std::string col_mgray       = "#C0C0C0";
    std::string col_lgray       = "#A0A0A0";
    std::string col_gray        = "#303030";

    std::string fntSans         = "sans-serif";

    std::vector<std::string> _svg_content;

    void svgcontent_append( const std::string & content ) {
        _svg_content.push_back(content); }

    void svgcontent_append( const std::vector<std::string> & content) {
        _svg_content.insert(_svg_content.end(), content.begin(), content.end() ); }

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
        txt += svg_param( "r",  r);
        if(wdt) txt += svg_param("stroke-width", wdt);
        if(!colf.empty()) txt += svg_param("stroke", colf);
        if(!colb.empty()) txt += svg_param("fill", colb);
        return svg_line("circle", txt); }

    // -------------------------------------------------------------------------------------------------

    std::string svg_rect(int x, int y, int width, int height, int wdt, std::string colf, std::string colb = "none", int rr = 0 ) {
        std::string txt = "";
        txt += svg_param("x", x);
        txt += svg_param("y", y);
        txt += svg_param("width",  width);
        txt += svg_param("height", height);
        if(rr) { txt += svg_param("rx", rr); txt += svg_param("ry", rr); }
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

        r.push_back( svg_rect(0, 0, svg_width, svg_height, 0, col_white, col_white ) );

        int w = gfx_ramka_x2 - gfx_ramka_x1;

        for(int ll=0; ll <= svg_depthmax; ll++) {
            int layposy = svgcalc_layer_posy(ll);
            std::string color = (ll & 1) ? yellowll : yellowl;
            std::string fig   = svg_rect(gfx_ramka_x1, svgcalc_layer_posy(ll), w, svgcalc_layer_height(), 0, color, color );
            r.push_back( fig ); }
        return r; }

    // -------------------------------------------------------------------------------------------------

    void svgcalc_ramka() {

        int minleft     = svg_width;
        int maxright    = 0;
        int mintop      = svg_height;
        int maxbot      = 0;

        for(int i=0; i <= cntall(); i++ ) {
            if(gfxpos_x[i] < minleft    ) { minleft  = gfxpos_x[i]; }
            if(gfxpos_x[i] > maxright   ) { maxright = gfxpos_x[i]; }
            if(gfxpos_y[i] < mintop     ) { mintop   = gfxpos_y[i]; }
            if(gfxpos_y[i] > maxbot     ) { maxbot   = gfxpos_y[i]; } }

        int dx = 80;

        minleft     -= dx;
        maxright    += dx;
        mintop      -= dx;
        maxbot      += dx;

        // copy
        gfx_ramka_x1    = minleft;
        gfx_ramka_x2    = maxright;
        gfx_ramka_y1    = mintop;
        gfx_ramka_y2    = maxbot;

    }


    void svgcalc_w(int idx, int * retwl, int * retwr) {

        int idxl = left[idx];
        int idxr = righ[idx];
        int wl, wr;

        if( idxl < _cntlow) {
            int ws = svg_node_xdist + svg_node_spacr;
            gfx_nodewl[idx ] = ws;
            gfx_nodewl[idxl] = ws / 2;
            gfx_nodewr[idxl] = ws / 2;
        } else {
            svgcalc_w(idxl, &wl, &wr);
            gfx_nodewl[idx] = wl + wr;
        }

        if( idxr < _cntlow) {
            int ws = svg_node_xdist + svg_node_spacr;
            gfx_nodewr[idx ] = ws;
            gfx_nodewl[idxr] = ws / 2;
            gfx_nodewr[idxr] = ws / 2;
        } else {
            svgcalc_w(idxr, &wl, &wr);
            gfx_nodewr[idx]  = wl + wr;
        }

        *retwl = gfx_nodewl[idx];
        *retwr = gfx_nodewr[idx];
        printf("W%2d  (L:%d R:%d) S:%d\n", idx, *retwl, *retwr, *retwl + *retwr);

    }

    // -------------------------------------------------------------------------------------------------

    void svgcalc_set_coords(int idx, int nodex) {

        gfxpos_x[idx] = nodex;
        gfxpos_y[idx] = svgcalc_layer_centerposy(lay[idx]);

        if(idx >= _cntlow) {
            int lnodex = nodex - gfx_nodewr[ left[idx] ];
            svgcalc_set_coords( left[idx], lnodex );
            int rnodex = nodex + gfx_nodewl[ righ[idx] ];
            svgcalc_set_coords( righ[idx], rnodex );
        } }

    // -------------------------------------------------------------------------------------------------

    std::vector<std::string> draw_link(int idx1, int idx2) {
        std::vector<std::string> r;
        int x1 = gfxpos_x[idx1];
        int y1 = gfxpos_y[idx1];
        int x2 = gfxpos_x[idx2];
        int y2 = gfxpos_y[idx2];
        r.push_back( svg_line(x1, y1, x2, y2, svg_lnkwidth, col_mgreen) );
        return r; }

    // -------------------------------------------------------------------------------------------------
    std::vector<std::string> draw_links(int idx) {
        std::vector<std::string> r;
        std::vector<std::string> tmp;
        if(idx >= _cntlow ) {
            tmp = draw_link(idx, left[idx]); r.insert(r.end(), tmp.begin(), tmp.end());
            tmp = draw_link(idx, righ[idx]); r.insert(r.end(), tmp.begin(), tmp.end());
            tmp = draw_links(left[idx]);     r.insert(r.end(), tmp.begin(), tmp.end());
            tmp = draw_links(righ[idx]);     r.insert(r.end(), tmp.begin(), tmp.end()); }
        return r; }

    // -------------------------------------------------------------------------------------------------

    std::vector<std::string> draw_elm(int idx) {
        std::vector<std::string> r;
        int elms = find_min_id();

        bool flagsym = idx < elms;

        int cx = gfxpos_x[idx];
        int cy = gfxpos_y[idx];

        int qx = gfxpos_x[idx] - svg_elm_width/2;
        int qy = gfxpos_y[idx] - svg_elm_width/2;
        int th = 1.5;

        int fontw = svg_elm_fntsz * 3 / 4;
        int fonth = svg_elm_fntsz * 4 / 7;
        int tx = gfxpos_x[idx] - ((idx>9) ? (fontw * 3 / 5) : (fontw / 4));
        int ty = gfxpos_y[idx] + (fonth / 2);

        std::string colf = flagsym ? col_mblue : col_dgreen;
        std::string colb = flagsym ? col_sblue : col_lgreen;

        // Debug staff : Rectangle [WL|WR]
        bool show_wl_range = false;
        if(show_wl_range) {
            int sx = gfxpos_x[idx] - gfx_nodewl[idx];
            int ww = gfx_nodewl[idx] + gfx_nodewr[idx];
            r.push_back( svg_rect(sx, qy, ww, svg_elm_width, 1, "black", "orange" ) );
        }

        if(!flagsym) {
            r.push_back(svg_circ(cx, cy, svg_elm_width*5/8, th*3/8, colf, "white")); }

        std::string fig = flagsym ?
            svg_rect(qx, qy, svg_elm_width, svg_elm_width, th, colf, colb, svg_elm_width * 0.2 ) :
            svg_circ(cx, cy, svg_elm_width/2,   th, colf, colb);
        r.push_back( fig );

        std::string txt_idx = svg_text(tx, ty, std::to_string(idx), fntSans, fontw, col_gray );
        r.push_back( txt_idx );

        // Props print
        bool show_props = false;
        if(show_props) {
            r.push_back( svg_text(tx, ty - 30, std::to_string(gfxpos_x[idx]), fntSans, 10, "black" ) );
            std::string wlwr = std::to_string(gfx_nodewl[idx]) + ":" + std::to_string(gfx_nodewr[idx]);
            r.push_back( svg_text(tx, ty - 20, wlwr, fntSans, 10, "black" ) );
        }

        return r; }

    // -------------------------------------------------------------------------------------------------

    std::vector<std::string> draw_elems(int idx) {
        std::vector<std::string> r;
        std::vector<std::string> tmp;
        
        tmp = draw_elm(idx); r.insert(r.end(), tmp.begin(), tmp.end());

        if(idx >= _cntlow ) {
            tmp = draw_elems(left[idx]);    r.insert(r.end(), tmp.begin(), tmp.end());
            tmp = draw_elems(righ[idx]);    r.insert(r.end(), tmp.begin(), tmp.end()); }

        return r; }

    // -------------------------------------------------------------------------------------------------

    void draw_layers() {
        for(int i=0; i <= svg_depthmax; i++) {
            int cx = gfx_ramka_x1 + 18;
            int cy = svgcalc_layer_posy(i) + svgcalc_layer_height()/2;
            svgcontent_append ( svg_circ(cx + 4, cy-2, svg_elm_width/2, 1, col_nyell, col_myell) );
            svgcontent_append ( svg_text(cx, cy, "L" + std::to_string(i), fntSans, 9, col_lgray) );
        }
    }

    // -------------------------------------------------------------------------------------------------

    void draw_scode() {
        
        std::string str = asbin() + " : " + astext(); // TCode : SCode
        int w = (gfx_ramka_x2 - gfx_ramka_x1)/2;
        int dd = 4;
        svgcontent_append ( svg_rect(gfx_ramka_x1 + dd, gfx_ramka_y1 + dd, w - dd*2, 46, 1, col_lblue, col_lblue, 8));
        svgcontent_append ( svg_text(gfx_ramka_x1 + 18, gfx_ramka_y1 + 32, str, fntSans, 16, col_nblue ) );
    }

    // -------------------------------------------------------------------------------------------------

    void draw_ramka() {

        int w = gfx_ramka_x2 - gfx_ramka_x1;
        int h = gfx_ramka_y2 - gfx_ramka_y1;

        svgcontent_append ( svg_rect(gfx_ramka_x1, gfx_ramka_y1, w, h, 1.5, col_nblue, "none", 12 ) );
        svgcontent_append ( svg_text(gfx_ramka_x1 +  10, gfx_ramka_y1 - 10, "Визуализатор деревьев Хаффмана", fntSans, 15, col_lgray) );
        svgcontent_append ( svg_text(gfx_ramka_x2 - 140, gfx_ramka_y1 - 10, "V1.00 V01G04A81 (C) 2025", fntSans, 10, col_lgray) );

    }

    // -------------------------------------------------------------------------------------------------

    std::vector<std::string> savetosvgi() {

        std::vector<std::string> tmp;

        svg_depthmax = scandepth();
        
        {
            int wl, wr;
            svgcalc_w(scode[0].id, & wl, & wr); // recurse
            svgcalc_set_coords(scode[0].id, svg_width / 2); // recurse, start from root
            svgcalc_ramka();
        }

        svgcontent_append( svg_wnd() );
        svgcontent_append( draw_background() );
        svgcontent_append( draw_links( cntall() ) ); // recurse
        svgcontent_append( draw_elems( cntall() ) ); // recurse

        draw_layers();
        draw_scode();
        draw_ramka();

        // r.push_back( svg_rect( 0, 0, svg_elm_width,   svg_elm_width, 1, col_gray, "none" ));
        // r.push_back( svg_circ(10,10, svg_elm_width/2,                1, col_gray, "none" ));

        svgcontent_append("</svg>");
        return _svg_content; }

};
