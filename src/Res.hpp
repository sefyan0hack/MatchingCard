#pragma once
#include <raylib.h>
#include <array>
#include <cstring>

#include <all_res.hpp>

bool contains(const char* str, const char* subStr) {
    return std::strstr(str, subStr) != nullptr;
}

struct AssInfo
{
    const char* path;
    int w, h;
};


Texture2D LoadTextureFromMemWithSize(Resource r, int width, int height) {
    Image img = LoadImageFromMemory(".png", r.data, r.size);
    ImageResize(&img, width, height);
    Texture2D texture = LoadTextureFromImage(img);
    UnloadImage(img);
    return texture;
}

// inline static AssInfo AssetsInfo[resources_count];
inline static std::array<Texture2D, resources_count> Assets;

// inline constexpr Image LoadIMG(const unsigned char* nameArr, int Datasize, int w = -1, int h = -1){
//     Image image = LoadImageFromMemory(".png", nameArr, Datasize);
//     if( !(w == -1 || h == -1) ) ImageResize(&image, w, h);
//     return image;
// }

// inline constexpr Image LoadIMG(const unsigned char* nameArr, int Datasize, int w = -1, int h = -1){
//     Image image = LoadIMG(nameArr, Datasize, w, h);
//     AssetsInfo[name] = AssInfo { w, h };
//     return image;
// }

// AssInfo GetResInfo(AssetName name){
//     return AssetsInfo[name];
// }

void InitResouces(){
    Image img;
    for (unsigned int i = 0; i < resources_count; i++)
    {
        auto r = resources[i];
        if( r.type == IMG){
            if(contains(r.path, "/ui/")){
                img = LoadImageFromMemory(".png", r.data, r.size);
                Assets[i] = LoadTextureFromImage(img);
                UnloadImage(img);
            }
        }
    }
}


constexpr bool is_eq_cstr(const char* str1, const char* str2) {
    while (*str1 && (*str1 == *str2)) {
        ++str1;
        ++str2;
    }
    return *str1 == *str2;
}

Texture2D GetTexture(const char* path){
    for (unsigned int i = 0; i < resources_count; i++)
    {
        auto r = resources[i];
        if( r.type == IMG){
            if(is_eq_cstr(path, r.path)){
                return Assets[i];
            }
        }
    }
    printf("No ASSET FOUND : %s", path);
    exit(1);
}