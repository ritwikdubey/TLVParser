#include <iostream>
#include "TLVHelper.hh"

int main()
{
    {
        // Unit test 1
        std::string Inputfile = "TestInput.binimage";
        TLVHelper tlvHelper(Inputfile);
        std::cout << "converting :" << Inputfile << std::endl;
        tlvHelper.WritePictureToTextFile();
    }

    {
        // Unit test 2
        std::string Inputfile = "sample.bin";
        TLVHelper tlvHelper(Inputfile);
        std::cout << "converting :" << Inputfile << std::endl;
        tlvHelper.WritePictureToTextFile();
    }
    return 0;
}
