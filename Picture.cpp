#include "Picture.h"
#include <fstream>
#include <iomanip>

// Create image file as 2D array of Pixel objects

int Picture::CreateImage(TLVHelper& tlvData)
{
	ImageFileName = tlvData.GetImageFileName();
	RowLength = tlvData.GetRowLength();
	NumOfRows = tlvData.GetNumOfRows();
	cout << RowLength << " X " << NumOfRows << endl;
	for(int i = 0;i<NumOfRows; i++)
	{
		vector<Pixel> row;
		for(int j=0;j<RowLength;j++)
		{
			row.push_back(tlvData.GetPixelColor(i,j));
		}
		image.push_back(row);
	}
}

int Picture::WritePictureToTextFile()
{
	ofstream writeFile(ImageFileName.c_str(),ios::out);
	writeFile.write(ImageFileName.c_str(), ImageFileName.size());
	writeFile << "\r\n";
	writeFile << NumOfRows << " " << RowLength;
	writeFile << "\r\n";
	writeFile << "\r\n";	
	for(int i = 0;i<NumOfRows; i++)
	{
		vector<Pixel> row;
		for(int j=0;j<RowLength;j++)
		{
			writeFile << hex << setfill('0') << setw(2) << image[i][j].GetRed() << " "
					  << setfill('0') << setw(2) << image[i][j].GetGreen() << " "
					  << image[i][j].GetBlue() << " ";
		}
		writeFile<<"\r\n";
	}
	
	writeFile.close();
}
