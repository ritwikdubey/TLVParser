#include <iostream>
#include <map>
#include <vector>
#include "Pixel.h"

using namespace std;

// Working class. The functions here parse the binary file
// maintain color map and coded pixel arrays. 
// Notice that pixel_data here is a 2D array like image in Picture.h
// However, pixel_data would have coded Key for color of each pixel,
// but image will have Pixel oject for each pixel.

class TLVHelper {
string InputFileName;
string ImageFileName;
unsigned char fileType;
int NumOfRows;
int RowLength;
map<unsigned char,vector<unsigned char> > color_table;
vector < vector<unsigned char> > pixel_data;

void SetFileName(vector<unsigned char> buffer);
void SetColorMap(vector<unsigned char> buffer);
void SetPixelData(vector<unsigned char> buffer);
vector<unsigned char> ParseRow(vector<unsigned char> rowCoded);

public:
TLVHelper(string str):InputFileName(str),NumOfRows(0),RowLength(0){}
int ReadBinaryFile();
Pixel GetPixelColor(int i, int j);
string GetImageFileName(){return ImageFileName;};
int GetNumOfRows() {return NumOfRows;};
int GetRowLength() {return RowLength;};
};
