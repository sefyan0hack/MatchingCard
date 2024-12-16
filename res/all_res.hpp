#pragma once

enum R_type {
    IMG,
    AUDIO,
};

struct Resource {
    int id;
    R_type type;
    const char* path;
    const unsigned char* data;
    const unsigned int size;
};

#if __cplusplus < 201103L
#define constexpr  
#endif

#if __cplusplus < 201703L
#define inline  
#endif

#include "c/audio/click.h"
#include "c/audio/good.h"
#include "c/audio/wrong.h"
#include "c/img/cards/boruto.h"
#include "c/img/cards/evolved_garou.h"
#include "c/img/cards/geto_young_manga.h"
#include "c/img/cards/Gojo.h"
#include "c/img/cards/itachi.h"
#include "c/img/cards/Jujutsu_Kaisen.h"
#include "c/img/cards/Naruto.h"
#include "c/img/cards/obito_naruto_akatsuki.h"
#include "c/img/cards/sakura_boruto.h"
#include "c/img/cards/Sasuke_Uchiha.h"
#include "c/img/ui/icon.h"
#include "c/img/ui/menu_bar.h"
#include "c/img/ui/menu_bar_optimized.h"
#include "c/img/ui/refresh_icon.h"
#include "c/img/ui/XX.h"

inline static Resource resources[] {
	{0, AUDIO, "audio/click.wav", __audio_click, __audio_click_len },
	{1, AUDIO, "audio/good.wav", __audio_good, __audio_good_len },
	{2, AUDIO, "audio/wrong.wav", __audio_wrong, __audio_wrong_len },
	{3, IMG, "img/cards/boruto.png", __img_cards_boruto, __img_cards_boruto_len },
	{4, IMG, "img/cards/evolved_garou.png", __img_cards_evolved_garou, __img_cards_evolved_garou_len },
	{5, IMG, "img/cards/geto_young_manga.png", __img_cards_geto_young_manga, __img_cards_geto_young_manga_len },
	{6, IMG, "img/cards/Gojo.png", __img_cards_Gojo, __img_cards_Gojo_len },
	{7, IMG, "img/cards/itachi.png", __img_cards_itachi, __img_cards_itachi_len },
	{8, IMG, "img/cards/Jujutsu_Kaisen.png", __img_cards_Jujutsu_Kaisen, __img_cards_Jujutsu_Kaisen_len },
	{9, IMG, "img/cards/Naruto.png", __img_cards_Naruto, __img_cards_Naruto_len },
	{10, IMG, "img/cards/obito_naruto_akatsuki.png", __img_cards_obito_naruto_akatsuki, __img_cards_obito_naruto_akatsuki_len },
	{11, IMG, "img/cards/sakura_boruto.png", __img_cards_sakura_boruto, __img_cards_sakura_boruto_len },
	{12, IMG, "img/cards/Sasuke_Uchiha.png", __img_cards_Sasuke_Uchiha, __img_cards_Sasuke_Uchiha_len },
	{13, IMG, "img/ui/icon.png", __img_ui_icon, __img_ui_icon_len },
	{14, IMG, "img/ui/menu_bar.png", __img_ui_menu_bar, __img_ui_menu_bar_len },
	{15, IMG, "img/ui/menu_bar_optimized.png", __img_ui_menu_bar_optimized, __img_ui_menu_bar_optimized_len },
	{16, IMG, "img/ui/refresh_icon.png", __img_ui_refresh_icon, __img_ui_refresh_icon_len },
	{17, IMG, "img/ui/XX.png", __img_ui_XX, __img_ui_XX_len },
};

constexpr unsigned int resources_count = sizeof(resources)/ sizeof(resources[0]);

#define IMGCOUNT 15
#define SOUNDCOUNT 3
#undef constexpr
#undef inline