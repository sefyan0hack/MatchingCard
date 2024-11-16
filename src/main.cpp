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

    int screenWidth = 640;
    int screenHeight = 720;
    // int screenWidth = 660;
    // int screenHeight = 660;

    SetConfigFlags(FLAG_MSAA_4X_HINT);

    InitWindow(screenWidth, screenHeight, "Game");
    SetWindowPosition(GetMonitorWidth(GetCurrentMonitor())/2 - screenWidth/2, 26);
    SetTargetFPS(FPS);
    InitResouces();

    int TitleFontSize = 30;
    int textWidth = MeasureText(GameName, TitleFontSize);
    int textX = (screenWidth - textWidth) / 2;


    std::vector<Texture2D> texs;
    
    size_t dim = 5;
    size_t gab = 1;

    int rectx = 0 + padding;
    int recty = 0 + 100 + padding;

    int rectWith = screenWidth - (2*padding);
    int rectHeight = screenHeight - (2*padding) - 100;
    int width_gab = rectWith - gab * dim;
    int height_gab = rectHeight - gab * dim;

    for (unsigned int i = 0; i < resources_count; i++)
    {
        auto r = resources[i];
        if( r.type == IMG){
            if(contains(r.path, "/cards/")){
                texs.push_back(LoadTextureFromMemWithSize(r, width_gab/5, height_gab/5));
            }
        }
    }

    std::vector<Card> map;
    
    for (size_t i = 0; i < texs.size(); i++)
    {
        for (size_t j = 0; j < 2; j++)
        {
            map.push_back( Card {texs[i], (int)i, false} );
        }
    }
    //sh
    for(unsigned char i = 0; i < (unsigned char)5; i++){
        std::shuffle(map.begin(), map.end(), rng);
    }

    //                     id   position active
    std::vector<std::tuple<int, Vector2, bool*>> GoodMatsh;
    std::string inf = "";
    while (!WindowShouldClose())
    {
        if (IsWindowResized() && !IsWindowFullscreen())
        {
            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();
        }

        BeginDrawing();

            ClearBackground({128,128,128, 255});
            DrawRectangleRec(CLITERAL(Rectangle) {0,0, (float)screenWidth, 100.0f }, {112, 128, 144, 255});

            Rectangle curr;
            
            Vector2 refAssetsize = {64, 64};
            Rectangle refechBRec = { (float)(screenWidth - (float)refAssetsize.x)/2, screenHeight -  (float)(screenHeight - height_gab)/2 - (float)refAssetsize.y/2 , (float)refAssetsize.x, (float)refAssetsize.y};
            
            if(CheckCollisionPointRec(GetMousePosition(), refechBRec)){
                DrawTextureEx(GetTexture("img/ui/refresh_icon.png"), {refechBRec.x, refechBRec.y}, 0.0f, 0.132f, Fade(BLACK, 0.9f));
            }else{
                DrawTextureEx(GetTexture("img/ui/refresh_icon.png"), {refechBRec.x, refechBRec.y}, 0.0f, 0.13f, BLACK);
            }

            for (size_t i = 0; i < dim; i++)
            {
                for (size_t j = 0; j < dim - 1 ; j++)
                {
                    int x = rectx + i * (width_gab /5 + gab);
                    int y = recty + j * (height_gab /5 + gab);
                    
                    auto& card = map[i * (dim-1) + j];

                    curr = {(float)x, (float)y, (float)width_gab/5, (float)height_gab/5 };

                    // DrawRectangleRec(curr, { 102, 191, 255, 155});
                    if(CheckCollisionPointRec(GetMousePosition(), curr)){
                        DrawRectangleRounded(curr, 0.1f, 60, {122, 211, 255, 255});
                    }else{
                        DrawRectangleRounded(curr, 0.1f, 60, { 102, 191, 255, 255});
                    }
                    // DrawTexture(map[i * (dim-1) + j].t, x, y, WHITE);

                    auto mousePos = GetMousePosition();
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                        //check if i pressed on the card to push it
                        if(CheckCollisionPointRec(mousePos, curr)){
                            if ( not card.active)
                            {
                                GoodMatsh.push_back( std::make_tuple( card.id, Vector2 {(float)x, (float)y}, &card.active ) );
                            }
                        }

                        //check if i pressed on the Replay Button to Replay 
                        if(CheckCollisionPointRec(mousePos, refechBRec)){
                            //suffle 2 times :)
                            std::shuffle(map.begin(), map.end(), rng);
                            std::shuffle(map.begin(), map.end(), rng);
                            for (auto &c : map)
                            {
                                c.active = false;
                            }
                            GoodMatsh.clear();
                        }
                    }
                    
                    //drwa only Good Matshed imgs
                    for (const auto &mcard : GoodMatsh)
                    {
                        const auto& [id, pos, _] = mcard;
                        DrawTexture(texs[id], pos.x, pos.y, WHITE);
                    }

                    //check if 2 cards are the same
                    if(not GoodMatsh.empty() && GoodMatsh.size() >= 2){
                        
                        if(GoodMatsh.size() % 2 == 0){
                            auto& [t1_id, t1_pos, t1_active] = *(GoodMatsh.end() - 1);
                            auto& [t2_id, t2_pos, t2_active] = *(GoodMatsh.end() - 2);

                            if(t1_id == t2_id){
                                if(t1_pos.x == t2_pos.x && t1_pos.y == t2_pos.y){
                                    GoodMatsh.pop_back();
                                }else{
                                    *t1_active = true;
                                    *t2_active = true;
                                    continue;
                                }
                            }else{
                                GoodMatsh.pop_back();
                                GoodMatsh.pop_back();
                            }
                        }                        
                    }
                }
            }

            

            DrawText("Matching Card!", textX, padding, TitleFontSize, GOLD);

            inf += std::to_string(GoodMatsh.size());
            DrawText(inf.c_str(), padding, padding, TitleFontSize, GOLD);
            inf.clear();

            if(GoodMatsh.size() == map.size()){
                OpenURL("https://youtube.com");
                exit(1);
            }
            
        EndDrawing();
    }

    CloseWindow();

    return 0;
}