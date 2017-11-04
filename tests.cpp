//#include <stb_image.h>
#include <iostream>
#include "matrix.hpp"

using namespace std;

float A[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

float B[] = { 2, 2, 2, 2, 2, 2, 2, 2, 2 };

float C[] = { 2, 2, 2, 3, 2 };

float I[] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

float AA[] = { 30, 36, 42, 66, 81, 96, 102, 126, 150 };

int main(int argc, char** argv) {
	int width, height, bpp;

	//stbi_uc* pixels = stbi_load("./", &width, &height, &bpp, 0);

	Matrix m = Matrix(4, 3, "m");
	cout << m.str() << endl;
	Matrix ma = Matrix(A, 3, 3, "ma");
	cout << ma.display() << endl;
	cout << "--------------------" << endl;
	Matrix mm = Matrix(1, 1, "mm");
	//mm.debug = true;
	mm.dot(&ma, &ma);
	ma.debug = false;
	cout << "MM " << mm.str() << mm.display() << endl;

	Matrix maa = Matrix(AA, 3, 3);
	//maa.debug=true;
	maa = *maa.sub(&mm);
	cout << maa.str() << maa.display() << endl;
	Matrix mi = Matrix(I, 3, 3, "I");

	cout << mi.display() << endl;
	cout << ma.display() << endl;

	//maa.debug=true;
	maa.dot(&ma, &mi);
	cout << "ma*mi " << maa.display() << endl;

	maa.add(&mi);
	cout << maa.display() << endl;

	maa.mul(&mi);
	cout << "mul " << maa.display() << endl;

	Matrix c1 = Matrix(A, 9, 1, "col");
	Matrix c2 = Matrix(A, 1, 9, "row");
	//maa.debug = true;
	maa.dot(&c1, &c2);
	cout << "c1*c2 " << maa.display() << endl;

	maa.dot(&c2, &c1);
	cout << "c1*c2 " << maa.display() << endl;

	Matrix d1 = Matrix(A, 9, 1, "col");
	Matrix d2 = Matrix(C, 1, 5, "row");
	//maa.debug = true;
	maa.dot(&d1, &d2);
	cout << "d1*d2 " << maa.display() << endl;

	try{
		maa.dot(&d2, &d1);
		cout << "d1*d2 " << maa.display() << endl;
	throw new invalid_argument("should not reach this");
	} catch (invalid_argument& e){
		//expected
	}

	Matrix e1 = Matrix(8, 8, "empty");
    Matrix e2 = Matrix(A, 1, 4, "row");
    e1.zero();
	//e1.debug=true;
	e1.add(&e2);
	cout <<"e1+e2 "<< e1.display() <<endl;

	Matrix e3 = Matrix(A, 2, 4, "patch");
	e1.zero();
	e1.add(&e3);
	cout <<"e1+e3 "<< e1.display() <<endl;



	return 0;

}
