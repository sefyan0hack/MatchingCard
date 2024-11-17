#pragma once

enum R_type {
    IMG,
    AUDIO,
};

struct Resource {
    int id;
    R_type type;
    const char* path;
    unsigned char* data;
    unsigned int size;
};

#include "../res/c/img/cards/boruto.h"
#include "../res/c/img/cards/evolved_garou.h"
#include "../res/c/img/cards/geto_young_manga.h"
#include "../res/c/img/cards/Gojo.h"
#include "../res/c/img/cards/itachi.h"
#include "../res/c/img/cards/Jujutsu_Kaisen.h"
#include "../res/c/img/cards/Naruto.h"
#include "../res/c/img/cards/obito_naruto_akatsuki.h"
#include "../res/c/img/cards/sakura_boruto.h"
#include "../res/c/img/cards/Sasuke_Uchiha.h"
#include "../res/c/img/ui/icon.h"
#include "../res/c/img/ui/refresh_icon.h"
#include "../res/c/img/ui/XX.h"

inline static Resource resources[] {
  {0, IMG, "img/cards/boruto.png", img_cards_boruto_png, img_cards_boruto_png_len},
  {1, IMG, "img/cards/evolved_garou.png", img_cards_evolved_garou_png, img_cards_evolved_garou_png_len},
  {2, IMG, "img/cards/geto_young_manga.png", img_cards_geto_young_manga_png, img_cards_geto_young_manga_png_len},
  {3, IMG, "img/cards/Gojo.png", img_cards_Gojo_png, img_cards_Gojo_png_len},
  {4, IMG, "img/cards/itachi.png", img_cards_itachi_png, img_cards_itachi_png_len},
  {5, IMG, "img/cards/Jujutsu_Kaisen.png", img_cards_Jujutsu_Kaisen_png, img_cards_Jujutsu_Kaisen_png_len},
  {6, IMG, "img/cards/Naruto.png", img_cards_Naruto_png, img_cards_Naruto_png_len},
  {7, IMG, "img/cards/obito_naruto_akatsuki.png", img_cards_obito_naruto_akatsuki_png, img_cards_obito_naruto_akatsuki_png_len},
  {8, IMG, "img/cards/sakura_boruto.png", img_cards_sakura_boruto_png, img_cards_sakura_boruto_png_len},
  {9, IMG, "img/cards/Sasuke_Uchiha.png", img_cards_Sasuke_Uchiha_png, img_cards_Sasuke_Uchiha_png_len},
  {10, IMG, "img/ui/icon.png", img_ui_icon_png, img_ui_icon_png_len},
  {11, IMG, "img/ui/refresh_icon.png", img_ui_refresh_icon_png, img_ui_refresh_icon_png_len},
  {12, IMG, "img/ui/XX.png", img_ui_XX_png, img_ui_XX_png_len},
};
constexpr unsigned int resources_count = sizeof(resources)/ sizeof(resources[0]);
