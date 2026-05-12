五子棋小工具， 棋谱阅读器 专用

emcc main.cpp -o RenLib.js -sSTACK_SIZE=1mb -sALLOW_MEMORY_GROWTH=1 -sMEMORY_GROWTH_GEOMETRIC_STEP=1 -sMAXIMUM_MEMORY=4gb --js-library pkg.js