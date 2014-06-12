// Pixel forms the base class for Picture
// Less of a functional use for this exercise
// but more because of the relation of pixel with picture.

class Pixel {
unsigned char red;
unsigned char green;
unsigned char blue;

public:
int GetRed(){return red;};
int GetGreen(){return green;};
int GetBlue(){return blue;};
Pixel( unsigned char r=0, unsigned char g=0, unsigned char b=0):red(r),green(g),blue(b){}
};



