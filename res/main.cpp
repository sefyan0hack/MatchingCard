#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>

namespace fs = std::filesystem;

#define INFO(...) std::cout << "[!] INFO : " << __VA_ARGS__ << std::endl
#define ERROR(...) std::cerr << "[x] ERROR : " << __VA_ARGS__ << std::endl; exit(1)

struct ResInfo
{
    std::string name;
    std::string path;
    std::string type;
};

std::vector<ResInfo> ress;

constexpr const char* FinalIncludeFile = "all_res.hpp";

constexpr char UnwantedSym[] = {
    '\\', '/', '.' ,' ', '!', '!', '"', '@', '#', '$', '%', '&', '*', '(',')', '-', '=', '+', '`', ';', '~', '[', ']', '{', '}'
};

int main(int argc, char** argv){
    if(argc < 2){
        ERROR("Usage: " << argv[0] << " <res folser>");
    }
    const char* resFolder = argv[1];

    auto root = fs::path(resFolder);
    auto root_c = root / fs::path("c");
    if (fs::exists(root_c))
    {
        INFO("Folder Exist : " << root_c);

        if(fs::remove_all(root_c) == 0){
            ERROR("Could'nt remove " << root_c);
        }else{
            INFO("Folder Deleted : " << root_c);
        }
    }
    //
    std::fstream include_file(root / fs::path(FinalIncludeFile), std::ios::trunc | std::ios::in | std::ios::out);
    if(include_file.fail()){
        ERROR("Could'nt Open : " << FinalIncludeFile);
    }
    //
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

    //
    if ( not fs::exists(root_c))
    {
        fs::create_directory(root_c);
        INFO("Folder Created Sucssucfuly : " << root_c);
    }
    //
    for (auto &&e : fs::directory_iterator(root, fs::directory_options::skip_permission_denied)) {
        if (e.is_directory() && e.path() != root_c) {
            auto stemDir = e.path();

            std::filesystem::path relativePath = fs::relative(stemDir, root);

            if (relativePath == stemDir) {
                relativePath = fs::path(e.path().filename().string());
            }

            std::filesystem::path newDirPath = "c" / relativePath;

            try {
                if (fs::create_directories(newDirPath)) {
                    // INFO("Created dir " << newDirPath.string());
                }
            } catch (const std::exception& ex) {
                ERROR(ex.what());
                continue;
            }

            for (auto &&ee : fs::recursive_directory_iterator(stemDir, fs::directory_options::skip_permission_denied)) {
                if (ee.is_directory()) {
                    std::filesystem::path newSubDirPath = "c" / fs::relative(ee.path(), root);

                    try {
                        if (fs::create_directories(newSubDirPath)) {
                            // INFO("Created dir " << newSubDirPath.string());
                        }
                    } catch (const std::exception& ex) {
                        ERROR(ex.what());
                        continue;
                    }
                }
            }
        }
    }

    //
    auto it = fs::recursive_directory_iterator(root, fs::directory_options::skip_permission_denied);

    for (auto &&e : it)
    {

        if(e.path() == root_c ){
            it.disable_recursion_pending();
            continue;
        }
        if(e.is_regular_file()){
            if(e.path().parent_path() == root){
                continue;
            }
            auto all_no_ext = e.path().parent_path() / e.path().stem();

            auto dest_path = fs::path(root_c.lexically_normal() / all_no_ext.lexically_normal()).replace_extension(".h");

            auto arr_name = all_no_ext.string();

            // remove symbols
            for (size_t i = 0; i < sizeof(UnwantedSym); i++)
            {
                std::replace(arr_name.begin(), arr_name.end(), UnwantedSym[i], '_');
            }


            auto p = e.path();
            std::ifstream readFile(p, std::ios::binary);
            
            std::ofstream outFile(dest_path);

            uintmax_t sizeF = fs::file_size(p);
            std::string name = p.stem().string();

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

            outFile << "constexpr unsigned int " << arr_name << "_len = " << sizeF << ";\n";
            auto include = std::string("#include \"");

            include += dest_path.lexically_normal().generic_string();
            include += "\"";

            include_file << include << "\n";

            std::string rtype;

            if(e.path().extension() == ".png"){
                rtype = "IMG";
            }else if(e.path().extension() == ".wav"){
                rtype = "AUDIO";
            }else{
                rtype = "None";
            }

            if(rtype == "None"){
                INFO("Unkown Type : " << p.lexically_normal().generic_string());
                continue;
            }

            ress.emplace_back(fs::relative(p.lexically_normal(), root).generic_string().c_str(), arr_name.c_str(), rtype);
            INFO("Packing " << p.lexically_normal().generic_string() << " --> " << dest_path.lexically_normal().generic_string());

            
        }

    }

    int id = 0;
    
    include_file << "\ninline static Resource resources[] {\n";
    for (auto&& e: ress)
    {
        auto [path, name, type] = e;
        include_file << "\t{"<< id++ << ", " << type << ", \"" << path << "\", " << name << ", " << name << "_len },\n";
    }
    include_file << "};\n";
    include_file << "\nconstexpr unsigned int resources_count = sizeof(resources)/ sizeof(resources[0]);\n";
    

}