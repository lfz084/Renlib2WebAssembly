//--------------------- Doc -------------------------

MoveNode* rootMoveNode = 0;
Stack* m_Stack = 0;
MoveList* m_MoveList = 0;
LibraryFile* m_file = 0;
TextPages* commentPages = 0;

int msb(BYTE ch) {
  return (ch & 0x80);
}

bool LessThan(CPoint Left, CPoint Right) {
  return PointToPos(Left) < PointToPos(Right);
}

void readOldComment(CString pStrOneLine) {
  BYTE data1;
  BYTE data2;
  pStrOneLine[0] = 0;
  do {
    m_file->Get(data1, data2);
  }while(msb(data1) == 0 && msb(data2) == 0);

}

UINT readNewComment(CString pStrOneLine) {
  BYTE data1;
  BYTE data2;
  UINT len = 0;

  do {
    m_file->Get(data1, data2);
    if(len < 1018) {
      pStrOneLine[len++] = data1;
      pStrOneLine[len++] = data2;
    }
  }while(data1 && data2);
  pStrOneLine[len + 1] = 0;
  pStrOneLine[len + 2] = 0;
  return len;
}

UINT readBoardText(CString pStrBoardText) {
  BYTE data1;
  BYTE data2;
  UINT len = 0;

  do {
    m_file->Get(data1, data2);
    if(len < 4) {
      pStrBoardText[len++] = data1;
      pStrBoardText[len++] = data2;
    }

  }while(data1 && data2);

  return len;
}

void addMove(MoveNode* pMove, MoveNode* pNewMove) {
  if (pMove->mDown == 0) {
    pMove->setDown(pNewMove);
  }
  else {
    if (pNewMove->mIdx < pMove->mDown->mIdx) {
      pNewMove->setRight(pMove->mDown);
      pMove->setDown(pNewMove);
    }
    else {
      pMove = pMove->mDown;
      bool br = false;
      while (!br) {
        if (pMove->mRight == 0) {
          pMove->setRight(pNewMove);
          br = true;
        }
        else if (pNewMove->mIdx < pMove->mRight->mIdx) {
          pNewMove->setRight(pMove->mRight);
          pMove->setRight(pNewMove);
          br = true;
        }
        pMove = pMove->mRight;
      }
    }
  }
}

void addAttributes(MoveNode* pMove, MoveNode* pFrom, bool& bMark, bool& bMove, bool& bStart) {
  bMark = false;
  bMove = false;
  bStart = false;

  if (pFrom->isMark() && !pMove->isMark()) {
    bMark = true;
    pMove->setIsMark(bMark);
  }

  if (pFrom->isMove() && !pMove->isMove()) {
    bMove = true;
    pMove->setIsMove(bMove);
  }

  if (pFrom->isStart() && !pMove->isStart()) {
    bStart = true;
    pMove->setIsStart(bStart);
  }
}

MoveNode* getVariant(MoveNode* pMove, BYTE Idx) {
  if (pMove->mDown) {
    //RenLib 3.6 标准
    pMove = pMove->mDown;

    if (pMove->mIdx == Idx) return pMove;

    while (pMove->mRight) {
      pMove = pMove->mRight;
      if (pMove->mIdx == Idx) return pMove;
    }
  }

  return 0;
}

EM_PORT_API(UINT) getAutoMove() {
  MoveNode* pMove = rootMoveNode;
  CPoint* pPos = (CPoint*)out_buffer;
  UINT len = 0;
  while (pMove->mDown) {
    pMove = pMove->mDown;
    if (pMove->mRight == 0) {
      pPos[len++] = IdxToPoint(pMove->mIdx);
    }
    else {
      break;
    }
  }
  return len;
}

//-------------------- get Branch --------------------

struct Item {
  int nMove;
  MoveNode* pMove;
};

struct Node {
  CPoint mPos;
  char* txt;
  UINT color;
};

struct InnerHTMLInfo {
  char* innerHTML;
  int depth;
};

void findNode(MoveNode*& pMove, CPoint pos) {
  while (pMove) {
    if (IdxToPoint(pMove->mIdx) == pos) {
      break;
    }
    else if (pMove->mRight) {
      pMove = pMove->mRight;
    }
    else {
      pMove = 0;
      break;
    }
  }
}

