#include "global.hpp"
#include "args.hpp"
#include "parse_args.hpp"
#include "calc.hpp"

#include "app_version.hpp"

#include <filesystem>

using namespace std;

extern TreeGenParamsIn		iparams;


verr GenerateMixMode();
verr GenerateSpcMode();
verr GenerateTcdMode();

// -----------------------------------------------------------------------------
static verr check_t( VHArgsParser & argsparser ) {
	if( !argsparser.checkopt("t") ) {
		std::cout << "No valid tcode data : "
			<< argsparser.listparams() << std::endl;
		argsparser.Usage(); }
	return verror(1); }

// -----------------------------------------------------------------------------
static verr check_s( VHArgsParser & argsparser ) {
	if( !argsparser.checkopt("s") ) {
		std::cout << "No valid spectrum data : "
			<< argsparser.listparams() << std::endl;
		argsparser.Usage(); }
	return verror(1); }

// -----------------------------------------------------------------------------
static verr SetRunMode() {

	int sztcode = iparams.param_tcode.size();
	int szspc	= iparams.param_spcints.size();

	if( !sztcode && !szspc )
			return verrmsg(100, "Need TCode or SPC array to process");

	// Default mode : tcode or spcmode ?
	if(iparams.param_mix == -1) {
		if( sztcode )			{ iparams.genmode = 1; }
		else if( szspc )		{ iparams.genmode = 2; }
	} else { // Mix mode
		iparams.genmode = 3;
		if(!szspc)
			return verrmsg(100, "Need SPC array to process"); }

	return vok; }

// -----------------------------------------------------------------------------
static std::string CheckOutFolder(const std::string & folder)
{
    std::string r;

   // Пустая строка - сразу ошибка
    if (folder.empty())
        return std::string();

    std::error_code ec;
    std::filesystem::path p(folder);

    // 1. Если папка уже существует - возвращаем путь
    if (std::filesystem::is_directory(p, ec))
    {
        if (ec)
            return std::string();
        return folder;
    }

    // 2. Если путь существует, но это не папка (файл/симлинк на файл и т.п.) - ошибка
    if (std::filesystem::exists(p, ec))
        return std::string();
    if (ec)
        return std::string();

    // 3. Папки нет - пробуем создать её вместе со всеми родительскими
    ec.clear();
    std::filesystem::create_directories(p, ec);
    if (ec)
        return std::string();

    // Дополнительная проверка, что после создания это действительно директория
    if (!std::filesystem::is_directory(p, ec) || ec)
        return std::string();

    // 4. Успех
    return folder;

}

// -----------------------------------------------------------------------------
int main( int argc, char * argv[] ) {

    if(argc == 2)
        if( std::string(argv[1]) == "--version") {
            std::cout << APP_VERSION << std::endl;
            std::exit(0);
        }

	VHArgsParser argsparser;

	// Parse command line params
	VHParseArgs pargs;
	verr rparse = pargs.ParseIParams( argc, argv, argsparser );
	if( vok != rparse ) return 1;

    //
    iparams.callparams = argsparser.listparams();

    iparams.param_tcode = argsparser.getopt("t");
    iparams.param_partn = argsparser.getopt("prtn");
    iparams.param_blockn = argsparser.getopt("blkn");

    iparams.param_spcints = pargs.ParseSpectrum(argsparser.getopt("v"));
    iparams.param_rotints = pargs.ParseRotation(argsparser.getopt("w"));
    iparams.param_svzints = pargs.ParseSvyazki(argsparser.getopt("z"));
    iparams.param_injected = pargs.ParseInjected(argsparser.getopt("injc"));
    iparams.param_cntrint = pargs.ParseInt(argsparser.getopt("cntr"));
    iparams.param_mix = pargs.ParseInt(argsparser.getopt("mix"));

    //
    {
        iparams.param_outfolder = argsparser.getopt("outfolder");
        if(iparams.param_outfolder.size()) {
            std::string strCheck = CheckOutFolder(iparams.param_outfolder);
            if(!strCheck.size())
                return 100;
        }
    }

	// Recognize mode and check params
	verr rmode = SetRunMode();
	if( vok != rmode )
		return verrmsg(80, "RunMode issue, input parameters maybe");

	verr ret = 1;
	switch( iparams.genmode ) {
		case 1 : { ret = GenerateTcdMode(); } break;
		case 2 : { ret = GenerateSpcMode(); } break;
		case 3 : { ret = GenerateMixMode(); } break;
		default: { } }

	// Save results
	if(vok == ret) {
		svg.savetosvg( oparams.outfname ); }
	else {
		std::cout <<
		"Final result: generation process failed !"
		<< std::endl; }

	return (vok == ret) ? 0 : 1; }


// >>> Done >>>
// DATE : TIME
// Call Params
// Ramka up
// BPath rates %
