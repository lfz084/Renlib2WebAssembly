五子棋小工具， 棋谱阅读器 专用

emcc main.cpp -o RenLib.js -sINITIAL_MEMORY=8mb -sSTACK_SIZE=1mb -sALLOW_MEMORY_GROWTH=1 -sMAXIMUM_MEMORY=4095mb --js-library pkg.js -O2