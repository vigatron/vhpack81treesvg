#pragma once

#include "vhplatform.hpp"

class TCode {

    public:

        TCode() { }
        TCode(std::string str) { initfromstr(str); }

        bool initfromstr(std::string str) { 
            std::vector<uint8_t>  src = strtobin(str);
            return initfrombin(src); }

        bool initfrombin(const std::vector<uint8_t> & src) {
            packedcode = src;
            unpacknodes();
            calcsymscount();
            dmpinfo();
            return true; }

        int operator[]  (int idx)   const { return nodes[idx]; }

        int sizenodes()             const { return _sizenodes; }
        int sizesyms()              const { return _sizesyms;  }
        int sizeall()               const { return _sizenodes + _sizesyms; }
        int getrootidx  ()          const { return sizeall() - 1; }

        void dmpinfo() { printf("cntsyms = %d cntnodes = %d total = %d\n", _sizesyms, _sizenodes, sizeall() ); }

    private:

        const uint8_t           nodeweight[4] = { 2, 1, 1, 0 };

        std::vector<uint8_t>    packedcode; 
        std::vector<uint8_t>    nodes;
        int                     _sizenodes;
        int                     _sizesyms;

        std::vector<uint8_t> strtobin(std::string str) {
            std::vector<uint8_t> tmp;
            for( const uint8_t ss : str ) { tmp.push_back(hextoi(ss)); }
            std::vector<uint8_t> r;
            for( int i=0; i < tmp.size()/2;i++) {
                uint8_t v = (tmp[i*2+0] << 4) + tmp[i*2+1];
                r.push_back(v); }
            return r; }

        uint8_t hextoi(uint8_t v) {
            uint8_t s = toupper(v);
            if((s>='0')&&(s<='9')) return(s-'0');
            else if((s>='A')&&(s<='F')) return(10 + (s - 'A'));
            return 0; }

        void addnode( uint8_t val ) { nodes.push_back(val & 3); }

        void unpacknodes() {
            int pos = 0;
            nodes.clear();
            _sizenodes = packedcode.size() ? (packedcode[0] + 1) : 0;
            for(int i=1; i < packedcode.size(); i++) {
                uint8_t bv = packedcode[i];
                if(pos < _sizenodes) { addnode(bv>>6); pos++; }
                if(pos < _sizenodes) { addnode(bv>>4); pos++; }
                if(pos < _sizenodes) { addnode(bv>>2); pos++; }
                if(pos < _sizenodes) { addnode(bv>>0); pos++; } } }

        void calcsymscount() {
			_sizesyms = 0;
			for(int i=0;i<nodes.size();i++) {
				_sizesyms += nodeweight[ nodes[i] & 3 ]; } }
};

/* int minid = find_minnode_id(); printf("cnt = %d\n", minid); */
