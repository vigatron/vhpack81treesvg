#pragma once

#include "vhplatform.hpp"

class VHTreeArch {

    public:

        VHTreeArch() { }

        struct stobj {

            uint16_t l;     // left
            uint16_t r;     // right
            uint16_t u;     // up
            uint16_t v;     // count
            
            uint8_t  y;     // layer
            uint8_t  m;     // Mark flag, need for build by spectrum
            uint8_t  w;     // Rotation flag, LR was swapped
            uint8_t  d;     // Depth of the node
        };

        enum enNodeType {
            eNodeF = 0,     // F Fin
            eNodeL = 1,     // L Left
            eNodeR = 2,     // R Right
            eNodeB = 3      // B Both
        };

        enNodeType nodetype(int idx) {
            bool lsym = ooo[idx].l < _cntlow;
            bool rsym = ooo[idx].r < _cntlow;
            if( lsym && rsym )  return eNodeF;
            if( !lsym && !rsym) return eNodeB;
            return !lsym ? eNodeL : eNodeR; }

        stobj *         operator[]  (int idx)   { return &  ooo[idx]; }

        uint16_t        size        () const    { return    huffcnt;    }
        uint16_t        cntsyms     () const    { return    _cntlow;    }
        uint16_t        depthmax    () const    { return    _depthmax;  }

        uint16_t        rootidx     () const    { return    huffcnt-1;}

        uint16_t        getv        (int i)     { return    ooo[i].v; }
        uint16_t        getu        (int i)     { return    ooo[i].u; }
        uint16_t        getleft     (int i)     { return    ooo[i].l; }
        uint16_t        getrigh     (int i)     { return    ooo[i].r; }
        int             getlay      (int i)     { return    ooo[i].y; }
        bool            getswap     (int i)     { return    ooo[i].w; }

        bool    issym       (int idx) const  { return idx < _cntlow;     }
        bool    isnode      (int idx) const  { return idx >= _cntlow;    }
        bool    isroot      (int idx) const  { return idx == rootidx();  }

        void    setlay          (int i, uint8_t y)      { ooo[i].y = y; }
        void    setsymscount    ( uint8_t s )           { _cntlow = s;  }

        static const u16        INV = 0xFFFF;

        void ClearAllNodes() { for(int i=0; i<512; i++) { clrnode(i); } }

        void SetSwapFlag( int idx, uint8_t  f) { ooo[idx].w = 1; }
        void SetCountVal( int idx, uint16_t v) { ooo[idx].v = v; }

        void swaplr(int idx) { int tmp = ooo[idx].l; ooo[idx].l = ooo[idx].r; ooo[idx].r = tmp; }

        verr LinkTreeFromSpectrum(int symscnt) {
            huffcnt = symscnt; while(symscnt-->=2) NLnk(); return vok; }

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

        // -----------------------------------------------------------------------------
        void CalculateMaxDepth() {
            uint8_t r = 0;
            for(int i=0;i < size();i++) { uint8_t lay = getlay(i); if( lay > r) r = lay; }
            _depthmax = r; }

    private:

        stobj                   ooo[512];
        uint16_t                huffcnt;
        uint16_t                _cntlow;
        uint8_t                 _depthmax;

        void    clrnode(int i)   {
            ooo[i].l = INV; ooo[i].r = INV; ooo[i].u = INV; ooo[i].v = 0;
            ooo[i].y = 0; ooo[i].m = 0; ooo[i].w = 0; ooo[i].d = 0; }

        // Link L and R to U
        void NSLRU(u16 i, u16 l, u16 r) { ooo[i].l = l; ooo[i].r=r; ooo[l].u=i; ooo[r].u=i; }

        void FindFirst(u16 *pidx) { for(;*pidx<huffcnt;(*pidx)++) if(!ooo[*pidx].m) { return; } }

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
            printf("TArchLink #%2d & #%2d ", il, ir); printf(" vals[%3d]=%3d\n", cnt, v); }

};
