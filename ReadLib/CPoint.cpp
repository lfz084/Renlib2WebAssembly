class CPoint
{
  public:
  BYTE x;
  BYTE y;
  private:
  const static char* alpha;
  const static char* mbArr;
  static char name[4];
  public:
  CPoint():
  x(0),
  y(0) {}
  CPoint(unsigned char _x, unsigned char _y):
  x(_x),
  y(_y) {}
  char* getName() {
    name[0] = alpha[x-1];
    if(y < 7) {
      name[1] = mbArr[1];
      name[2] = mbArr[16-y-10];
      name[3] = 0;
    }
    else {
      name[1] = mbArr[16-y];
      name[2] = 0;
    }
    return name;
  }
  CPoint operator+(const CPoint& b) {
    CPoint p;
    p.x = this->x + b.x;
    p.y = this->y + b.y;
    return p;
  }
  CPoint operator-(const CPoint& b) {
    CPoint p;
    p.x = this->x - b.x;
    p.y = this->y - b.y;
    return p;
  }
  CPoint operator += (const CPoint& b) {
    this->x += b.x;
    this->y += b.y;
    return *this;
  }
  CPoint operator -= (const CPoint& b) {
    this->x -= b.x;
    this->y -= b.y;
    return *this;
  }
  bool operator == (const CPoint& b) {
    return this->x == b.x && this->y == b.y;
  }
  bool operator != (const CPoint& b) {
    return this->x != b.x || this->y != b.y;
  }
};

const char* CPoint::alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char* CPoint::mbArr = "0123456789";
char CPoint::name[4] = {
  0,
  0,
  0,
  0
};

CPoint* newCPoint() {
  CPoint* p = (CPoint*)newBuffer(sizeof(CPoint));
  p->x = 0;
  p->y = 0;
  return p;
}

CPoint* newCPoint(BYTE x, BYTE y) {
  CPoint* p = (CPoint*)newBuffer(sizeof(CPoint));
  p->x = x;
  p->y = y;
  return p;
}