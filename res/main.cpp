#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <system_error>

namespace fs = std::filesystem;

#define INFO(...) std::cout << "[!] INFO : " << __VA_ARGS__ << std::endl
#define WHY std::cout << "[!] ERROR CODE (" << ER_code.value() << ") : "<< ER_code.message() << std::endl
#define ERROR(...) std::cerr << "[x] ERROR : " << __VA_ARGS__ << std::endl; exit(1)

struct ResInfo
{
    std::string name;
    std::string path;
    std::string type;
};

bool between(std::time_t number, std::time_t bound1, std::time_t bound2);
std::string replaceSymb(const fs::path& path);
std::string File2Ccode(const fs::path& in, std::ofstream& outFile);
bool needUpdate(const fs::path& include, const fs::path& rootc);
bool deleteFolder(const fs::path& foldername);

std::error_code  ER_code{};

std::vector<ResInfo> ress;
unsigned int Uknown_count = 0;
unsigned int Img_count = 0;
unsigned int Sound_count = 0;

constexpr const char* FinalIncludeFile = "all_res.hpp";

int main(int argc, char** argv){
    if(argc < 2){
        ERROR("Usage: " << argv[0] << " <res folser>");
    }
    const char* resFolder = argv[1];

    auto root = fs::path(resFolder);
    auto root_c = root / fs::path("c");
    auto pathInclude = root / fs::path(FinalIncludeFile);
    

    if( not needUpdate(pathInclude, root_c)){
        INFO("Every Thing Up To Date :)");
        return 0;
    }

    if(fs::remove(pathInclude, ER_code) == false){
        INFO("Can't remove : " << pathInclude.lexically_normal().generic_string());
    }
    if(ER_code) WHY;


    if ( not deleteFolder(root_c))
    {
        INFO("Could'nt remove " << root_c);
    }

    std::fstream include_file(pathInclude, std::ios::trunc | std::ios::in | std::ios::out);
    if(include_file.fail()){
        ERROR("Could'nt Open : " << FinalIncludeFile);
    }

    include_file <<
        "#pragma once\n"
        "\n"
        "enum R_type {\n"
        "    IMG,\n"
        "    AUDIO,\n"
        "};\n"
        "\n"
        "struct Resource {\n"
        "    int id;\n"
        "    R_type type;\n"
        "    const char* path;\n"
        "    const unsigned char* data;\n"
        "    const unsigned int size;\n"
        "};\n\n";


    if ( not fs::exists(root_c, ER_code))
    {
        fs::create_directory(root_c, ER_code);
        if(ER_code) WHY;
        INFO("Folder Created Sucssucfuly : " << root_c);
    }
    if(ER_code) WHY;

    auto it1 = fs::directory_iterator(root, fs::directory_options::skip_permission_denied, ER_code);
    if(ER_code) WHY;

    for (auto &&e : it1) {
        auto isNotC = e.is_directory(ER_code) && e.path() != root_c;
        if(ER_code) WHY;

        if (isNotC) {
            auto stemDir = e.path();

            std::filesystem::path relativePath = fs::relative(stemDir, root,ER_code);
            if(ER_code) WHY;

            if (relativePath == stemDir) {
                relativePath = fs::path(e.path().filename().string());
            }

            std::filesystem::path newDirPath = "c" / relativePath;

            try {
                fs::create_directories(newDirPath, ER_code);
                if(ER_code) WHY;

            } catch (const std::exception& ex) {
                ERROR(ex.what());
                continue;
            }

            auto nestedIt = fs::recursive_directory_iterator(stemDir, fs::directory_options::skip_permission_denied,ER_code);
            if(ER_code) WHY;
            
            for (auto &&ee : nestedIt ) {

                auto isdir = ee.is_directory(ER_code);
                if(ER_code) WHY;

                if (isdir) {
                    std::filesystem::path newSubDirPath = "c" / fs::relative(ee.path(), root);
                    try {
                        fs::create_directories(newSubDirPath);
                        if(ER_code) WHY;
                    } catch (const std::exception& ex) {
                        ERROR(ex.what());
                        continue;
                    }
                }
            }
        }
    }

    auto it = fs::recursive_directory_iterator(root, fs::directory_options::skip_permission_denied, ER_code);

    for (auto &&e : it)
    {

        auto p = e.path();

        if(p == root_c ){
            it.disable_recursion_pending();
            continue;
        }
        auto isNormalFile = e.is_regular_file(ER_code);
        if(ER_code) WHY;

        if(isNormalFile){
            if(p.parent_path() == root){
                continue;
            }
            auto all_no_ext = p.parent_path() / p.stem();

            auto dest_path = fs::path(root_c.lexically_normal() / all_no_ext.lexically_normal()).replace_extension(".h");
            auto p_lexnormal = p.lexically_normal();
            auto dest_path_lexnormal = dest_path.lexically_normal();

            std::string rtype;

            if(p.extension() == ".png"){
                rtype = "IMG";
                Img_count++;
            }else if(p.extension() == ".wav"){
                rtype = "AUDIO";
                Sound_count++;
            }else{
                rtype = "None";
                Uknown_count++;
            }

            if(rtype == "None"){
                INFO("Unkown Type : " << p_lexnormal.generic_string());
                continue;
            }

            std::ofstream outFile(dest_path);
            auto arr_name = File2Ccode(p, outFile);

            auto include = std::string("#include \"");

            include += dest_path_lexnormal.generic_string();
            include += "\"";

            include_file << include << "\n";

            ress.emplace_back(fs::relative(p_lexnormal, root).generic_string(), arr_name, rtype);
            INFO("Packing " << p_lexnormal.generic_string() << " --> " << dest_path_lexnormal.generic_string());

        }

    }
    //NOTe  : img & sound for now add  more
    if(Img_count == 0 && Sound_count == 0 ) {
        include_file
            << "\ninline static Resource resources[0];\n"
            << "\nconstexpr unsigned int resources_count = 0;\n";

        deleteFolder(root_c);
        return 0;
    }

    int id = 0;
    
    include_file << "\ninline static Resource resources[] {\n";
    for (auto&& e: ress)
    {
        auto [path, name, type] = e;
        include_file << "\t{"<< id++ << ", " << type << ", \"" << path << "\", " << name << ", " << name << "_len },\n";
    }
    include_file 
        << "};\n"
        << "\nconstexpr unsigned int resources_count = sizeof(resources)/ sizeof(resources[0]);\n";

}

