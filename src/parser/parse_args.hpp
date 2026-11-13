#include "args.hpp"


class VHParseArgs
{
public:

    std::vector<int> ParseSpectrum(std::string strspc);
    std::vector<int> ParseRotation(std::string rotation);
    std::vector<int> ParseSvyazki(std::string strsvzk);
    std::vector<int> ParseInjected(std::string txt);

    verr ParseIParams(int argc, char *argv[], VHArgsParser &argsparser);

    /** @brief Безопасное преобразование строки в int (-1 для пустой). */
    int ParseInt(std::string param);

private:


};
