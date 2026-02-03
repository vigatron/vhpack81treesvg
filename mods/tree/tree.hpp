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
		verr buildFromTCode ( const std::string strtcode , bool rotation ) {

			if(!tcode.initfromstr( strtcode )) return verror(1);

			_scode = CreateSCodeFromTCode(tcode);
			tarch.ClearAllNodes();
			tarch.setsymscount( find_minnode_id() );
			autoenumerate(0, 0);
			tarch.setsize(_scode[0].id+1); // Important
			tarch.CalculateNodesDepth();
			tarch.CalculateMaxDepth();
			
			if(rotation) tarch.Rotation();

			dump_scode_lr( scode() );
			dumpnodes();
			return vok; }
 
		// -----------------------------------------------------------------------------
		verr buildFromSpectrum( const std::vector<int> spcints , bool rotation ) {

			if( spcints.size() < 2 )
				return verrmsg(1,"Can't build tree with spectrum less than <2 elms");

			tarch.ClearAllNodes();
			for( int i=0 ; i < spcints.size() ; i++ ) {
				tarch.SetCountVal(i, spcints[i]); }

			tarch.LinkTreeFromSpectrum( spcints.size());
			tarch.setsymscount( spcints.size() );
			_scode = BuildSCodeFromHuffman();
			tarch.CalculateNodesDepth();
			tarch.CalculateMaxDepth();
			if(rotation) tarch.Rotation();

			dump_scode_lr( scode() );
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
		std::vector<stnode> BuildSCodeFromHuffman( ) {

			_autoscode.clear();

			_reidxnode = tarch.size() - 1;
			_reidxnodes.clear();

			_reidxsym = 0;
			_reidxsyms.clear();

			int rootidx = tarch.size() - 1;
			autopass(rootidx, 0);

			std::vector<stnode> r = _autoscode;
			dumpSCode("Direct SCode generated from Huffman : ", r);

			for( int i = 1; i < r.size(); i++ ) { r[i].id = r[0].id - i; }
			dumpSCode("Transf SCode generated from Huffman : ", r);

			return r; }

		// -----------------------------------------------------------------------------

		int                             cntall      ()          { return _scode[0].id;    }
		int                             rootidx     ()          { return cntall();          }

		const VHTreeArch  &             arch        ()          { return tarch; }
		VHTreeArch::stobj *             operator[]  (int idx)   { return tarch[idx];        }

		int     getleft(int i) { return tarch.getleft(i); }
		int     getrigh(int i) { return tarch.getrigh(i); }
		int     getlay (int i) { return tarch.getlay (i); }
		bool    getswap(int i) { return tarch.getswap(i); }

		std::vector<stnode> scode() const { return _scode; }
		std::vector<int> reidxtbl() const { return _reidxsyms; }

        std::string SCodeToText( std::vector<stnode> arr ) {
            std::string r;
            int ss = arr.size();
            r += szhex()[ ((arr.size() - 1) >> 4) & 0xF ];
            r += szhex()[ ((arr.size() - 1) >> 0) & 0xF ];
            for(const stnode & item :arr) { r += '0' + item.tt; }
            return r; }

        std::string SCodeToTCode( std::vector<stnode> arr ) {
            std::vector<unsigned char> r;
            r.push_back( arr.size() - 1 );
            unsigned char v   = 0;
            unsigned char msk = 0x80;

            for(int i=0; i< arr.size(); i++ ) {
                const stnode & item = arr[i];
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

		void dumpSCode( std::string msg, std::vector<stnode> & scde) {
			printf("%s SCode: ", msg.c_str() );
			for( const stnode & n : scde) {
				printf("[%2d:%d]", n.id, n.tt); } printf("\n"); }

        void dump_scode_lr( std::vector<stnode> arr ) {
            for( const stnode & n : arr) {
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
		std::vector<stnode>     _scode;				// Scode
		std::vector<int>        seqlays[16];        // 2D array layers sequence

		// Auto-enumeration related
		int						_autoenumcnt;
		std::vector<stnode>		_autoscode;

		// Re-indexation vars
		int						_reidxnode;
		std::vector<int>		_reidxnodes;

		int						_reidxsym;
		std::vector<int>		_reidxsyms;

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
			dumpSCode( "SCode generated from TCode : " , r);
			return r; }

		// -----------------------------------------------------------------------------
		int find_minnode_id() {
			int minval    = _scode[0].id;
			for( int i=1; i < _scode.size();i++) {
				if(_scode[i].id < minval) minval = _scode[i].id; }
			return minval; }

        // -----------------------------------------------------------------------------
		// Used when building tree from TCode
        // -----------------------------------------------------------------------------
        int autoenumerate(int scodeidx, int lay) {

            int tid     = _scode[scodeidx].id;
            int tt      = _scode[scodeidx].tt;
            int r       = scodeidx;
            int layn    = lay + 1;

            if(!scodeidx) { _autoenumcnt = 0; tarch.setlay( tid, 0 ); }

            printf("AUTOENUM Enter >   #%2d LAY=%2d [%d:%d] )\n", tid, lay, tid, tt);

            switch(tt) {
                case VHTreeArch::eNodeL: {
                    tarch.LinkLeft(_scode[r+1].id,  tid, layn);
                    tarch.LinkRigh(_autoenumcnt++, tid, layn);
                    r = autoenumerate(r+1, layn);
                    } break;
                case VHTreeArch::eNodeR: {
                    tarch.LinkLeft(_autoenumcnt++, tid, layn);
                    tarch.LinkRigh(_scode[r+1].id,  tid, layn);
                    r = autoenumerate(r+1, layn);
                } break;
                case VHTreeArch::eNodeB: {
                    tarch.LinkLeft(_scode[r+1].id, tid, layn);
                    r = autoenumerate(r+1, layn);
                    tarch.LinkRigh(_scode[r+1].id, tid, layn);
                    r = autoenumerate(r+1, layn);
                    } break;
                default: {
                    tarch.LinkLeft(_autoenumcnt++, tid, layn);
                    tarch.LinkRigh(_autoenumcnt++, tid, layn);
                    } break; }

            printf("AUTOENUM Exit  <   #%2d LAY=%2d [%d:%d] )\n", tid, lay, tid, tt);
            return r; }

		// -----------------------------------------------------------------------------
		// Collect and assemble TCode record from Huffman tree
		// -----------------------------------------------------------------------------
		int autopass(int idx, int lay) {

			VHTreeArch::enNodeType      tt      = tarch.nodetype(idx);
			stnode                      nn      = { .id = idx, .tt = tt };
			uint8_t                     layn    = lay + 1;

			_autoscode.push_back(nn);
			_reidxnodes.push_back(_reidxnode--);

			int lidx = getleft(idx);
			int ridx = getrigh(idx);

			// Update layern attribute
			tarch.setlay(lidx,layn);
			tarch.setlay(ridx,layn);

			switch(tt) {
				case VHTreeArch::eNodeL: {
					_reidxsyms.push_back(ridx);
					autopass(lidx, layn);
				} break;
				case VHTreeArch::eNodeR: {
					_reidxsyms.push_back(lidx);
					autopass(ridx, layn);
				} break;

				case VHTreeArch::eNodeB: {
					autopass(lidx, layn);
					autopass(ridx, layn);
				} break;

				default: { 
					_reidxsyms.push_back( lidx );
					_reidxsyms.push_back( ridx );
				} break; }

		return idx; }
};

/*
void rotatenodes() {
	for(int i = 0; i <= arrnodes.size(); i++) {
		if(arrnodes[i].tt == 2) {
			arrnodes[i].tt = 1;
			int swpidx = arrnodes[i].id;
			tarch.swaplr(swpidx); } } }
*/

// void set ( const std::vector<stnode> & vect) { arrnodes = vect; }
// const std::vector<stnode> & nodes ( ) { return arrnodes; }

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
