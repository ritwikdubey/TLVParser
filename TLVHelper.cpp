#include <fstream>
#include "TLVHelper.hh"
#include <iomanip>

//#define TYPESIZE 1
//#define BLOCKSIZE 2
//#define COLORMAPSIZE 7
int TLVHelper::ReadBinaryFile(const std::string& inputFileName)
{
	// find size of the file
	std::ifstream mystream(inputFileName.c_str(),std::ios::in|std::ios::binary);
	mystream.seekg(0,mystream.end);
	unsigned long size = mystream.tellg();
	mystream.seekg(0,mystream.beg);

	std::vector<char> buffer(size);
	mystream.read(buffer.data(),size);
	mystream.close();

	int index = 0;
	//fileType = buffer.at(index);
	index += (TYPESIZE + BLOCKSIZE);

	int numType = 3;	// Since there can be only 3 types of sub blocks in any order
	while ( numType-- )
	{
		int type = buffer.at(index);
		int unitSize = (buffer.at(index+1)<<8)|buffer.at(index+2);
		index += (TYPESIZE + BLOCKSIZE);
		std::vector<unsigned char>SectionBuffer(&buffer[index],&buffer[index+unitSize]);
		switch(type){
			case 2:
			if(_imageFileName.empty())
			{
				_imageFileName.assign(SectionBuffer.begin(), SectionBuffer.end());
			}
			//SetFileName(SectionBuffer);
			break;

			case 3:
			SetColorMap(SectionBuffer);
			break;

			case 5:
			SetPixelData(SectionBuffer);
			break;
		
			default:
			std::cout<< "Invalid b/Not all block types detected" << std::endl;
		}
		index += unitSize;
	}
	return 1;
}

void TLVHelper::SetColorMap(const std::vector<u_char>& buffer)
{
	size_t index = 0;
	for ( size_t count = 0; count < buffer.size()/COLORMAPSIZE; count++ )
	{
		index += (TYPESIZE + BLOCKSIZE);
		u_char key = buffer.at(index++);
		color_table.insert({key,{buffer.begin()+index, buffer.begin()+index+3}});
		index += 3;
	}
}

void TLVHelper::ParseRow(const std::vector<u_char>& rowCoded, std::vector<u_char>& row)
{
	size_t i = 0;
	while ( i < rowCoded.size() )
	{
		switch( rowCoded.at(i)) {
			case 8:    // individual pixel data
			{
				i += (TYPESIZE + BLOCKSIZE);
				row.push_back(rowCoded.at(i));
				i++;
				break;
			}
			case 7:    // group pixel data converted to individual pixel data
			{
				i += (TYPESIZE + BLOCKSIZE);
				u_char num = rowCoded.at(i++);
				u_char key = rowCoded.at(i++);
				while (num--) {
					row.push_back(key);
				}
				break;
			}
			default:
			std::cout << "Error: Unknown color code in row" << std::endl;
		}
	}
}

void TLVHelper::SetPixelData(const std::vector<u_char>& buffer)
{
	size_t index = 0;
	while((index < buffer.size()) && (buffer.at(index) == 6)) // type 0x06 identifies new row
	{
		index++;
		int size = ((buffer.at(index))<<8)|(buffer.at(index+1));
		index += 2;
		
		// make a 2D array of <rows<pixel>>
		std::vector<u_char> rowCoded(buffer.begin()+index, buffer.begin()+index+size);
		/*for ( int count = 0; count < size; count++)
		{`
			rowCoded.push_back(PixelDataBuffer.at(index+count));
		}*/

		std::vector<u_char>row;
		ParseRow(rowCoded, row);
		if(_rowLength==0)
		{
			_rowLength = row.size();
		}
		else
		{
			if(_rowLength != row.size())
			{
				std::cout << "Problem, how come this row isn't matching last one"
						  << std::endl;
			}
		}
		pixel_data.push_back(row);
		index += size;
	}
	_numOfRows = pixel_data.size();	// number of pixel rows in the coded file
}

PixelType TLVHelper::GetPixelColor(u_long i, u_long j) const
{
	// 1. Get the key at pixel location
	// 2. Get the color combination from color map
	// 3. Initialize and return Pixel object
	
	u_char key = (pixel_data[i][j]&0xff);
	std::vector<u_char> color = color_table.find(key)->second;

	PixelType p((color[0]&0xff), (color[1]&0xff), (color[2]&0xff));
	return p;
}

void TLVHelper::WritePictureToTextFile()
{
	std::ofstream writeFile(_imageFileName.c_str());
	writeFile << _imageFileName.c_str();
	writeFile << "\r\n";
	writeFile << _numOfRows << " " << _rowLength;
	writeFile << "\r\n";
	writeFile << "\r\n";
	for(u_int i = 0;i<_numOfRows; i++)
	{
		//std::vector<PixelType> row;
		for(u_int j=0;j<_rowLength;j++)
		{
            writeFile << std::hex << std::setw(2) << std::setfill('0')
                      << short(GetPixelColor(i,j).red) << " " << std::setw(2) << std::setfill('0')
                      << short(GetPixelColor(i,j).green) << " " << std::setw(2) << std::setfill('0')
					  << short(GetPixelColor(i,j).blue) << " ";
		}
        writeFile<<"\r\n";
	}

	writeFile.close();
}
