class MoveList
{
  public:
  MoveList() {
    ClearAll();
  }

  bool IsEmpty() {
    return m_nIndex == -1;
  }
  bool IsFull() {
    return m_nIndex == MAXINDEX;
  }

  void SetRoot(MoveNode* pMove) {
    /*if(!IsEmpty()){
            onError(STR_MOVELIST_SETROOT_ERR);
        }*/
    Add(pMove);
    //m_List[++m_nIndex] = pMove;
  }
  void Add(MoveNode* pMove) {
    /*if(m_nIndex >= MAXINDEX){
            onError(STR_MOVELIST_FULL);
        }*/
    m_List[++m_nIndex] = pMove;
  }
  void Swap(int nIndex1, int nIndex2) {
    /*if(nIndex1 < 1 || nIndex1 > m_nIndex || nIndex2 < 1 || nIndex2 > m_nIndex){
            onError(STR_MOVELIST_SWAP_ERR);
        }*/
    MoveNode* temp = m_List[nIndex1];
    m_List[nIndex1] = m_List[nIndex2];
    m_List[nIndex2] = temp;
  }

  MoveNode* Get(int nIndex) {
    /*if(IsEmpty() ||
        nIndex < 0 || nIndex > MAXINDEX){
            onError(STR_MOVELIST_GET_ERR);
        }*/
    return m_List[nIndex];
  }
  MoveNode* GetRoot() {
    /*if(IsEmpty()){
            onError(STR_MOVELIST_GETROOT_ERR);
        }*/
    return m_List[0];
  }
  MoveNode* Current() {
    /*if(IsEmpty()){
            onError(STR_MOVELIST_CURRENT_ERR);
        }*/
    return m_List[m_nIndex];
  }

  MoveNode* Next() {
    if (m_nIndex < MAXINDEX) {
      return m_List[m_nIndex + 1];
    }
    else {
      return 0;
    }
  }
  MoveNode* Previous() {
    if (m_nIndex > 0) {
      return m_List[m_nIndex - 1];
    }
    else {
      return 0;
    }
  }

  void SetIndex(int nIndex) {
    /*if(nIndex < 0 || nIndex > m_nIndex){
            onError(STR_MOVELIST_SETINDEX_ERR);
        }*/
    m_nIndex = nIndex;
  }
  void SetRootIndex() {
    SetIndex(0);
  }
  void Decrement() {
    /*if(m_nIndex <= 0){
            onError(STR_MOVELIST_DECREMENT_ERR);
        }*/
    m_nIndex--;
  }
  int Index() {
    return m_nIndex;
  }

  void ClearAll() {
    for (int i = 0; i <= MAXINDEX; i++) {
      m_List[i] = 0;
    }

    m_nIndex = -1;
  }
  void ClearEnd() {
    for (int i = m_nIndex + 1; i <= MAXINDEX; i++) {
      m_List[i] = 0;
    }
  }

  private:
  enum {
    MAXINDEX = 225
  };
  MoveNode* m_List [MAXINDEX + 1]; // 1 based
  int m_nIndex;
};