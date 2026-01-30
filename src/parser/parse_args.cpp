#include "global.hpp"
#include "args.hpp"
#include "parse_args.hpp"

using namespace std;

// -----------------------------------------------------------------------------
std::vector<int> ParseSpectrum( std::string strspc ) {
    std::vector<std::string>    spcvals = split(strspc, '.');
    std::vector<int> r;
    for( std::string s : spcvals) {
        if(!check_str_digit(s)) {
            verrmsg(1, "Invalid spectrum values");
            exit(1); }
        r.push_back( std::stoi(s) ); }
    return r; }

// -----------------------------------------------------------------------------
std::vector<int> ParseRotation( std::string rotation ) {
    std::vector<int> r;
    std::vector<std::string> rotvals = split(rotation, '^');
    for( std::string s : rotvals) {
        if(!check_str_digit(s)) {
            verrmsg(2, "Invalid rotation values");
            exit(1); }
        r.push_back( std::stoi(s) ); }
    return r; }

// -----------------------------------------------------------------------------
std::vector<int> ParseSvyazki( std::string strsvzk ) {
    std::vector<int> r;
    std::vector<std::string> svzpairs = split(strsvzk, ',');
    for( std::string s : svzpairs ) {
        std::vector<std::string> spl = split(s, '.');
        if(spl.size() == 2) {
            if(!check_str_digit(spl[0]) || !check_str_digit(spl[1]) ) {
                verrmsg(2, "Invalid bind values");
                exit(1); }
            int v1 = std::stoi( spl[0] );
            int v2 = std::stoi( spl[1] );
            r.push_back( v1 > v2 ? v2 : v1 );
            r.push_back( v1 > v2 ? v1 : v2 ); } }
    return r; }

// -----------------------------------------------------------------------------
std::vector<int> ParseInjected( std::string txt ) {
	std::vector<int> r;
	std::vector<std::string> spl = split(txt, '.');
	for( std::string s : spl) {
		if(!check_str_digit(s)) {
			verrmsg(2, "Invalid injection values");
			exit(1); }
			r.push_back( std::stoi( s )); }
	return r; }
