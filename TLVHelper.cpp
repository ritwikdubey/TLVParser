#include <fstream>
#include <vector>
#include "TLVHelper.h"
#include <iomanip>

using namespace std;
#define TYPESIZE 1
#define BLOCKSIZE 2
#define COLORMAPSIZE 7
int TLVHelper::ReadBinaryFile()
{
	// find size of the file
	ifstream mystream(InputFileName.c_str(),ios::in|ios::binary);
	mystream.seekg(0,mystream.end);
	int size = mystream.tellg();
	mystream.seekg(0,mystream.beg);

	vector<char> buffer(size);
	mystream.read(buffer.data(),size);
	mystream.close();

	int index = 0;
	fileType = buffer.at(index);
	index += (TYPESIZE + BLOCKSIZE);

	int numType = 3;	// Since there can be only 3 types of sub blocks in any order
	while ( numType-- )
	{
		int type = buffer.at(index);
		int unitSize = (buffer.at(index+1)<<8)|buffer.at(index+2);
		index += (TYPESIZE + BLOCKSIZE);
		vector<unsigned char>SectionBuffer(&buffer[index],&buffer[index+unitSize]);
		switch(type){
			case 2:
			SetFileName(SectionBuffer);
			break;

			case 3:
			SetColorMap(SectionBuffer);
			break;

			case 5:
			SetPixelData(SectionBuffer);
			break;
		
			default:
			cout<< "Invalid b/Not all block types detected" << endl;
		}
		index += unitSize;
	}
	return 1;
}

void TLVHelper::SetFileName(vector<unsigned char> FileNameBuffer)
{
	for ( int count = 0; count < FileNameBuffer.size(); count++ )
	{
		ImageFileName.push_back(FileNameBuffer[count]);
	}
	cout << ImageFileName<<endl; // for debugging purpose
}

void TLVHelper::SetColorMap(vector<unsigned char> ColorMapBuffer)
{
	int numOfEntries = ColorMapBuffer.size()/COLORMAPSIZE;	// color mapping size is constant
	int index = 0;
	for ( int count = 0; count < numOfEntries; count++ )
	{
		index += (TYPESIZE + BLOCKSIZE);
		unsigned char key = ColorMapBuffer.at(index++);
		vector<unsigned char> colorComb(&ColorMapBuffer[index], &ColorMapBuffer[index+3]);

		color_table.insert(map<unsigned char,vector<unsigned char> >::value_type(key,colorComb));
		index += 3;
	}
}

vector<unsigned char> TLVHelper::ParseRow(vector<unsigned char> rowCoded)
{
	vector<unsigned char> row;
	int i = 0;
	while ( i < rowCoded.size() )
	{
		if ( rowCoded.at(i) == 8 )	// individual pixel data
		{
			i += (TYPESIZE + BLOCKSIZE);
			row.push_back(rowCoded.at(i));
			i++;
		}
		else if (rowCoded.at(i) == 7)	// group pixel data converted to individual pixel data
		{
			i += (TYPESIZE + BLOCKSIZE);
			unsigned char num = rowCoded.at(i);
			i++;
			unsigned char key = rowCoded.at(i);
			while (num--)
			{
				row.push_back(key);
			}
			i++;
		}
		else
			cout<<"Error"<<endl;
		
	}
	vector<unsigned char>::iterator t = row.begin();
	return row;
}

void TLVHelper::SetPixelData(vector<unsigned char> PixelDataBuffer)
{
	int index = 0;
	for( ; ((index < PixelDataBuffer.size()) && (PixelDataBuffer.at(index) == 6)); ) // type 0x06 identifies new row
	{
		index++;
		int size = ((PixelDataBuffer.at(index))<<8)|(PixelDataBuffer.at(index+1));
		index += 2;
		
		// make a 2D array of <rows<pixel>>
		vector<unsigned char> rowCoded;
		for ( int count = 0; count < size; count++)
		{
			rowCoded.push_back(PixelDataBuffer.at(index+count));
		}

		vector<unsigned char>row;
		row = ParseRow(rowCoded);
		RowLength = row.size();	// row size
		pixel_data.push_back(row);
		index += size;
	}
	NumOfRows = pixel_data.size();	// number of pixel rows in the coded file
}

Pixel TLVHelper::GetPixelColor(int i1, int j1)
{
	// 1. Get the key at pixel location
	// 2. Get the color combination from color map
	// 3. Initialize and return Pixel object
	
	unsigned char key = (pixel_data[i1][j1]&0xff);
	vector<unsigned char> color = color_table.find(key)->second;

	Pixel p((color[0]&0xff), (color[1]&0xff), (color[2]&0xff));
	return p;
}



