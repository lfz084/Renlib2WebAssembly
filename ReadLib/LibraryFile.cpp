const int HEADER_SIZE = 20;

const int MAJOR_FILE_VERSION_INDEX = 8;
const int MINOR_FILE_VERSION_INDEX = 9;

const int MAJOR_FILE_VERSION = 3;
const int MINOR_FILE_VERSION = 4;

const int MAJOR_FILE_VERSION_H8 = 3;
const int MINOR_FILE_VERSION_OLD = 0;

const int CENTER = 0x78;


class LibraryFile
{
  public:
  LibraryFile():
  m_indexStart(0),
  m_indexEnd(0),
  m_MajorFileVersion(0),
  m_MinorFileVersion(0) {}

  int Open(int size) {
    m_indexStart = 0;
    m_indexEnd = 0;
    m_MajorFileVersion = 0;
    m_MinorFileVersion = 0;
    return size;
  }

  bool Get(BYTE& data1, BYTE& data2) {
    data1 = 0;
    data2 = 0;
    //log(STR_GET);
    if (m_indexStart >= m_indexEnd) {
      UINT nBytesRead = getBuffer(libFile_buffer, BUFFERSIZE);

      if (nBytesRead == 0) {
        return false;
      }

      m_indexEnd = nBytesRead - 1;
      m_indexStart = 0;
    }

    data1 = libFile_buffer[m_indexStart++];
    data2 = libFile_buffer[m_indexStart++];

    return true;
  }
  bool Get(MoveNode& node) {
    node.setIdx(NullIdx);
    node.clearInformation();

    BYTE data1;
    BYTE data2;

    bool success = Get(data1, data2);

    if (success) {
      node.setPosInfo(data1, data2);

      if (node.isExtension()) {
        success = Get(data1, data2);
        node.setExtendedInfo(data1, data2);
      }
    }
    return success;
  }
  bool CheckVersion() {
    bool VersionOk = false;

    //                            0     1    2    3    4    5    6    7
    BYTE header[HEADER_SIZE] = {
      0xFF,
      'R',
      'e',
      'n',
      'L',
      'i',
      'b',
      0xFF
    };
    UINT dwRead;

    dwRead = getBuffer(libFile_buffer, HEADER_SIZE);
    //log(STR_CHECKING_CODE);
    if (dwRead == HEADER_SIZE) {
      bool HeaderMatch = true;

      for (int i = 0; i <= 7; i++) {
        if (libFile_buffer[i] != header[i]) {
          //cout << "HeaderMatch = false" << endl;
          HeaderMatch = false;
          i = 8;
        }
      }

      if (HeaderMatch) {
        m_MajorFileVersion = libFile_buffer[MAJOR_FILE_VERSION_INDEX];
        m_MinorFileVersion = libFile_buffer[MINOR_FILE_VERSION_INDEX];

        if (100 * m_MajorFileVersion + m_MinorFileVersion <=
          100 * MAJOR_FILE_VERSION + MINOR_FILE_VERSION) {
          VersionOk = true;
        }
      } else if (libFile_buffer[0] == CENTER) {
        VersionOk = true;
      }
    }
    /*
        if (!VersionOk){
               onError(STR_CHECK_VERSION_ERR);
        }
        */
    return VersionOk;
  }

  void Close() {
    m_indexStart = 0;
    m_indexEnd = 0;
    m_MajorFileVersion = 0;
    m_MinorFileVersion = 0;
  }

  CString GetVersion() {
    return m_Version;
  }

  private:
  enum {
    BUFFERSIZE = _LIBFILE_BUFFER_SIZE
  };

  int m_indexStart;
  int m_indexEnd;
  CString m_Version;
  BYTE m_MajorFileVersion;
  BYTE m_MinorFileVersion;
};