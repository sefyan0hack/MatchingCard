em++ -o MatchCard.html ..\src\main.cpp -Wall --preload-file ../res@/res -std=c++23 -D_DEFAULT_SOURCE -Wno-missing-braces -Wunused-result -Os -I. -I ..\raylib-master\src -I ..\raylib-master\src\external -L. -L ..\raylib-master\src -s USE_GLFW=3 -s ASYNCIFY -s TOTAL_MEMORY=10485760 -s FORCE_FILESYSTEM=1 --shell-file ..\raylib-master\src\shell.html ..\raylib-master\src\web\libraylib.a -DPLATFORM_WEB -s "EXPORTED_FUNCTIONS=[_main]" -s EXPORTED_RUNTIME_METHODS=ccall -s ALLOW_MEMORY_GROWTH=1