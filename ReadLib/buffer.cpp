typedef unsigned char BYTE;
typedef unsigned int UINT; 
typedef unsigned short DWORD;
typedef char* CString;

const UINT _ONE_KB = 1024; //1KB
const UINT _PAGE_SIZE = _ONE_KB*64; //64K
const UINT _ONE_MB = _ONE_KB*1024; //1MB
const UINT _IO_BUFFER_SIZE = _PAGE_SIZE;
const UINT _ERR_BUFFER_SIZE = _ONE_MB;
const UINT _LOG_BUFFER_SIZE = _ONE_MB;
const UINT _SGF_BUFFER_SIZE = _ONE_MB*2;
const UINT _COMMENT_BUFFER_SIZE = _ONE_MB*2;
const UINT _LIBFILE_BUFFER_SIZE = _ONE_MB;

char _empty0[_ONE_KB] = {0};

const char ALPHA[] = "aabcdefghijklmnopqrstuvwxyz";

char MoveNode_Name_buffer[4] = {0};
char bit32_buffer[4] = {0};

UINT current_log_buffer = 0;
UINT current_err_buffer = 1;
UINT current_comment_buffer = 2;
UINT current_boardText_buffer = 3;
UINT current_data_buffer = 4;
UINT end_data_buffer = 5;

const UINT skip_data_pages = 32;

BYTE markStack[256] = {0};
char _empty1[_ONE_KB] = {0};

BYTE out_buffer[_IO_BUFFER_SIZE] = {0};
char _empty2[_ONE_KB] = {0};
BYTE in_buffer[_IO_BUFFER_SIZE] = {0};
char _empty3[_ONE_KB] = {0};

BYTE log_buffer[_LOG_BUFFER_SIZE] = {0}; //8M
char _empty4[_ONE_KB] = {0};
BYTE err_buffer[_ERR_BUFFER_SIZE] ={0}; // 1M
char _empty5[_ONE_KB] = {0};

BYTE sgf_buffer[_SGF_BUFFER_SIZE] = {0};
char _empty6[_ONE_KB] = {0};

BYTE libFile_buffer[_LIBFILE_BUFFER_SIZE] = {0};
char _empty7[_ONE_KB] = {0};

BYTE* comment_buffer;
BYTE* boardText_buffer;
BYTE* data_buffer; // Stack(1804) + MoveList(908) + LibraryFile(16) + all MoveNode......

//--------------------------------------

EM_PORT_API(BYTE*) getOutBuffer(){
    return out_buffer;
}

EM_PORT_API(BYTE*) getInBuffer(){
    return in_buffer;
}

EM_PORT_API(BYTE*) getLogBuffer(){
    return log_buffer;
}

EM_PORT_API(BYTE*) getErrorBuffer(){
    return err_buffer;
}

//----------------- comment buffer --------------------

BYTE* getCommentBuffer(){
    return comment_buffer;
}

//----------------- boardText buffer --------------------

BYTE* getBoardTextBuffer(){
    return boardText_buffer;
}

//----------------- data buffer --------------------

EM_PORT_API(BYTE*) getDataBuffer(){
    return data_buffer;
}

BYTE* newBuffer(UINT size){
    if(current_data_buffer + size > end_data_buffer){
        current_data_buffer = end_data_buffer;
    	return 0;
    }
    current_data_buffer += size;
    return &data_buffer[current_data_buffer-size];
}

//------------------ LibraryFile Buffer ---------------

BYTE* getLibFileBuffer(){
    return libFile_buffer;
}

EM_PORT_API(void) setMemoryEnd(UINT size) {
    end_data_buffer = size;
}