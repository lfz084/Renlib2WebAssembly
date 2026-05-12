//---------------log and error--------------------

void log(const char* message) {
  UINT len = 0;
  if(current_log_buffer < _LOG_BUFFER_SIZE-1) {
    log_buffer[current_log_buffer++] = 10;
    while(message[len] && (current_log_buffer < _LOG_BUFFER_SIZE-1)) {
      log_buffer[current_log_buffer++] = message[len++];
    }
    log_buffer[current_log_buffer] = 0;
  }
}

void log(char* message) {
  UINT len = 0;
  if(current_log_buffer < _LOG_BUFFER_SIZE-1) {
    log_buffer[current_log_buffer++] = 10;
    while(message[len] && (current_log_buffer < _LOG_BUFFER_SIZE-1)) {
      log_buffer[current_log_buffer++] = message[len++];
    }
    log_buffer[current_log_buffer] = 0;
  }
}

void onError(const char* message) {
  UINT len = 0;
  if(current_err_buffer < _ERR_BUFFER_SIZE -1) {
    err_buffer[current_err_buffer++] = 10;
    while(message[len] && (current_err_buffer < _ERR_BUFFER_SIZE -1)) {
      err_buffer[current_err_buffer++] = message[len++];
    }
    err_buffer[current_err_buffer] = 0;
  }

  log(message);
}