#include <iostream>
#include <unordered_map>
#include <vector>

struct PixelType {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    PixelType(u_char r, u_char g, u_char b):
            red(r),green(g),blue(b){}
};

class TLVHelper {
public:
    TLVHelper(const std::string& inputFile):_imageFileName(""),_numOfRows(0),_rowLength(0)
    {
        ReadBinaryFile(inputFile);
    }
    PixelType GetPixelColor(u_long i, u_long j)  const;
    inline std::string GetImageFileName() const {return _imageFileName;};
    inline u_long GetNumOfRows() const {return _numOfRows;};
    inline u_long GetRowLength() const {return _rowLength;};
    void WritePictureToTextFile();
    TLVHelper(const TLVHelper& rhs) = delete;
    TLVHelper& operator=(const TLVHelper& rhs) = delete;

    ~TLVHelper(){}

private:
    int ReadBinaryFile(const std::string& inputFile);
    void SetColorMap(const std::vector<u_char>& buffer);
    void ParseRow(const std::vector<u_char>& rowCoded, std::vector<u_char>& row);
    void SetPixelData(const std::vector<u_char>& buffer);

    static const size_t TYPESIZE = 1;
    static const size_t BLOCKSIZE = 2;
    static const size_t COLORMAPSIZE = 7;

    std::string _imageFileName;
    u_long _numOfRows;
    u_long _rowLength;
    std::unordered_map<u_int,std::vector<u_char> > color_table;
    std::vector < std::vector<u_char> > pixel_data;
};
