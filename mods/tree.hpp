#pragma once

#include "vhplatform.hpp"
#include "tcode.hpp"
#include "treearch.hpp"
#include "svg.hpp"

#include <charconv>


class VHTree {

    public:

        struct stnode    { int id; int tt; };

        VHTree() { }

        // -----------------------------------------------------------------------------
        verr buildFromTCode ( std::string strtcode , std::vector<int> rotints) {

            if(!tcode.initfromstr(strtcode)) return verror(1);

            arrnodes = CreateSCodeFromTCode(tcode);
            tarch.ClearAllNodes();
            tarch.setsymscount( find_minnode_id() );
            autoenumerate(0, 0);
            tarch.CalculateMaxDepth();

            InternalAutorotation(rotints);

            dumplr();
            dumpnodes();
            return vok; }
 
        // -----------------------------------------------------------------------------
        verr buildFromSpectrum(std::vector<int> arrspc, std::vector<int> rotints) {

            if( arrspc.size() < 2 )
                return verrmsg(1,"Can't build tree with spectrum less than <2 elms");

            tarch.ClearAllNodes();
            for( int i=0 ; i < arrspc.size() ; i++ ) { tarch.SetCountVal(i, arrspc[i]); }

            tarch.LinkTreeFromSpectrum( arrspc.size());
            tarch.setsymscount( arrspc.size() );
            arrnodes = CreateSCodeFromHuff( tarch.size() - 1);
            tarch.CalculateMaxDepth();

            InternalAutorotation(rotints);

            dumplr();
            dumpnodes();
            return vok; }

        // -----------------------------------------------------------------------------
        void InternalAutorotation(std::vector<int> rotints) {
            for(int rotidx : rotints) { tarch.SetSwapFlag(rotidx,1); }
            // rotatenodes();
        }

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

        // void set ( const std::vector<stnode> & vect) { arrnodes = vect; }
        // const std::vector<stnode> & nodes ( ) { return arrnodes; }

        int                             cntall      ()          { return arrnodes[0].id;    }
        int                             rootidx     ()          { return cntall();          }

        const VHTreeArch  &             arch        ()          { return tarch; }
        VHTreeArch::stobj *             operator[]  (int idx)   { return tarch[idx];        }

        int     getleft(int i) { return tarch.getleft(i); }
        int     getrigh(int i) { return tarch.getrigh(i); }
        int     getlay (int i) { return tarch.getlay (i); }
        bool    getswap(int i) { return tarch.getswap(i); }

        std::string SCodeToText() {
            std::string r;
            int ss = arrnodes.size();
            r += szhex()[ ((arrnodes.size() - 1) >> 4) & 0xF ];
            r += szhex()[ ((arrnodes.size() - 1) >> 0) & 0xF ];
            for(const stnode & item :arrnodes) { r += '0' + item.tt; }
            return r; }

