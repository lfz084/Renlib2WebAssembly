#ifndef EM_PORT_API
#	if defined(__EMSCRIPTEN__)
#		include <emscripten.h>
#		if defined(__cplusplus)
#			define EM_PORT_API(rettype) extern "C" rettype EMSCRIPTEN_KEEPALIVE
#		else
#			define EM_PORT_API(rettype) rettype EMSCRIPTEN_KEEPALIVE
#		endif
#	else
#		if defined(__cplusplus)
#			define EM_PORT_API(rettype) extern "C" rettype
#		else
#			define EM_PORT_API(rettype) rettype
#		endif
#	endif
#endif

#include "buffer.cpp"
#include "import.cpp"
#include "console.cpp"
#include "CPoint.cpp"
#include "MoveNode.cpp"
#include "Stack.cpp"
#include "MoveList.cpp"
#include "LibraryFile.cpp"
#include "TextPages.cpp"
#include "RenLibDoc.cpp"
#include "sgf.cpp"

EM_PORT_API(int) init(){
    //log("wasm >> init");
    UINT comment_buffer_size = _ONE_MB*2;
    UINT boardText_buffer_size = _ONE_KB;
    UINT buffer_pages = (comment_buffer_size + boardText_buffer_size) / _PAGE_SIZE + 1;
    
    current_log_buffer = 0;
    current_err_buffer = 0;
    current_comment_buffer = 0;
    current_boardText_buffer = 0;
    current_data_buffer = 0;
    end_data_buffer = _ONE_MB;
    
    comment_buffer = skip_data_pages*_PAGE_SIZE + libFile_buffer + _LIBFILE_BUFFER_SIZE + _ONE_KB;
    boardText_buffer = comment_buffer + comment_buffer_size + _ONE_KB;
    data_buffer = boardText_buffer + boardText_buffer_size + _ONE_KB;
        
    //log("reset m_Stack");
    /*--------------------------------------
        grow() 前不要初始化这几个全局变量，指针会溢出,
        后续在 checkVersion() 里面初始化
    ----------------------------------------*/
    m_Stack = (Stack*)newBuffer(sizeof(Stack));
    m_MoveList = (MoveList*)newBuffer(sizeof(MoveList));
    m_file = (LibraryFile*)newBuffer(sizeof(LibraryFile));
    commentPages = (TextPages*)newBuffer(sizeof(TextPages));
    /*--------------------------------------*/
    
    return int(skip_data_pages);
}

