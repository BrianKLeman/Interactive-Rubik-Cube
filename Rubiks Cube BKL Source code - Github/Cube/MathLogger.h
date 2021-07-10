#pragma once
#include <fstream>
using namespace std;
#include <d3dx9math.h>


class MathLogger
{
public:
MathLogger(string filename);
~MathLogger(void);
void writeHeader(string name);
void writeMatrix(D3DXMATRIX* matrix, string name);
void Write(D3DXVECTOR3* vector, int name);
private:
	fstream* mFile;
};

