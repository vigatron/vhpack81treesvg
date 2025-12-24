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

            for(int i=0; i<255; i++) {
                left [i] = 0xFFFF;
                righ [i] = 0xFFFF;
                up   [i] = 0xFFFF;
                lay  [i] = 0xFFFF;
            }

            recurse(0, 0);
            asm("nop");
        }

        void dump() {
            int i=0;
            for( const snode & n : nds) { printf("#%d  %2d:%d\n", i++, n.id, n.tt); }
            int minid = find_min_id();
            printf("cnt = %d\n", minid); }

        void dumplr() {
            for( const snode & n : nds) { 
                printf("%2d:%d L=%2d R=%2d\n", n.id, n.tt, left[n.id], righ[n.id]);
            }
        }

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
        int lay [256];

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

        int _numi;
        int inum() { return _numi++; }

        int recurse(int i, int layn) {

            if(!i) _numi = 0;

            int tid = nds[i].id;
            int tt  = nds[i].tt;
            int r   = i;

            printf("ENTERING #%2d LAY=%2d ( %d:%d ) \n", i, layn, tid, tt);

            if(tid == 12) {
                asm("nop"); }

            switch(tt) {
                case eNodeL:    {
                    left [tid] = nds[i+1].id;
                    r = recurse(r+1, layn+1);
                    righ [tid] = inum();
                    } break;
                case eNodeR:    {
                    righ [tid] = nds[i+1].id;
                    r = recurse(i+1, layn+1);
                    left [tid] = inum();
                } break;
                case eNodeB:    {
                    left [tid] = nds[i+1].id;
                    r = recurse(i+1, layn+1);
                    righ [tid] = nds[r+1].id;
                    r = recurse(r+1, layn+1);
                    } break;
                default:        { 
                    left [tid] = inum();
                    righ [tid] = inum(); 
                    } break;
            }

            printf("EXITING  #%2d LAY=%2d ( %d:%d ) \n", i, layn, tid, tt);
            return r;
        }

};
