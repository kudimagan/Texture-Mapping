#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define D 5
#define fine 100
#define SX (D*fine)
#define row 10
#define MAX 255

int main()
{
	unsigned char fi[SX][SX];
	int i, j, rx, ry;

	int rs = SX/row;
	for (i = 0; i < SX; ++i)
	{
		for (j = 0; j < SX; ++j)
		{
			rx = i / rs;
			ry = j / rs;
			fi[i][j] = ((rx + ry) % 2) * 255;
		}
	}

	string fn = "./cube.pgm";
	ofstream fp(fn.c_str(), ios::out);

	// file header
	fp << "P5\n" << SX << " " << SX << "\n" << MAX << "\n";

	//unsigned char z = 0;
	// writing the pixel intensities to the file
	for (i = 0; i < SX; ++i)
		for (j = 0; j < SX; ++j)
			fp << fi[i][j];

	fp.close();
}