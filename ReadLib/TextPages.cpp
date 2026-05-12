const UINT TEXTPAGE_SIZE = 1024;

struct TextPage
{
  MoveNode* pMoveNode;
  char text[1020];
};

class TextPages
{
  public:
  TextPages(BYTE* buffer, UINT buffer_size) {
    page_start = (TextPage*)buffer;
    page_end = (TextPage*)(buffer+buffer_size);
    m_index = 0;
  }

  TextPage* getTextPage() {
    TextPage* page = page_start + m_index;
    if (page < page_end) {
      m_index++;
      return page;
    }
    return 0;
  }

  TextPage* findTextPage(MoveNode* pNode) {
    int i = 0;
    TextPage* st_page = page_start;
    TextPage* ed_page = page_start + (m_index-1);
    TextPage* c_page = st_page + (ed_page - st_page)/2;
    while(st_page <= ed_page && i++ < 32) {
      if (c_page->pMoveNode == pNode) {
        return c_page;
      }
      else if (c_page->pMoveNode > pNode) {
        ed_page = c_page - 1;
        c_page = st_page + (ed_page - st_page)/2;
      }
      else {
        st_page = c_page + 1;
        c_page = st_page + (ed_page - st_page)/2;
      }
    }
    return 0;
  }
  public:
  TextPage* page_start;
  TextPage* page_end;
  UINT m_index;
};