#include "MathLogger.h"

MathLogger::MathLogger(string filename)
	: mFile( new fstream(filename.c_str(),ios_base::out) )
{	

}

MathLogger::~MathLogger(void)
{
	mFile->close();
	delete mFile;
}

void MathLogger::Write(D3DXVECTOR3* vector, int number)
{
	*mFile<<endl<<"----------------------------------------------"<<endl;
	*mFile<<number<<endl<<"VECTOR"<<endl<<endl;
	*mFile<<vector->x<<endl;
	*mFile<<vector->y<<endl;
	*mFile<<vector->z<<endl;
}

void MathLogger::writeHeader(string name)
{
	*mFile<<name.c_str()<<endl<<endl;
}