#include <iostream>
#include "Picture.h"
using namespace std;

int main()
{
string Inputfile = "TestInput.binimage";
TLVHelper helperObj(Inputfile);

// Parse the binary file
helperObj.ReadBinaryFile();

Picture image;

// Convert the coded image to 2D array of pixel
image.CreateImage(helperObj);

// Write the image file info to text file
image.WritePictureToTextFile();
}