        std::string SCodeToTCode() {
            std::vector<unsigned char> r;
            r.push_back( arrnodes.size() - 1 );
            unsigned char v   = 0;
            unsigned char msk = 0x80;

            for(int i=0; i< arrnodes.size(); i++ ) {
                const stnode & item = arrnodes[i];
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


        std::string bintohex( const std::vector<unsigned char> & arr ) {
            std::string r;
            for( const unsigned char s : arr) { 
                r.push_back(szhex()[ (s>>4) & 0xF ]); r.push_back(szhex()[ (s>>0) & 0xF ]); }
            return r; }

        void rotatenodes() {
            for(int i = 0; i <= arrnodes.size(); i++) {
                if(arrnodes[i].tt == 2) {
                    arrnodes[i].tt = 1;
                    int swpidx = arrnodes[i].id;
                    tarch.swaplr(swpidx); } } }

        void dumpSCode(std::vector<stnode> & scde) {
            printf("SCode: "); for( const stnode & n : scde) { printf("[%2d:%d]", n.id, n.tt); } printf("\n"); }

        void dumplr() {
            for( const stnode & n : arrnodes) {
                printf("%2d:%d L=%2d R=%2d\n", n.id, n.tt, getleft(n.id), getrigh(n.id)); } }

        void dumpnodes() {
            int total = cntall();
            for(int i=0; i<=total;i++) {
                printf("#%2d LY=%d\n", i, getlay(i)); } }

        std::vector<int> findbylay(int ll) {
            std::vector<int> r;
            int total = cntall();
            for(int i=0;i<=total;i++) { if( getlay(i) == ll ) r.push_back(i); }
            return r; }

        const std::vector<int> & seqlay(int lay) { return seqlays[lay]; }

        void dump_str_array(std::vector<std::string> & content) {
            for( const std::string & s : content) { printf("%s\n", s.c_str() ); } }

        std::string bitpath(int idx) {
            std::string r;
            int curidx = idx;
            while(curidx != rootidx()) {
                int parent = tarch.getu(curidx);
                r += '0' + ( tarch.getleft(parent) != curidx );
                curidx = parent; }
            std::reverse(r.begin(), r.end());
            return r; }

        char symrate_buffer[64];

        std::string symrate( int idx ) {
            std::string     bpath   = bitpath(idx);
            double          val     = bpath.size() * 1.0f / ( tarch.getv(idx) * 8);
            auto [ptr, ec] = std::to_chars(symrate_buffer, symrate_buffer + 64, val * 100, std::chars_format::fixed, 2);
            std::string s(symrate_buffer, ptr);
            std::string r = "%" + s;
            return r; }

    private:

        TCode                   tcode;              // TCode
        std::vector<stnode>     arrnodes;           // Scode
        std::vector<int>        seqlays[16];        // 2D array layers sequence
        
        // -----------------------------------------------------------------------------
        // Huffman tree architecture
        // -----------------------------------------------------------------------------
        VHTreeArch              tarch;


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
            autopass(rootidx, 0);
            std::vector<stnode> r = _autoscode;
            printf("SCode generated from Huffman : "); dumpSCode(r); return r; }

        // -----------------------------------------------------------------------------
        int find_minnode_id() {
            int minval    = arrnodes[0].id;
            for( int i=1; i < arrnodes.size();i++) {
                if(arrnodes[i].id < minval) minval = arrnodes[i].id; }
            return minval; }

        int _autoenumcnt;

        // -----------------------------------------------------------------------------
        int autoenumerate(int scodeidx, int lay) {

            int tid     = arrnodes[scodeidx].id;
            int tt      = arrnodes[scodeidx].tt;
            int r       = scodeidx;
            int layn    = lay + 1;

            if(!scodeidx) { _autoenumcnt = 0; tarch.setlay( tid, 0 ); }

            printf("AUTOENUM Enter >   #%2d LAY=%2d [%d:%d] )\n", tid, lay, tid, tt);

            switch(tt) {
                case VHTreeArch::eNodeL: {
                    tarch.LinkLeft(arrnodes[r+1].id,  tid, layn);
                    tarch.LinkRigh(_autoenumcnt++, tid, layn);
                    r = autoenumerate(r+1, layn);
                    } break;
                case VHTreeArch::eNodeR: {
                    tarch.LinkLeft(_autoenumcnt++, tid, layn);
                    tarch.LinkRigh(arrnodes[r+1].id,  tid, layn);
                    r = autoenumerate(r+1, layn);
                } break;
                case VHTreeArch::eNodeB: {
                    tarch.LinkLeft(arrnodes[r+1].id, tid, layn);
                    r = autoenumerate(r+1, layn);
                    tarch.LinkRigh(arrnodes[r+1].id, tid, layn);
                    r = autoenumerate(r+1, layn);
                    } break;
                default: {
                    tarch.LinkLeft(_autoenumcnt++, tid, layn);
                    tarch.LinkRigh(_autoenumcnt++, tid, layn);
                    } break; }

            printf("AUTOENUM Exit  <   #%2d LAY=%2d [%d:%d] )\n", tid, lay, tid, tt);
            return r; }

        std::vector<stnode> _autoscode;

        // -----------------------------------------------------------------------------
        int autopass(int idx, int lay) {

            VHTreeArch::enNodeType      tt      = tarch.nodetype(idx);
            stnode                      nn      = { .id = idx, .tt = tt };
            uint8_t                     layn    = lay + 1;

            _autoscode.push_back(nn);
    
            int lidx = getleft(idx);
            int ridx = getrigh(idx);

            tarch.setlay(lidx,layn);
            tarch.setlay(ridx,layn);

            switch(tt) {
                case VHTreeArch::eNodeL: {      autopass(lidx, layn); } break;
                case VHTreeArch::eNodeR: {      autopass(ridx, layn); } break;
                case VHTreeArch::eNodeB: {      autopass(lidx, layn);
                                                autopass(ridx, layn); } break;
                default: { } break; }

        return idx; }
};

// void fromscd     ( std::vector<unsigned char> & arr )    { scode = fromscdi(arr); }
// void fromtcd     ( std::vector<unsigned char> & arr )    { scode = fromtcdi(arr); }
// -----------------------------------------------------------------------------
// std::vector<stnode> fromscdi ( std::vector<unsigned char> & arr ) { std::vector<stnode> r; return r; }
// -----------------------------------------------------------------------------
// std::vector<stnode> fromtcdi ( std::vector<unsigned char> & arr ) { std::vector<stnode> r; return r; }

// int bitpathi(int idx) {
//     int r = 0;
//     int curidx = idx;
//     while(curidx != rootidx()) {
//         int parent = ooo[curidx].u;
//         r += '0' + (ooo[parent].l != curidx);
//         curidx = parent; }
//     return r; }
