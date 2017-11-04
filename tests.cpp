
//#include <stb_image.h>
#include <iostream>
#include "matrix.hpp"

using namespace std;

    float A[] = { 1, 2, 3, 
                  4, 5, 6, 
                  7, 8, 9};
                 
    
    float B[] = {2, 2, 2, 
                 2, 2, 2, 
                 2, 2, 2};

    float C[] = {2, 2, 2, 3, 2};
                 
    
    float I[] = { 1, 0, 0, 
                  0, 1, 0, 
                  0, 0, 1};

	float AA[] = {
	30,    36,    42,
    66,    81,    96,
   102,   126,   150};

 int  main(int argc, char** argv){
    int width, height, bpp;
    
   //stbi_uc* pixels = stbi_load("./", &width, &height, &bpp, 0);
   
   Matrix m = Matrix(4,3,"m");
   cout << m.str() <<endl;   
   Matrix ma = Matrix(A, 3, 3, "ma");
   cout << ma.display() <<endl;
   cout<<"--------------------"<<endl;
   Matrix mm = Matrix(1,1, "mm");
   //mm.debug = true;
   mm.dot(&ma,&ma);
   ma.debug=false;
   cout <<"MM "<< mm.str() << mm.display() <<endl;

   Matrix maa = Matrix(AA, 3, 3);
   //maa.debug=true;
   maa = *maa.sub(&mm);
   cout << maa.str() << maa.display() <<endl;
   Matrix mi = Matrix(I, 3, 3,"I");
   
   cout << mi.display() <<endl;
   cout << ma.display() <<endl;

   //maa.debug=true;
   maa.dot(&ma, &mi);
   cout <<"ma*mi "<< maa.display() <<endl;
   
   maa.add(&mi);
   cout << maa.display() <<endl;
   
   maa.mul(&mi);
   cout <<"mul "<< maa.display() <<endl;
   
   
   Matrix c1 = Matrix(A, 9, 1, "col");
   Matrix c2 = Matrix(A, 1, 9, "row");
   maa.debug=true;
   maa.dot(&c1, &c2);
   cout <<"c1*c2 "<< maa.display() <<endl;

   maa.dot(&c2, &c1);
   cout <<"c1*c2 "<< maa.display() <<endl;


   return 0;
    
}