EM_PORT_API(void) searchInnerHTMLInfo(CPoint* posArr, UINT len) {
  MoveNode* pMove = rootMoveNode;
  CPoint* pPos = (CPoint*)in_buffer;
  struct InnerHTMLInfo* innerHTMLInfo = (struct InnerHTMLInfo*)out_buffer;
  innerHTMLInfo->innerHTML = NULL; //pMove->mOneLineComment;
  innerHTMLInfo->depth = -1;

  for (UINT i = 0; i < len; i++) {
    if (pMove->mDown) {
      pMove = pMove->mDown;
      findNode(pMove, pPos[i]);
      if (pMove) {
        if(pMove->isNewComment() && i == len - 1) {
          TextPage* page = commentPages->findTextPage(pMove);
          if (page) {
            innerHTMLInfo->innerHTML = page->text;
            innerHTMLInfo->depth = i;
          }
        }
      }
      else {
        break;
      }
    }
    else {
      break;
    }
  }
}

int indexOf(CPoint Pos, CPoint* posArr, int len) {
  for(int i = 0; i < len; i++) {
    if(posArr[i] == Pos) return i;
  }
  return -1;
}

int indexOfNode(CPoint Pos, struct Node* nodes, int len) {
  for(int i = 0; i < len; i++) {
    if(nodes[i].mPos == Pos) return i;
  }
  return -1;
}

EM_PORT_API(void) getBranchNodes(CPoint* posArr, int len) {
  bool done = false;
  int* count = (int*)out_buffer;
  *count = 0;
  struct Node* Nodes = (struct Node*)&out_buffer[4];
  struct Node* pNode = Nodes;
  MoveNode* pMove = rootMoveNode->mDown;
  MoveNode** moveList = (MoveNode**)&in_buffer[1024];
  int listLength = 0;
  struct Item* stack = (struct Item*)&in_buffer[2048];
  int stackLength = 0;
  struct Item jointNode = {
    0,
    0
  };
  while(!done) {

    if(pMove) {
      int idx = indexOf(IdxToPoint(pMove->mIdx), posArr, len);
      moveList[listLength++] = pMove;

      //if(listLength <= len + 1){
      if(pMove->mRight) {
        stack[stackLength].pMove = pMove->mRight;
        stack[stackLength++].nMove = listLength;
      }

      if(listLength <= len) {
        if(idx > -1 && idx%2 == (listLength-1)%2) {
          pMove = pMove->mDown;
        }
        else if(idx == -1 && jointNode.pMove == 0 && listLength%2 == (len+1)%2) {
          jointNode.nMove = listLength;
          jointNode.pMove = pMove;
          pMove = pMove->mDown;
        }
        else {
          pMove = 0;
        }
      }
      else if(listLength == len + 1) {
        if(idx >= 0) {
          int idxNode = indexOfNode(IdxToPoint(jointNode.pMove->mIdx), Nodes, *count);
          if(idxNode > -1) {
            if(Nodes[idxNode].txt == 0) {
              Nodes[idxNode].txt = pMove->getBoardText();
              Nodes[idxNode].color = 0;
            }
          }
          else {
            pNode->mPos = IdxToPoint(jointNode.pMove->mIdx);
            pNode->txt = pMove->getBoardText();
            pNode->color = 0;
            pNode++;
            (*count)++;
          }
        }
        else if(idx == -1 && jointNode.pMove == 0) {
          int idxNode = indexOfNode(IdxToPoint(pMove->mIdx), Nodes, *count);
          if(idxNode > -1) {
            if(Nodes[idxNode].txt == 0) {
              Nodes[idxNode].txt = pMove->getBoardText();
              Nodes[idxNode].color = 0;
            }
          }
          else {
            pNode->mPos = IdxToPoint(pMove->mIdx);
            pNode->txt = pMove->getBoardText();
            pNode->color = 0;
            pNode++;
            (*count)++;
          }
        }
        pMove = 0;
      }
      //}
      //else{
      //pMove = 0;
      //}
    }
    else if(stackLength > 0) {
      pMove = stack[--stackLength].pMove;
      listLength = stack[stackLength].nMove - 1;
      if(jointNode.pMove && jointNode.nMove > listLength) {
        jointNode.nMove = 0;
        jointNode.pMove = 0;
      }
    }
    else {
      done = true;
    }

  }
}

//-------------------- RenLib Tree ------------------

UINT m_MoveNode_count = 0;

