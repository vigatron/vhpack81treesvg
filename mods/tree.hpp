#pragma once

#include "vhplatform.hpp"
#include "tcode.hpp"
#include "svg.hpp"

struct stnode    { int id; int tt; };

class VHTree {

    public:

        enum enNodeType {
            eNodeF = 0,     // F Fin
            eNodeL = 1,     // L Left
            eNodeR = 2,     // R Right
            eNodeB = 3      // B Both
        };

        struct stobj {
            uint16_t l;     // left
            uint16_t r;     // right
            uint16_t u;     // up
            uint16_t y;     // layer
        };

        const char * szhex = "0123456789ABCDEF";

        VHTree() { }

        verr buildFromTCode ( std::string strtcode ) {
            if(!tcode.initfromstr(strtcode)) return verror(1);
            scode.clear();
            for(int i=0; i < tcode.sizenodes(); i++) {
                stnode nn = { .id = tcode.sizeall() - 1 - i, .tt = tcode[i] };
                scode.push_back(nn); }
            dumpscode(); 
            build();
            dumplr();
            dumpnodes();
            return vok; }

        verr buildFromSpectrum(std::vector<int> arr) {

            return verror(1);
        }

        void build() {
            for(int i=0; i<255; i++) { clrnode(i); }
            _cntlow = find_minnode_id();
            // recurse1pass(0, 0);
            // attachelms();
            autoenumerate(0, 0);
            // exit(10);
            // rotatenodes();
            _depthmax = scandepth();
            asm("nop"); }


        void fromstr ( std::string strscode) {
            std::vector<int>     lbo; // [
            std::vector<int>     lsp; // :
            std::vector<int>     lbc; // ]
            std::vector<stnode>   r;

            for(int i=0;i<strscode.size();i++) {
                char s = strscode[i];
                if(s == '[') lbo.push_back(i);
                else if(s == ':') lsp.push_back(i);
                else if(s == ']') lbc.push_back(i);
                else if( ! std::isdigit(s)) { i=strscode.size(); return;} }

            // Check syntax pass 1 cnt
            if( ( lbo.size() != lsp.size() ) || ( lsp.size() != lbc.size() ) )
                return;

            // Check syntax pass 2 order
            for(int i=0; i< lbo.size();i++) {
                if( ! ( (lsp[i] - 1) - (lbo[i]) >= 1 ) ) return;
                if( ! ( (lbc[i] - 1) - (lsp[i]) >= 1 ) ) return; }

            for(int i=0; i< lbo.size();i++) {
                std::string sid = strscode.substr(lbo[i]+1, lsp[i] - lbo[i] - 1);
                std::string sit = strscode.substr(lsp[i]+1, lbc[i] - lsp[i] - 1);
                stnode nn = { .id = std::stoi(sid), .tt = std::stoi(sit) };
                r.push_back(nn); } }

        void                            set         ( const std::vector<stnode> & vect)     { scode = vect; }
        void                            fromscd     ( std::vector<unsigned char> & arr )    { scode = fromscdi(arr); }
        void                            fromtcd     ( std::vector<unsigned char> & arr )    { scode = fromtcdi(arr); }
        const std::vector<stnode> &     nodes       ( )                                     { return scode; }

        int                             cntall      ()          { return scode[0].id;    }
        int                             rootidx     ()          { return cntall();       }
        int                             cntsyms     ()          { return _cntlow;        }
        int                             depthmax    ()          { return _depthmax;      }
        bool                            issym       (int idx)   { return idx < _cntlow;  }
        bool                            isnode      (int idx)   { return idx >= _cntlow; }

        int     getleft(int i) { return ooo[i].l; }
        int     getrigh(int i) { return ooo[i].r; }
        int     getlay (int i) { return ooo[i].y; }

        void    clrnode(int i) { ooo[i].l = 0xFFFF; ooo[i].r = 0xFFFF; ooo[i].u = 0xFFFF; ooo[i].y = 0xFFFF; }

        std::string astext() {
            std::string r;
            int ss = scode.size();
            r += szhex[ ((scode.size() - 1) >> 4) & 0xF ];
            r += szhex[ ((scode.size() - 1) >> 0) & 0xF ];
            for(const stnode & item :scode) { r += '0' + item.tt; }
            return r; }

        std::string bintohex( const std::vector<unsigned char> & arr ) {
            std::string r;
            for( const unsigned char s : arr) { 
                r.push_back(szhex[ (s>>4) & 0xF ]); r.push_back(szhex[ (s>>0) & 0xF ]); }
            return r; }

        std::string asbin() {
            std::vector<unsigned char> r;
            r.push_back( scode.size() - 1 );
            unsigned char v   = 0;
            unsigned char msk = 0x80;

            for(int i=0; i< scode.size(); i++ ) {
                const stnode & item = scode[i];
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
                if(!msk) { msk = 0x80; r.push_back(v); v = 0; } }
            
            if(msk != 0x80) r.push_back(v);
            return bintohex(r); }

