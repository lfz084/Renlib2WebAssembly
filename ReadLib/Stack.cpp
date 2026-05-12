class Stack
{
  public:
  Stack() {
    ClearAll();
  }

  void ClearAll() {
    m_nIndex = 0;
    for (int i = 0; i < SIZE; i++) {
      m_Stack[i].nMove = 0;
      m_Stack[i].pMove = 0;
    }
  }

  bool IsEmpty() {
    return (m_nIndex == 0);
  }

  void Push(int nMove, MoveNode* pMove) {
    /*if (m_nIndex >= SIZE){
            onError(STR_STACK_FULL_0);
        }*/

    m_Stack[m_nIndex].nMove = nMove;
    m_Stack[m_nIndex].pMove = pMove;
    m_nIndex++;
  }
  void Pop(int& nMove, MoveNode*& pMove) {
    /*if(m_nIndex <= 0){
            onError(STR_STACK_EMPTY_0);
        }*/

    m_nIndex--;
    nMove = m_Stack[m_nIndex].nMove;
    pMove = m_Stack[m_nIndex].pMove;
  }

  void Push(MoveNode* pMove) {
    /*if (m_nIndex >= SIZE){
            onError(STR_STACK_FULL_1);
        }*/

    m_Stack[m_nIndex].nMove = 0;
    m_Stack[m_nIndex].pMove = pMove;
    m_nIndex++;
  }
  void Pop(MoveNode*& pMove) {
    /*if(m_nIndex <= 0){
            onError(STR_STACK_EMPTY_1);
        }*/

    m_nIndex--;
    pMove = m_Stack[m_nIndex].pMove;
  }

  void Push(int nMove) {
    /*if (m_nIndex >= SIZE){
               onError(STR_STACK_FULL_2);
        }*/

    m_Stack[m_nIndex].nMove = nMove;
    m_Stack[m_nIndex].pMove = NULL;

    m_nIndex++;
  }
  void Pop(int& nMove) {
    /*if(m_nIndex <= 0){
            onError(STR_STACK_EMPTY_2);
        }*/

    m_nIndex--;
    nMove = m_Stack[m_nIndex].nMove;
  }
  int Index() {
    return m_nIndex;
  }

  private:
  struct Item
  {
    int nMove;
    MoveNode* pMove;
  };
  enum {
    SIZE = 225
  };
  Item m_Stack [SIZE];
  int m_nIndex;
};