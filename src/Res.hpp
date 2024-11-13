#pragma once
#include <raylib.h>
#include <array>

#include <all_res.hpp>

#define LoadMEx(nm, name, w, h) Load(nm, img_##name##_png, img_##name##_png_len, w, h)
#define LoadM(name) Load(img_##name##_png, img_##name##_png_len)

constexpr size_t ASSETS_COUNT = 32;

struct AssInfo
{
    // const char* name;
    int w, h;
};


Texture2D LoadTextureFromImageWithSize(Image img, int width, int height) {
    ImageResize(&img, width, height);
    Texture2D texture = LoadTextureFromImage(img);
    // UnloadImage(img);
    return texture;
}


enum AssetName{
    REFRECH = 0,
    X,
    Asset_limit
};

inline static AssInfo AssetsInfo[ASSETS_COUNT];
inline static std::array<Texture2D, ASSETS_COUNT> Assets;

inline constexpr Image Load(const unsigned char* nameArr, int Datasize, int w = -1, int h = -1){
    Image image = LoadImageFromMemory(".png", nameArr, Datasize);
    if( !(w == -1 || h == -1) ) ImageResize(&image, w, h);
    return image;
}

inline constexpr Image Load(AssetName name, const unsigned char* nameArr, int Datasize, int w = -1, int h = -1){
    Image image = Load(nameArr, Datasize, w, h);
    AssetsInfo[name] = AssInfo { w, h };
    return image;
}


inline static std::array<Image, ASSETS_COUNT> AssetsIMG {
    LoadMEx(REFRECH, refresh_icon, 64, 64),
    LoadMEx(X, XX, 400, 400),
};


AssInfo GetResInfo(AssetName name){
    return AssetsInfo[name];
}

void InitResouces(){
    static_assert(ASSETS_COUNT >= Asset_limit);

    for(size_t i = 0; i < Asset_limit; i++){
        Assets[i] = LoadTextureFromImage(AssetsIMG[i]);
    }
}

inline static std::vector<Image> Cardimgs{
    LoadM(Gojo),
    LoadM(Jujutsu_Kaisen),
    LoadM(obito_naruto_akatsuki),
    LoadM(geto_young_manga),
    LoadM(itachi),
    LoadM(Naruto),
    LoadM(Sasuke_Uchiha),
    LoadM(sakura_boruto),
    LoadM(boruto),
    LoadM(evolved_garou),
};