        void rotatenodes() {
            for(int i = 0; i <= scode.size(); i++) {
                if(scode[i].tt == 2) {
                    scode[i].tt = 1;
                    int swpidx = scode[i].id;
                    swaplr(swpidx); } } }

        void swaplr(int idx) { int tmp = ooo[idx].l; ooo[idx].l = ooo[idx].r; ooo[idx].r = tmp; }

        void dumpscode() {
            printf("SCode: ");
            for( const stnode & n : scode) { printf("[%2d:%d]", n.id, n.tt); }
            printf("\n"); }

        void dumplr() {
            for( const stnode & n : scode) { printf("%2d:%d L=%2d R=%2d\n", n.id, n.tt, ooo[n.id].l, ooo[n.id].r); } }

        void dumpnodes() {
            int total = cntall(); for(int i=0; i<=total;i++) { printf("#%2d LY=%d\n", i, ooo[i].y); } }
        
        int scandepth() {
            int r = 0;
            int total = cntall();
            for(int i=0;i<total;i++) { if(ooo[i].y != 0xFFFF) if(ooo[i].y > r) r = ooo[i].y; }
            return r; }

        std::vector<int> findbylay(int ll) {
            std::vector<int> r;
            int total = cntall();
            for(int i=0;i<=total;i++) { if( ooo[i].y == ll ) r.push_back(i); }
            return r; }

        const std::vector<int> & seqlay(int lay) { return seqlays[lay]; }

        void dump_str_array(std::vector<std::string> & content) {
            for( const std::string & s : content) { printf("%s\n", s.c_str() ); } }

        std::string bitpath(int idx) {
            std::string r;
            int curidx = idx;
            while(curidx != rootidx()) {
                int parent = ooo[curidx].u;
                r += '0' + (ooo[parent].l != curidx);
                curidx = parent; }
            std::reverse(r.begin(), r.end());
            return r; }

    private:

        TCode                   tcode;
        std::vector<stnode>     scode;             // Binary tree scode
        std::vector<int>        seqlays[16];       // 2D array layers sequence

        stobj                   ooo[512];

        int                     _cntlow;
        int                     _depthmax;

        // -----------------------------------------------------------------------------

        std::vector<stnode> fromscdi ( std::vector<unsigned char> & arr ) {
            std::vector<stnode> r;
            return r; }

        // -----------------------------------------------------------------------------

        std::vector<stnode> fromtcdi ( std::vector<unsigned char> & arr ) {
            std::vector<stnode> r;
            return r; }

        // -----------------------------------------------------------------------------

        int find_minnode_id() {
            int minval    = scode[0].id;
            for( int i=1; i < scode.size();i++) { if(scode[i].id < minval) minval = scode[i].id; }
            return minval; }

        void LinkLeft(int idx, int parent, int curlay) {
            ooo[parent].l    = idx;
            ooo[idx].y       = curlay;
            ooo[idx].u       = parent;
            printf("LinkLeft #%d L%d <- %d \n", idx, curlay, parent ); }

        void LinkRigh(int idx, int parent, int curlay) {
            ooo[parent].r    = idx;
            ooo[idx].y       = curlay;
            ooo[idx].u       = parent;
            printf("LinkRigh #%d L%d <- %d \n", idx, curlay, parent ); }

        int _autoenumcnt;

        // -----------------------------------------------------------------------------
        int autoenumerate(int tcodeidx, int lay) {

            int tid     = scode[tcodeidx].id;
            int tt      = scode[tcodeidx].tt;
            int r       = tcodeidx;
            int layn    = lay + 1;

            if(!tcodeidx) { _autoenumcnt = 0; ooo[tid].y = 0; }
            printf("AUTOENUM Enter >   #%2d LAY=%2d [%d:%d] )\n", tid, lay, tid, tt);

            switch(tt) {
                case eNodeL: {
                    LinkLeft(scode[r+1].id,  tid, layn);         // Node link
                    LinkRigh(_autoenumcnt++, tid, layn);         // Sym link
                    r = autoenumerate(r+1, layn);
                    } break;
                case eNodeR: {
                    LinkLeft(_autoenumcnt++, tid, layn);
                    LinkRigh(scode[r+1].id,  tid, layn);
                    r = autoenumerate(r+1, layn);
                } break;
                case eNodeB: {
                    LinkLeft(scode[r+1].id, tid, layn);
                    r = autoenumerate(r+1, layn);
                    LinkRigh(scode[r+1].id, tid, layn);
                    r = autoenumerate(r+1, layn);
                    } break;
                default: {
                    LinkLeft(_autoenumcnt++, tid, layn);
                    LinkRigh(_autoenumcnt++, tid, layn);
                    } break; }
            
            printf("AUTOENUM Exit  <   #%2d LAY=%2d [%d:%d] )\n", tid, lay, tid, tt);
            return r;
        }
};
