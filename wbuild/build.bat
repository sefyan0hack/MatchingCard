em++ -o MatchCard.html ..\src\main.cpp -Wall -std=c++23 -D_DEFAULT_SOURCE -Wno-missing-braces -Wunused-result -Os -I. -I ..\res -I ..\raylib-master\src -I ..\raylib-master\src\external -L. -L ..\raylib-master\src -s USE_GLFW=3 -s ASYNCIFY -s TOTAL_MEMORY=10485760 -s FORCE_FILESYSTEM=1 --shell-file ..\raylib-master\src\shell.html ..\raylib-master\src\web\libraylib.a -DPLATFORM_WEB -s "EXPORTED_FUNCTIONS=[_main]" -s EXPORTED_RUNTIME_METHODS=ccall -s ALLOW_MEMORY_GROWTH=1 -flto -s WASM=1 -s ASSERTIONS=0 -s DISABLE_EXCEPTION_CATCHING=1 -s NO_EXIT_RUNTIME=1