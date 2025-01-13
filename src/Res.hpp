#pragma once
#include <raylib.h>
#include <array>
#include <cstring>

#include <all_res.hpp>

struct TexInfo {
    Texture2D t;
    const char* name;
    int width, height;
};

inline static std::vector<TexInfo> ui;

auto contains(const char* str, const char* subStr) -> bool {
    return std::strstr(str, subStr) != nullptr;
}

constexpr auto is_eq_cstr(const char* str1, const char* str2) -> bool {
    while (*str1 && (*str1 == *str2)) {
        ++str1;
        ++str2;
    }
    return *str1 == *str2;
}

auto LoadTextureFromMemWithSize(Resource r, int width, int height) -> Texture2D {
    Image img = LoadImageFromMemory(".png", r.data, r.size);
    ImageResize(&img, width, height);
    Texture2D texture = LoadTextureFromImage(img);
    UnloadImage(img);
    return texture;
}

auto GetTexturesfromfolder(const char* foldername, int width=-1, int height=-1) -> std::vector<TexInfo> {
    std::vector<TexInfo> result;

    std::string path_postfix(foldername);
    path_postfix += "/";
    for (unsigned int i = 0; i < resources_count; i++)
    {
        auto r = resources[i];
        if( r.type == IMG){
            if(contains(r.path, path_postfix.c_str())){
                if(width != -1 && height != -1){
                    result.push_back({LoadTextureFromMemWithSize(r, width, height), strdup(r.path), width, height});
                }else{
                    Image img = LoadImageFromMemory(".png", r.data, r.size);
                    result.push_back({LoadTextureFromImage(img), strdup(r.path), img.width, img.height});
                    UnloadImage(img);
                }
            }
        }
    }

    if(result.empty()) {
        printf("%s is  empty or not exist", foldername);
        exit(1);
    }
    return result;
}
auto GetTextureCountFromFolder(const char* foldername) -> std::size_t {
    std::string path_postfix(foldername);
    path_postfix += "/";
    size_t count = 0;
    for (unsigned int i = 0; i < resources_count; i++)
    {
        auto r = resources[i];
        if( r.type == IMG){
            if(contains(r.path, path_postfix.c_str())){
                count++;
            }
        }
    }
    return count;
}

void Init_Resources(){
    ui = GetTexturesfromfolder("ui");
}
auto GetUiTex(const char* name) -> const Texture2D& {
    for (const auto& t : ui)
    {
        if(contains(t.name, name)){
            return t.t;
        }
    }
    
    printf("this is unreachable in GetUiTex");
    exit(1);
}

auto GetUiTexWidth(const char* name) -> int {
    for (const auto& t : ui)
    {
        if(contains(t.name, name)){
            return t.width;
        }
    }
    
    printf("this is unreachable in GetUiTex");
    exit(1);
}

auto GetUiTexHeight(const char* name) -> int {
    for (const auto& t : ui)
    {
        if(contains(t.name, name)){
            return t.height;
        }
    }
    
    printf("this is unreachable in GetUiTex");
    exit(1);
}

auto GetImageFromRes(const char* name) -> Image {
    for (unsigned int i = 0; i < resources_count; i++)
    {
        auto r = resources[i];
        if( r.type == IMG){
            if(is_eq_cstr(r.path, name)){
                return LoadImageFromMemory(".png", r.data, r.size);
            }
        }
    }
    
    printf("No image with name : %s", name);
    exit(1);
}

auto GetSound(const char* name) -> Sound {
    for (unsigned int i = 0; i < resources_count; i++)
    {
        auto r = resources[i];
        if( r.type == AUDIO){
            if(is_eq_cstr(r.path, name)){
                return LoadSoundFromWave(LoadWaveFromMemory(".wav", r.data, r.size));
            }
        }
    }

    printf("No Sound with name : %s", name);
    exit(1);
}