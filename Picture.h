#include "TLVHelper.h"
using namespace std;

// Picture a 2D array of pixels and hence the pixel acts as
// a base class here. Added parameters are length, breadth and
// file name of the file 

class Picture:public Pixel
{
private:
string ImageFileName;
int RowLength;
int NumOfRows;
vector< vector<Pixel> > image;

public:
int CreateImage(TLVHelper& tlvData);
int WritePictureToTextFile();
Picture():RowLength(0),NumOfRows(0),ImageFileName(""){}
};
