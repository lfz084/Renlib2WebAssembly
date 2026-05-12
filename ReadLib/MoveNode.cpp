const int BOARD_TEXT = 0x000100;

const int DOWN = 0x000080;
const int RIGHT = 0x000040;
const int OLD_COMMENT = 0x000020;
const int MARK = 0x000010;
const int COMMENT = 0x000008;
const int START = 0x000004;
const int NO_MOVE = 0x000002;
const int EXTENSION = 0x000001;

const int MASK = 0xFFFF3F;
const CPoint NullPoint(0, 0);
const BYTE NullIdx = 225;

bool isValid(CPoint Pos) {
  return (Pos == NullPoint) ||
  (Pos.x >= 1 && Pos.x <= 15 && Pos.y >= 1 && Pos.y <= 15);
}


bool bit_is_one(int bit_value, UINT value) {
  return ((value & bit_value) != 0);
}

void set_bit(int bit_value, DWORD& value) {
  value |= bit_value;
}

void clear_bit(int bit_value, DWORD& value) {
  value &= ~bit_value;
}

bool isValidPoint(CPoint point) {
  return point.x >= 1 && point.x <= 15 && point.y >= 1 && point.y <= 15;
}

bool isEmpty(CString str) {
  return str == NULL || str[0] == 0;
}

CPoint PosToPoint(BYTE pos) {
  if (pos == 0) {
    return CPoint(0, 0);
  }
  else {
    return CPoint(pos % 16, pos / 16 + 1);
  }
}

BYTE PointToPos(CPoint point) {
  if (isValidPoint(point)) {
    return 16 * (point.y - 1) + point.x;
  }
  else {
    return 0;
  }
}

CPoint IdxToPoint(BYTE idx) {
  if (idx == 225) {
    return CPoint(0, 0);
  }
  else {
    return CPoint(idx % 15 + 1, idx / 15 + 1);
  }
}

BYTE PointToIdx(CPoint point) {
  if (isValidPoint(point)) {
    return 15 * (point.y - 1) + point.x - 1;
  }
  else {
    return 225;
  }
}

BYTE PosToIdx(BYTE pos) {
  if (pos == 0) {
    return 225;
  }
  else {
    return pos % 16 - 1 + pos / 16 * 15;
  }
}

BYTE IdxToPos(BYTE idx) {
  if (idx == 225) {
    return 0;
  }
  else {
    return idx % 15 + 1 + idx / 15 * 16;
  }
}

class MoveNode
{

  public:
  MoveNode():
  mIdx(NullIdx),
  mInfo(0),
  mDown(0),
  mRight(0),
  mBoardText(NULL) {}
  MoveNode(MoveNode& node):
  mIdx(node.mIdx),
  mInfo(node.mInfo),
  mDown(0),
  mRight(0),
  mBoardText(NULL) {}
  MoveNode(BYTE Idx):
  mIdx(Idx),
  mInfo(0),
  mDown(0),
  mRight(0),
  mBoardText(NULL) {}

  //virtual ~MoveNode();

  void setPosInfo(BYTE pos, BYTE info) {
    mIdx = PosToIdx(pos);
    mInfo = (mInfo & 0xFFFF00) | info;
  }
  void getPosInfo(BYTE& pos, BYTE& info) {
    pos = IdxToPos(mIdx);
    info = BYTE(mInfo & 0xFF);
  }

  void setExtendedInfo(BYTE info2, BYTE info1) {
    mInfo &= 0xFF;
    mInfo |= ((info2 << 8) | info1) << 8;
  }
  void getExtendedInfo(BYTE& info2, BYTE& info1) {
    UINT info = mInfo >> 8;
    info1 = BYTE(info & 0xFF);

    info >>= 8;
    info2 = BYTE(info & 0xFF);
  }

  //
  // Position
  //
  void setIdx(BYTE idx) {
    mIdx = idx;
    setIsMove(true);
  }
  BYTE getIdx() {
    return mIdx;
  }
  char* getName() {
    return IdxToPoint(mIdx).getName();
  }

  //
  // Information
  //
  void clearInformation() {
    mInfo = 0;
  }
  bool isInformation() {
    return (mInfo & MASK) != 0;
  }

  bool isDown() {
    return isValue(DOWN);
  }
  void setIsDown(bool value) {
    setIsValue(value, DOWN);
  }

  bool isRight() {
    return isValue(RIGHT);
  }
  void setIsRight(bool value) {
    setIsValue(value, RIGHT);
  }

  bool isOldComment() {
    return isValue(OLD_COMMENT);
  }
  bool isNewComment() {
    return isValue(COMMENT);
  }

  bool isMark() {
    return isValue(MARK);
  }
  void setIsMark(bool value) {
    setIsValue(value, MARK);
  }

  bool isStart() {
    return isValue(START);
  }
  void setIsStart(bool value) {
    setIsValue(value, START);
  }

  bool isMove() {
    return !isValue(NO_MOVE);
  }
  bool isPassMove() {
    bool result = false;
    if (isMove()) {
      result = (mIdx == NullIdx);
    }
    return result;
  }
  void setIsMove(bool value) {
    setIsValue(!value, NO_MOVE);
  }

  bool isExtension() {
    return isValue(EXTENSION);
  }
  void setIsExtension(bool value) {
    setIsValue(value, EXTENSION);
  }

  //
  // Instance data
  //
  void setDown(MoveNode* node) {
    mDown = node;
  }
  MoveNode* getDown() {
    return mDown;
  }

  void setRight(MoveNode* node) {
    mRight = node;
  }
  MoveNode* getRight() {
    return mRight;
  }

  bool isOneLineComment() {
    return false;
  }
  void setOneLineComment(CString comment) {
    //mOneLineComment = comment;
    setIsNewComment(isOneLineComment());
  }
  CString getOneLineComment() {
    return NULL;
  }

  bool isBoardText() {
    return isValue(BOARD_TEXT);
  }
  void setBoardText(CString text) {
    mBoardText = text;
    setIsBoardText(!isEmpty(mBoardText));
  }
  CString getBoardText() {
    return (CString)&mBoardText;
  }

  private:
  void setIsNewComment(bool value) {
    setIsValue(value, COMMENT);
    setIsValue(false, OLD_COMMENT);
  }
  void setIsBoardText(bool value) {
    setIsValue(value, BOARD_TEXT);
    checkExtension();
  }
  void checkExtension() {
    setIsExtension((mInfo & 0xFFFF00) != 0);
  }
  bool isValue(UINT bitValue) {
    return bit_is_one(bitValue, mInfo);
  }
  void setIsValue(bool value, UINT bitValue) {
    if (value) {
      set_bit(bitValue, mInfo);
    }
    else {
      clear_bit(bitValue, mInfo);
    }
  }

  public:
  BYTE mIdx; // position, idx

  BYTE m;

  DWORD mInfo; // information

  //CString        mOneLineComment;   // one line comment
  CString mBoardText; // board text

  MoveNode* mDown; // next level
  MoveNode* mRight; // same level
};

MoveNode* newMoveNode() {
  MoveNode* node = (MoveNode*)newBuffer(sizeof(MoveNode));
  return node;
}

MoveNode* newMoveNode(MoveNode& _node) {
  MoveNode* node = (MoveNode*)newBuffer(sizeof(MoveNode));
  node->mIdx = _node.mIdx;
  node->mInfo = _node.mInfo;
  return node;
}

MoveNode* newMoveNode(BYTE Idx) {
  MoveNode* node = (MoveNode*)newBuffer(sizeof(MoveNode));
  node->mIdx = Idx;
  return node;
}