UINT* getMoveNodeCountBuffer() {
  return &m_MoveNode_count;
}

EM_PORT_API(bool) checkVersion() {
  if(m_file->CheckVersion()) {
    m_MoveList->ClearAll();
    m_Stack->ClearAll();
    commentPages->page_start = (TextPage*)comment_buffer;
    commentPages->page_end = (TextPage*)(comment_buffer+_COMMENT_BUFFER_SIZE);
    commentPages->m_index = 0;

    MoveNode* pCurrentMove = 0;

    if (m_MoveList->IsEmpty()) {
      pCurrentMove = (MoveNode*)newBuffer(sizeof(MoveNode));
      pCurrentMove->mIdx = NullIdx;
      m_MoveList->SetRoot(pCurrentMove);
      rootMoveNode = pCurrentMove;
    }
    else {
      pCurrentMove = m_MoveList->GetRoot();
      m_MoveList->SetRootIndex();
      rootMoveNode = pCurrentMove;
    }
    return true;
  }
  else {
    return false;
  }
}

EM_PORT_API(UINT) loadAllMoveNode() {
  m_MoveNode_count = 0;
  MoveNode* next = (MoveNode*)newBuffer(sizeof(MoveNode));

  //log("next = newMoveNode()");
  int len = 0;
  char* str;
  while(m_file->Get(*next)) {
    m_MoveNode_count++;
    if (next->isOldComment()) {
      //log("readOldComment");
      str = (char*)in_buffer;
      readOldComment(str);
      next->mInfo &= ~(OLD_COMMENT | COMMENT);
    }

    if(next->isNewComment()) {
      //log("readNewComment");
      TextPage* page = commentPages->getTextPage();
      if (page) {
        page->pMoveNode = next;
        str = page->text;
      }
      else {
        str = (char*)in_buffer;
        next->mInfo &= ~(OLD_COMMENT | COMMENT);
      }
      len = readNewComment(str);
    }

    if (next->isBoardText()) {
      //log("readBoardText");
      str = (char*)in_buffer; // (char*)&boardText_buffer[current_boardText_buffer];
      len = readBoardText((CString)&(next->mBoardText));
    }

    next = (MoveNode*)newBuffer(sizeof(MoveNode));
    if (next == 0) {
      memoryBound();
      break;
    }
  }

  return m_MoveNode_count;
}

EM_PORT_API(bool) createRenjuTree() {
  bool bMark = false;
  bool bMove = false;
  bool bStart = false;
  MoveNode* pCurrentMove = rootMoveNode;
  MoveNode* pNextMove = 0;
  MoveNode* next = pCurrentMove;

  m_MoveList->SetRootIndex();

  for(UINT i = 0; i < m_MoveNode_count; i++) {
    next++;
    if(i%300000 == 0) loading(i, m_MoveNode_count);

    BYTE idx = next->mIdx;
    
    if (idx == NullIdx) {
      //log(STR_SKIP_ROOT_NODE);
    }
    else if (idx < 0 || idx > 225) {
      //log(STR_CHECKING_CODE_ERR);
      //return false;
    }
    else {
      //log(STR_GETVARIANT);
      pNextMove = getVariant(pCurrentMove, next->mIdx);
      if (pNextMove) {
        pCurrentMove = pNextMove;
        //if(pCurrentMove->mOneLineComment==0) pCurrentMove->mOneLineComment = next->mOneLineComment;
        if(pCurrentMove->mBoardText == 0) pCurrentMove->mBoardText = next->mBoardText;
        addAttributes(pCurrentMove, next, bMark, bMove, bStart);
      }
      else {
        pNextMove = next;
        addMove(pCurrentMove, pNextMove);
        pCurrentMove = pNextMove;
      }
      //log(STR_GETVARIANT);
      m_MoveList->Add(pCurrentMove);
    }

    if (next->isDown()) {
      if(m_MoveList->Index() > 0) m_Stack->Push(m_MoveList->Index());
    }

    if (next->isRight()) {
      if (!m_Stack->IsEmpty()) {
        int nMove = 0;
        m_Stack->Pop(nMove);
        m_MoveList->SetIndex(nMove - 1);
        pCurrentMove = m_MoveList->Current();
      }
      else {
        m_MoveList->SetRootIndex();
        pCurrentMove = m_MoveList->GetRoot();
      }
    }
  }
  return true;
}