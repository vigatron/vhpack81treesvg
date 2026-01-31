#include "args.hpp"

std::vector<int> ParseSpectrum  ( std::string strspc    );
std::vector<int> ParseRotation  ( std::string rotation  );
std::vector<int> ParseSvyazki   ( std::string strsvzk   );
std::vector<int> ParseInjected	( std::string txt );

verr ParseIParams( int argc, char * argv[], VHArgsParser & argsparser );
