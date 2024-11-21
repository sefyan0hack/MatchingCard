#include <iostream>
#include <raylib.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>
#include <string>
#include <tuple>

#include "Res.hpp"

#define FPS 30
#define padding 5
#define TOP_HEIGHT screenHeight * 0.09f

struct Card
{
    Texture2D t;
    int id;
    bool active;
};

int main(){
    
    const char* GameName = "Hello, Raylib";

    std::random_device rd;
    std::default_random_engine rng(rd());

    int screenWidth = 800;
    int screenHeight = 600;

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);

    InitWindow(screenWidth, screenHeight, "Game");
    SetWindowPosition(GetMonitorWidth(GetCurrentMonitor())/2 - screenWidth/2, 26);
    SetTargetFPS(FPS);
    Init_Resources();
    InitAudioDevice();
    
    SetWindowIcon(GetImageFromRes("img/ui/icon.png"));
    int TitleFontSize = 30;
    int textWidth = MeasureText(GameName, TitleFontSize);
    int textX = (screenWidth - textWidth) / 2;

    auto sqt_cards = std::sqrt(GetTextureCountFromFolder("cards")*2);
    size_t dimx = sqt_cards;
    size_t dimy = sqt_cards;
    while (dimx * dimy < GetTextureCountFromFolder("cards")*2)
    {
        dimx++;
    }

    std::cout << dimx << "|" << dimy;

    float gab = 3.0f;

    float rectx = screenWidth * 0.05f;
    float recty = TOP_HEIGHT + padding;

    float rectWidth = screenWidth - 2*rectx;
    float rectHeight = screenHeight - (2*padding) - TOP_HEIGHT;
    float width_gab = rectWidth - gab * dimx;
    float height_gab = rectHeight - gab * dimy;

    auto texs_width =  width_gab/float(dimx);
    auto texs_height =  height_gab/float(dimy);

    std::vector<TexInfo> texs = GetTexturesfromfolder("cards", texs_width, texs_height);

    std::vector<Card> map;
    
    for (size_t i = 0; i < texs.size(); i++)
    {
        for (size_t j = 0; j < 2; j++)
        {
            map.push_back( Card {texs[i].t, (int)i, false} );
        }
    }
    //shuffle
    for(unsigned char i = 0; i < (unsigned char)5; i++){
        std::shuffle(map.begin(), map.end(), rng);
    }

    //                     id   position active
    std::vector<std::tuple<int, Vector2, bool*>> GoodMatsh;
    std::string inf = "";
    
    float wrongTimer = 0.0f;
    bool showWrong = false;

    Sound clickSound = GetSound("audio/click.wav");
    Sound wrongSound = GetSound("audio/wrong.wav");
    // Sound goodSound  = GetSound("audio/good.wav");

    while (!WindowShouldClose())
    {
        if (IsWindowResized() && !IsWindowFullscreen())
        {
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();

            rectx = screenWidth * 0.05f;
            recty = TOP_HEIGHT + padding;

            rectWidth = screenWidth - 2*rectx;
            rectHeight = screenHeight - (2*padding) - TOP_HEIGHT;


            width_gab = rectWidth - gab * dimx;
            height_gab = rectHeight - gab * dimy;


            textX = (screenWidth - textWidth) / 2;
        }

        BeginDrawing();

            ClearBackground({117, 117, 117, 255});
            DrawRectangleRec({0,0, (float)screenWidth, TOP_HEIGHT }, {176, 190, 197, 255});
            Rectangle curr;
            
            Vector2 menubarSize = {GetUiTexWidth("menu_bar_optimized.png"), GetUiTexHeight("menu_bar_optimized.png")};
            Rectangle menubarRec = {  screenWidth - padding - menubarSize.x, (TOP_HEIGHT -menubarSize.y)/2.0f, menubarSize.x, menubarSize.y};
            
            if(CheckCollisionPointRec(GetMousePosition(), menubarRec)){
                DrawTexture(GetUiTex("menu_bar_optimized.png"),menubarRec.x, menubarRec.y, Fade(BLACK, 0.9f));
            } else{
                DrawTexture(GetUiTex("menu_bar_optimized.png"), menubarRec.x, menubarRec.y, BLACK);
            }

            float tail_width = width_gab / dimx;
            float tail_height = height_gab / dimy;
            size_t CardCount = 0;
            for (size_t i = 0; i < dimx; i++)
            {
                for (size_t j = 0; j < dimy ; j++)
                {
                    
                    float x = rectx + i * (tail_width + gab);
                    float y = recty + j * (tail_height + gab);
                    
                    auto& card = map[CardCount++];

                    curr = {x, y, tail_width, tail_height };

                    if(CheckCollisionPointRec(GetMousePosition(), curr)){
                        DrawRectangleRounded(curr, 0.1f, 40, {247, 249, 237, 255});
                    } else{
                        DrawRectangleRounded(curr, 0.1f, 40, {247, 249, 237, 255});
                    }

                    auto mousePos = GetMousePosition();
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                        if(showWrong == false){
                            PlaySound(clickSound);
                        }

                        //check if i pressed on the card to push it
                        if(CheckCollisionPointRec(mousePos, curr)){
                            if ( not card.active && showWrong == false)
                            {
                                GoodMatsh.push_back( std::make_tuple( card.id, Vector2 {x, y}, &card.active ) );
                            }
                        }

                        //check if i pressed on the Replay Button to Replay 
                        // if(CheckCollisionPointRec(mousePos, menubarRec)){
                        //     //suffle 2 times :)
                        //     std::shuffle(map.begin(), map.end(), rng);
                        //     std::shuffle(map.begin(), map.end(), rng);
                        //     for (auto &c : map)
                        //     {
                        //         c.active = false;
                        //     }
                        //     GoodMatsh.clear();
                        // }
                    }
                }
            }
            //drwa only Good Matshed imgs
            for (const auto &mcard : GoodMatsh)
            {
                const auto& [id, pos, _] = mcard;
                DrawTexturePro(texs[id].t, {0.0f, 0.0f, texs_width, texs_height}, {pos.x, pos.y, tail_width, tail_height}, {0.0f, 0.0f}, 0.0f, WHITE);
                // DrawTexture(texs[id].t, pos.x, pos.y, WHITE);
            }

            //check if 2 cards are the same
            if(not GoodMatsh.empty()){

                if(GoodMatsh.size() % 2 == 0){
                    auto& [t1_id, t1_pos, t1_active] = *(GoodMatsh.end() - 1);
                    auto& [t2_id, t2_pos, t2_active] = *(GoodMatsh.end() - 2);
                    if(t1_id == t2_id){
                        if(t1_pos.x != t2_pos.x || t1_pos.y != t2_pos.y){
                            *t1_active = true;
                            *t2_active = true;
                        } else{
                            GoodMatsh.pop_back();
                        }
                    } else if (!showWrong) {
                        showWrong = true;
                        wrongTimer = 1.0f;
                        PlaySound(wrongSound);
                    }
                }
            }

            if (showWrong) {
                wrongTimer -= GetFrameTime();
                if (wrongTimer <= 0.0f) {
                    showWrong = false;
                    if (GoodMatsh.size() >= 2) {
                        GoodMatsh.pop_back();
                        GoodMatsh.pop_back();
                    }
                }
            }

            // Draw the wrong texture
            if (showWrong) {
                auto tx = GetUiTex("XX.png");
                float scale = 0.4f * screenWidth/screenHeight;
                DrawTextureEx(tx, {screenWidth/2.0f - tx.width/2.0f*scale, screenHeight/2.0f - tx.height/2.0f*scale}, 0.0f, scale, Fade(WHITE, 0.8f));
            }

            DrawText("Matching Card!", textX, padding, TitleFontSize, DARKBLUE);

            inf += std::to_string(GoodMatsh.size());
            DrawText(inf.c_str(), padding, padding, TitleFontSize, DARKBLUE);
            inf.clear();

            if(GoodMatsh.size() == map.size()){
                static bool gameFinished = false;
                if (!gameFinished) {
                    OpenURL("https://github.com/sefyan0hack?tab=repositories");
                    gameFinished = true;
                    std::shuffle(map.begin(), map.end(), rng);
                    std::shuffle(map.begin(), map.end(), rng);
                    for (auto &c : map)
                    {
                        c.active = false;
                    }
                    GoodMatsh.clear();
                }
            }
            
        EndDrawing();
    }
    CloseAudioDevice();
    CloseWindow();

    return 0;
}