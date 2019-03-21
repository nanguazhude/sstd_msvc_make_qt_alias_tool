
/* https://en.cppreference.com/w/cpp/filesystem */

#include <fstream>
#include <iostream>
#include <filesystem>

#include <string>
#include <string_view>

#include <exception>

using namespace std::string_literals;
using namespace std::string_view_literals;

#include <regex>

namespace _theMainFile {

    namespace filesystem = std::filesystem;

    inline void makeDirLink(const filesystem::path & target,const filesystem::path & link);
    inline void makeLink(const filesystem::path & target,const filesystem::path & link);

    inline void replaceAll(std::string & arg){
        for( auto & varI:arg ){
            if( varI == '\\' ){
                varI = '/';
            }
        }
    }

}/*namespace _theMainFile*/


int main(int argc,char ** argv) try {

    if( argc!=2 ){
        std::cout << "you should input Qt dir"sv << std::endl;
        return -1;
    }

    namespace the = _theMainFile;
    the::filesystem::path varApplicationFilePath { argv[0] };
    the::filesystem::path varApplicationDirPath = varApplicationFilePath.stem();
    the::filesystem::path varRealQtDir { argv[1] };
    the::filesystem::path varTargetRootDir{ "C:/Qt" };
    the::filesystem::path varAliasPath = varTargetRootDir / "alias"sv;

    if(!the::filesystem::exists( varTargetRootDir )) try{
        the::filesystem::create_directories(varTargetRootDir);
    }catch(...){
        std::cout << "can not create : "sv << varTargetRootDir << std::endl;
        return -1;
    }

    if( the::filesystem::exists( varAliasPath ) ) try {
        the::filesystem::remove_all( varAliasPath );
    }catch(...){
        std::cout << "can not remove : "sv << varAliasPath << std::endl;
        return -1;
    }

    try {
        the::filesystem::create_directories(varAliasPath);
    } catch (...) {
        std::cout << "can not create : "sv <<varAliasPath <<std::endl;
        return -1;
    }

    {
        std::ofstream varLog{ varAliasPath/"warning.txt"sv , std::ios::binary };
        varLog.sync_with_stdio(false);
        if(varLog.is_open()==false){
            std::cout << "can not write in dir : " << varAliasPath << std::endl;
            return -1;
        }
        varLog << u8R"( all changes in this dir will be delete !
                  )"sv;
    }


    {
        the::filesystem::directory_iterator varPos{varRealQtDir};
        the::filesystem::directory_iterator varEnd;
        for(;varPos!=varEnd;++varPos){
            const auto & varPath = varPos->path();
            const auto & varName = varPath.filename();
            if(varPos->is_directory()){
                /*something lick : 5.12.2*/
                const static std::regex varQtRegex{u8R"([0-9]+\.[0-9]+\.[0-9]+)"};
                if(std::regex_match( varName.string() ,varQtRegex )){
                    the::makeDirLink( varPath , varAliasPath/"qtalias"sv );
                }else{
                    the::makeDirLink( varPath , varAliasPath/varName );
                }
            }else{
                the::makeLink( varPath , varAliasPath/varName  );
            }
        }
    }

    std::cout << u8R"ooooooooo(
do not forget to change qt.conf something lick :

[Paths]
Documentation=C:/log/Qt.5.12.2/5.12.2/mingw73_64/../../Docs/Qt-5.12.2
Examples=C:/log/Qt.5.12.2/5.12.2/mingw73_64/../../Examples/Qt-5.12.2
Prefix=C:/log/Qt.5.12.2/5.12.2/mingw73_64/

to change qtenv2.bat  something lick :

@echo off
echo Setting up environment for Qt usage...
set PATH=C:\log\Qt.5.12.2\5.12.2\msvc2017_64\bin;%PATH%
cd /D C:\log\Qt.5.12.2\5.12.2\msvc2017_64
call "C:/Program Files (x86)/Microsoft Visual Studio/2017/Enterprise/VC/Auxiliary/Build/vcvars64.bat"
echo Remember to call vcvarsall.bat to complete environment setup!

)ooooooooo"sv;

}catch( const std::exception & arg ){
    std::cout << arg.what() << std::endl ;
}

namespace _theMainFile {

    inline void makeDirLink(const filesystem::path & target,const filesystem::path & link){
        std::string varCommand = "mklink /J \""s;
        varCommand += link.string();
        varCommand += "\" \""sv;
        varCommand += target.string();
        varCommand += "\"";
        replaceAll(varCommand);
        system( varCommand.c_str() );
    }

    inline void makeLink(const filesystem::path & target,const filesystem::path & link){
        std::string varCommand = "mklink \""s;
        varCommand += link.string();
        varCommand += "\" \""sv;
        varCommand += target.string();
        varCommand += "\"";
        replaceAll(varCommand);
        system( varCommand.c_str() );
    }

}





