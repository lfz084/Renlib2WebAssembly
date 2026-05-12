//---------------------- sgf cache --------------------

UINT sgfCacheLeng = 0;
char* sgfCache = (char*)&sgf_buffer;

void _outputSGFCache() {
  if (sgfCacheLeng) {
    outputSGFCache(sgfCache, sgfCacheLeng);
    sgfCacheLeng = 0;
  }
}

void pushSGFChar(char c) {
  sgfCache[sgfCacheLeng++] = c;
  if (sgfCacheLeng >= _SGF_BUFFER_SIZE) {
    _outputSGFCache();
  }
}

void pushSGFSpace(int len, bool isFormat) {
  if (!isFormat) return;
  for(int i = 0; i < len; i++) {
    pushSGFChar(32);
  }
}

void pushSGFNewLine(bool isFormat) {
  if (!isFormat) return;
  pushSGFChar(10);
}

void pushSGFString(CString str, UINT maxLen) {
  for (UINT i = 0; i < maxLen; i++) {
    if (str[i]) {
      pushSGFChar(str[i]);
    }
    else break;
  }
}

//---------------------- lib2sgf --------------------

EM_PORT_API(UINT) getSGFByteLength(bool isFormat) {
  return m_MoveNode_count*(isFormat ? 136: 18) + 32;
}

void pushBoardText(CString str) {
  pushSGFString(str, 4);
}

void pushSGFComment(CString str) {
  pushSGFString(str, 1024);
}

void pushLB(MoveNode* node) {
  MoveNode* next = node->mDown;
  CPoint point;
  CString boardText;
  bool wLB = true;
  while (next) {
    boardText = next->getBoardText();
    if (boardText[0]) {
      point = IdxToPoint(next->mIdx);
      if (wLB) {
        pushSGFChar('L');
        pushSGFChar('B');
        wLB = false;
      }
      pushSGFChar('[');
      pushSGFChar(ALPHA[point.x]);
      pushSGFChar(ALPHA[point.y]);
      pushSGFChar(':');
      pushBoardText(boardText);
      pushSGFChar(']');
    }
    next = next->mRight;
  }
}

void pushComment(MoveNode* node) {
  TextPage* page = commentPages->findTextPage(node);
  if (page && page->text[0]) {
    pushSGFChar('C');
    pushSGFChar('[');
    pushSGFComment(page->text);
    pushSGFChar(']');
  }
}

EM_PORT_API(void) lib2sgf(bool isFormat) {
  CPoint point;
  UINT count = 0;
  int eCount = 0;
  int depth = 1;
  MoveNode* next = rootMoveNode->mDown;
  next->setIsDown(true);
  m_Stack->ClearAll();

  sgfCacheLeng = 0;
  pushSGFChar('(');
  pushSGFNewLine(isFormat);
  eCount += 2;

  pushSGFSpace(eCount, isFormat);
  pushSGFString(";GM[1]CA[gb2312]SZ[15]", 128);
  pushLB(rootMoveNode);
  pushComment(rootMoveNode);
  pushSGFNewLine(isFormat);

  for(int i = 0; i < 256; i++) {
    markStack[i] = 0;
  }

  while(next) {
    count++;
    if(count%300000 == 0) loading(count, m_MoveNode_count);
    if (next->mRight) {
      m_Stack->Push(depth, next->mRight);
      markStack[depth] = 1;
    }

    if (markStack[depth] && next->isDown()) {
      pushSGFSpace(eCount, isFormat);
      pushSGFChar('(');
      pushSGFNewLine(isFormat);
      eCount += 2;
    }

    point = IdxToPoint(next->mIdx);
    pushSGFSpace(eCount, isFormat);
    pushSGFChar(';');
    pushSGFChar(depth % 2 ? 'B': 'W');
    pushSGFChar('[');
    pushSGFChar(ALPHA[point.x]);
    pushSGFChar(ALPHA[point.y]);
    pushSGFChar(']');
    pushLB(next);
    pushComment(next);
    pushSGFNewLine(isFormat);

    if(next->mDown) {
      next = next->mDown;
      next->setIsDown(true);
      depth++;
    }
    else {
      if (!m_Stack->IsEmpty()) {
        int d = 0;
        m_Stack->Pop(d, next);
        next->setIsDown(false);
        for (int i = d+1; i <= depth; i++) {
          if (markStack[i]) {
            eCount -= 2;
            pushSGFSpace(eCount, isFormat);
            pushSGFChar(')');
            pushSGFNewLine(isFormat);
            markStack[i] = 0;
          }
        }
        eCount -= 2;
        pushSGFSpace(eCount, isFormat);
        pushSGFChar(')');
        pushSGFChar('(');
        pushSGFNewLine(isFormat);
        eCount += 2;
        depth = d;
      }
      else {
        for (int i = 1; i <= depth; i++) {
          if (markStack[i]) {
            eCount -= 2;
            pushSGFSpace(eCount, isFormat);
            pushSGFChar(')');
            pushSGFNewLine(isFormat);
            markStack[i] = 0;
          }
        }
        next = 0;
      }
    }
  }


  eCount -= 2;
  pushSGFSpace(eCount, isFormat);
  pushSGFChar(')');

  _outputSGFCache();
}