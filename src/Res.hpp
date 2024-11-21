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

bool contains(const char* str, const char* subStr) {
    return std::strstr(str, subStr) != nullptr;
}

constexpr bool is_eq_cstr(const char* str1, const char* str2) {
    while (*str1 && (*str1 == *str2)) {
        ++str1;
        ++str2;
    }
    return *str1 == *str2;
}

Texture2D LoadTextureFromMemWithSize(Resource r, int width, int height) {
    Image img = LoadImageFromMemory(".png", r.data, r.size);
    ImageResize(&img, width, height);
    Texture2D texture = LoadTextureFromImage(img);
    UnloadImage(img);
    return texture;
}

std::vector<TexInfo> GetTexturesfromfolder(const char* foldername, int width=-1, int height=-1){
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
size_t GetTextureCountFromFolder(const char* foldername){
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
const Texture2D& GetUiTex(const char* name){
    for (const auto& t : ui)
    {
        if(contains(t.name, name)){
            return t.t;
        }
    }
    
    printf("this is unreachable in GetUiTex");
    exit(1);
}

int GetUiTexWidth(const char* name){
    for (const auto& t : ui)
    {
        if(contains(t.name, name)){
            return t.width;
        }
    }
    
    printf("this is unreachable in GetUiTex");
    exit(1);
}

int GetUiTexHeight(const char* name){
    for (const auto& t : ui)
    {
        if(contains(t.name, name)){
            return t.height;
        }
    }
    
    printf("this is unreachable in GetUiTex");
    exit(1);
}

Image GetImageFromRes(const char* name){
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

Sound GetSound(const char* name){
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