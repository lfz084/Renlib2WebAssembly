# Renlib2WebAssembly
## command
    em++ -sSTACK_SIZE=1mb -sINITIAL_MEMORY=32mb -sALLOW_MEMORY_GROWTH=1 -sMEMORY_GROWTH_GEOMETRIC_STEP=1 -sMAXIMUM_MEMORY=4gb --js-library library.js main.cpp -o web/demo.js