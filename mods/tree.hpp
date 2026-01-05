#pragma once

#include "vhplatform.hpp"
#include "tcode.hpp"
#include "svg.hpp"

#include <charconv>

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
            uint16_t v;     // count
            uint8_t  y;     // layer
            uint8_t  m;     // Mark flag, need for build by spectrum
            uint8_t  w;     // Rotation flag, LR was swapped
        };

        const char * szhex = "0123456789ABCDEF";

        VHTree() { }

        // -----------------------------------------------------------------------------
        verr buildFromTCode ( std::string strtcode ) {

            if(!tcode.initfromstr(strtcode)) return verror(1);

            scode = CreateSCodeFromTCode(tcode);
            ClearAllNodes();
            _cntlow = find_minnode_id();
            autoenumerate(0, 0);
            _depthmax = scandepth();
            // rotatenodes();

            dumplr();
            dumpnodes();
            return vok; }
 
        // -----------------------------------------------------------------------------
        verr buildFromSpectrum(std::vector<int> arr) {

            if(arr.size()<2)
                return verrmsg(1,"Can't build tree with spectrum less than <2 elms");

            ClearAllNodes();
            for( int i=0 ; i < arr.size() ; i++ ) { ooo[i].v = arr[i]; }
            
            LinkTreeFromSpectrum(arr.size());
            _cntlow = arr.size();
            scode = CreateSCodeFromHuff(huffcnt - 1);
            _depthmax = scandepth();

            dumplr();
            dumpnodes();
            return vok; }

        // -----------------------------------------------------------------------------
        void buildFromNodes ( std::string strscode) {
            std::vector<int>     lbo; // [
            std::vector<int>     lsp; // :
            std::vector<int>     lbc; // ]
            std::vector<stnode>   r;

            for(int i=0;i<strscode.size();i++) {
                char s = strscode[i];
                if(s == '[')        lbo.push_back(i);
                else if(s == ':')   lsp.push_back(i);
                else if(s == ']')   lbc.push_back(i);
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

        // -----------------------------------------------------------------------------

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

        VHTree::stobj *                 operator[]  (int idx)   { return &ooo[idx];      }

        int     getleft(int i) { return ooo[i].l; }
        int     getrigh(int i) { return ooo[i].r; }
        int     getlay (int i) { return ooo[i].y; }

        void    clrnode(int i)   {
            ooo[i].l = INV; ooo[i].r = INV; ooo[i].u = INV;
            ooo[i].y = 0; ooo[i].v = 0; ooo[i].m = 0; }
        
        void ClearAllNodes() { for(int i=0; i<512; i++) { clrnode(i); } }

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

        void dumpSCode(std::vector<stnode> & scde) {
            printf("SCode: ");
            for( const stnode & n : scde) { printf("[%2d:%d]", n.id, n.tt); }
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

        // int bitpathi(int idx) {
        //     int r = 0;
        //     int curidx = idx;
        //     while(curidx != rootidx()) {
        //         int parent = ooo[curidx].u;
        //         r += '0' + (ooo[parent].l != curidx);
        //         curidx = parent; }
        //     return r; }

        std::string bitpath(int idx) {
            std::string r;
            int curidx = idx;
            while(curidx != rootidx()) {
                int parent = ooo[curidx].u;
                r += '0' + (ooo[parent].l != curidx);
                curidx = parent; }
            std::reverse(r.begin(), r.end());
            return r; }

        char symrate_buffer[64];

        std::string symrate( int idx ) {
            std::string     bpath   = bitpath(idx);
            double          val     = bpath.size() * 1.0f / (ooo[idx].v * 8);

            auto [ptr, ec] = std::to_chars(symrate_buffer, symrate_buffer + 64, val * 100, std::chars_format::fixed, 2);
            std::string s(symrate_buffer, ptr);
            std::string r = "%" + s; // + "%)";
            return r; }

    private:

        TCode                   tcode;              // TCode
        std::vector<stnode>     scode;              // Scode

        std::vector<int>        seqlays[16];        // 2D array layers sequence

        stobj                   ooo[512];

        int                     _cntlow;
        int                     _depthmax;

        // -----------------------------------------------------------------------------
        // Huffman tree props
        // -----------------------------------------------------------------------------

        int                     huffcnt;
        // int                     huffcntsym;
        const u16               INV = 0xFFFF;


        void NSLRU(u16 i, u16 l, u16 r) { ooo[i].l = l; ooo[i].r=r; ooo[l].u=i; ooo[r].u=i; }

        void FindFirst(u16 *pidx) {
            for(;*pidx<huffcnt;(*pidx)++) if(!ooo[*pidx].m) { return; } }

        void Find2Min(u16 *r1, u16 *r2) {
            u16 idx = 0, t, min1, min2;
            FindFirst(&idx); min1=ooo[idx].v; *r1=idx++;
            FindFirst(&idx); min2=ooo[idx].v; *r2=idx++;
            // swap: min2 should be always > min1
            if(min1 > min2) {t=min2; min2=min1; min1=t; t=*r1; *r1=*r2; *r2=t;}
            for(;idx<huffcnt;idx++)
                if(!ooo[idx].m) {
                    if(ooo[idx].v<min1) {*r2=*r1;min2=min1;*r1=idx;min1=ooo[idx].v;}
                    else if(ooo[idx].v<min2) {*r2=idx;min2=ooo[idx].v;} } }

        void NLnk () { u16 idx1, idx2; Find2Min(&idx1, &idx2);
            ooo[idx1].m=1; ooo[idx2].m=1; ooo[huffcnt].v = ooo[idx1].v + ooo[idx2].v;
            dmpnlnk(idx1, idx2, huffcnt, ooo[huffcnt].v);
            NSLRU(huffcnt,idx1,idx2); ooo[huffcnt].m=0; huffcnt++; }

        void dmpnlnk(int il, int ir, int cnt, int v ) {
            printf("Link #%2d & #%2d ", il, ir); printf(" vals[%3d]=%3d\n", cnt, v); }

        verr LinkTreeFromSpectrum(int symscnt) {
            huffcnt = symscnt;
            while(symscnt-->=2) NLnk();
            return vok; }


        // -----------------------------------------------------------------------------
        std::vector<stnode> CreateSCodeFromTCode( const TCode & tcd) {
            std::vector<stnode> r;
            for(int i=0; i < tcd.sizenodes(); i++) {
                stnode nn = { .id = tcd.sizeall() - 1 - i, .tt = tcd[i] };
                r.push_back(nn); }
            printf("SCode generated from TCode : "); dumpSCode(r);
            return r; }

        // -----------------------------------------------------------------------------
        std::vector<stnode> CreateSCodeFromHuff(int rootidx) {
            std::vector<stnode> r;

            autopass(rootidx, 0);
            r = _autoscode;

            printf("SCode generated from Huffman : "); dumpSCode(r);
            return r; }

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
        int autoenumerate(int scodeidx, int lay) {

            int tid     = scode[scodeidx].id;
            int tt      = scode[scodeidx].tt;
            int r       = scodeidx;
            int layn    = lay + 1;

            if(!scodeidx) { _autoenumcnt = 0; ooo[tid].y = 0; }
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
            return r; }

        std::vector<stnode> _autoscode;

        enNodeType nodetype(int idx) {
            bool lsym = ooo[idx].l < _cntlow;
            bool rsym = ooo[idx].r < _cntlow;
            if( lsym && rsym )  return eNodeF;
            if( !lsym && !rsym) return eNodeB;
            return !lsym ? eNodeL : eNodeR; }

        // -----------------------------------------------------------------------------
        int autopass(int idx, int lay) {

            enNodeType  tt      = nodetype(idx);
            stnode      nn      = { .id = idx, .tt = tt };
            int         layn    = lay + 1;

            _autoscode.push_back(nn);
    
            int lidx = ooo[idx].l;
            int ridx = ooo[idx].r;

            ooo[lidx].y = layn;
            ooo[ridx].y = layn;

            switch(tt) {
                case eNodeL: {
                    autopass(ooo[idx].l, layn); } break;
                case eNodeR: {
                    autopass(ooo[idx].r, layn); } break;
                case eNodeB: {
                    autopass(ooo[idx].l, layn);
                    autopass(ooo[idx].r, layn); } break;
                default: { } break; }

        return idx; }
};