bool between(std::time_t number, std::time_t bound1, std::time_t bound2) {
    return number >= std::min(bound1, bound2) && number <= std::max(bound1, bound2);
}

std::string replaceSymb(const fs::path& path){
    constexpr char UnwantedSym[] = {
        '\\', '/', '.' ,' ', '!', '!', '"', '@', '#', '$', '%', '&', '*', '(',')', '-', '=', '+', '`', ';', '~', '[', ']', '{', '}'
    };
    auto name = path.string();

    for (size_t i = 0; i < sizeof(UnwantedSym); i++)
    {
        std::replace(name.begin(), name.end(), UnwantedSym[i], '_');
    }

    return name;
}

std::string File2Ccode(const fs::path& in, std::ofstream& outFile){

    auto all_no_ext = in.parent_path() / in.stem();
    auto arr_name = replaceSymb(all_no_ext);

    std::ifstream readFile(in, std::ios::binary);
    uintmax_t sizeF = fs::file_size(in);
    std::string name = in.stem().string();

    outFile << "constexpr unsigned char " << arr_name << "[] = { ";
    char buff[5] = {'\0'};
    for (uintmax_t i = 0; i < sizeF; i++)
    {
        char d;
        readFile.get(d);
        sprintf(buff, "0x%02X", (unsigned char)d);

        if(i % 20 == 0){
            outFile << "\n\t";
        }

        outFile << buff;
        if( i != sizeF - 1){
            outFile << ", ";
        }
    }
    outFile << "};\n";
    outFile << "\nconstexpr unsigned int " << arr_name << "_len = " << sizeF << ";\n";

    return arr_name;
}

bool needUpdate(const fs::path& include, const fs::path& rootc) {
    auto fileAndCexist = fs::exists(include) && fs::exists(rootc);
    if(ER_code) WHY;
    if(fileAndCexist){

        auto latwrite_c = fs::last_write_time(rootc);
        auto latwrite_inc = fs::last_write_time(include);

        auto sctplatwrite_c = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            latwrite_c - std::filesystem::file_time_type::clock::now()
            + std::chrono::system_clock::now()
        );

        auto sctplatwrite_inc = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            latwrite_inc - std::filesystem::file_time_type::clock::now()
            + std::chrono::system_clock::now()
        );

        std::time_t latwrite_c_cftime = std::chrono::system_clock::to_time_t(sctplatwrite_c);
        std::time_t latwrite_inc_cftime = std::chrono::system_clock::to_time_t(sctplatwrite_inc);

        std::time_t eps = 10;
        if( between(latwrite_c_cftime, latwrite_inc_cftime + eps, latwrite_inc_cftime - eps) 
            || between(latwrite_inc_cftime, latwrite_c_cftime + eps, latwrite_c_cftime - eps)){
            return false;
        }else {
            return true;
        }

    }else {
        return true;
    }
}

bool deleteFolder(const fs::path& foldername){
    auto folderExist = fs::exists(foldername, ER_code);
    if(ER_code) WHY;
    
    if (folderExist)
    {
        auto Remov = fs::remove_all(foldername);
        if(Remov == 0 || Remov == static_cast<std::uintmax_t>(-1)){
            if(ER_code) WHY;
            return false;
        }else{
            return true;
        }
    }else{
        return false;
    }
}