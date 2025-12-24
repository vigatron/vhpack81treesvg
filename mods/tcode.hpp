#pragma once

#include <string>
#include <vector>

struct snode { int id; int tt; };


class VHTree {

    public:

        VHTree() { }
        VHTree                              ( std::string str)                      { fromstr(str); }
        void                    set         ( const std::vector<snode> & vect)      { nds = vect; }
        void                    fromstr     (std::string scode)                     { nds = fromstri(scode); }

        const std::vector<snode> & nodes() { return nds; }

        void build() {
            for(int i=0; i<255; i++) { left [i] = 0xFFFF; righ [i] = 0xFFFF; up [i] = 0xFFFF; }
            recurse(0); }

        void dump() {
            int i=0;
            for( const snode & n : nds) { printf("#%d  %d:%d\n", i++, n.id, n.tt); }
            int minid = find_min_id();
            printf("cnt = %d\n", minid); }

        enum enNodeType {
            eNodeF = 0,     // F Fin
            eNodeL = 1,     // L Left
            eNodeR = 2,     // R Right
            eNodeB = 3      // B Both
        };


    private:

        std::vector<snode> nds;

        int left[256];
        int righ[256];
        int up  [256];

        std::vector<snode> fromstri(std::string scode) {

            std::vector<int>     lbo; // [
            std::vector<int>     lsp; // :
            std::vector<int>     lbc; // ]
            std::vector<snode>   r;

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
                snode nn = { .id = std::stoi(sid), .tt = std::stoi(sit) };
                r.push_back(nn); }

            return r; }

        int find_min_id() {
            int minval    = nds[0].id;
            for( int i=1; i < nds.size();i++) {
                if(nds[i].id < minval) minval = nds[i].id; }
            return minval; }

        int recurse(int i) {

            int tid = nds[i].id;
            int tt  = nds[i].tt;
            int r   = i;

            printf("ENTERING #%2d ( %d:%d ) \n", i, tid, tt);

            if(tid == 12) {
                asm("nop"); }

            switch(tt) {
                case eNodeL:    {
                    int jinl = nds[i+1].id;
                    left [tid] = jinl;
                    r = recurse(r+1);
                    righ [tid] = 0xAAAA;
                    } break;
                case eNodeR:    {
                    left [tid] = 0xAAAA;
                    int jinr = nds[i+1].id;
                    righ [tid] = jinr;
                    r = recurse(i+1);
                } break;
                case eNodeB:    {
                    int jinl = nds[i+1].id;
                    left [tid] = jinl;
                    r = recurse(i+1);
                    int jinr = nds[r+1].id;
                    righ [tid] = jinr;
                    r = recurse(r+1);
                    } break;
                default:        { 
                    left [tid] = 0xAAAA;
                    righ [tid] = 0xAAAA; 
                    } break;
            }

            printf("EXITING  #%2d ( %d:%d ) \n", i, tid, tt);
            return r;
        }

};
