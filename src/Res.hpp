#pragma once
#include <raylib.h>
#include <array>

constexpr size_t ASSETS_COUNT = 32;

struct AssInfo
{
    const char* name;
    size_t w, h;
};


Texture2D LoadTextureWithSize(const char *fileName, int width, int height) {

    Image image = LoadImage(fileName);
    ImageResize(&image, width, height);

    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);

    return texture;
}

Texture2D LoadResexture(AssInfo inf) {

    Image image = LoadImage(inf.name);
    ImageResize(&image, inf.w, inf.h);

    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);

    return texture;
}

enum AssetName{
    REFRECH = 0,
    X,
    Asset_limit
};

inline static AssInfo AssetsInfo[ASSETS_COUNT] {
    [REFRECH] = {"../res/refresh-icon-10853.png", 64, 64},
    [X] = {"../res/X.png", 400, 400},
};

inline static std::array<Texture2D, ASSETS_COUNT> Assets;

AssInfo GetResInfo(AssetName name){
    return AssetsInfo[name];
}

void InitResouces(){
    static_assert(ASSETS_COUNT >= Asset_limit);
    for(size_t i = 0; i < Asset_limit; i++){
        Assets[i] = LoadResexture(AssetsInfo[i]);
    }
}