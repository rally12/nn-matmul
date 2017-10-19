
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
                 
    
    float I[] = { 1, 0, 0, 
                  0, 1, 0, 
                  0, 0, 1};

	float AA[] = {   30,    36,    42,
    66,    81,    96,
   102,   126,   150};

 int  main(int argc, char** argv){
    int width, height, bpp;
    
   //stbi_uc* pixels = stbi_load("./", &width, &height, &bpp, 0);
   
   Matrix m = Matrix(4,3);   
   cout << m.str() <<endl;   
   Matrix ma = Matrix(A, 3, 3);   
   cout << ma.str() <<endl;
   cout<<"--------------------"<<endl;
   Matrix mm = Matrix(1,1);
   ma.debug = true;
   mm.dot(&ma,&ma);
   ma.debug=false;
   cout <<"MM "<< mm.str() << mm.display() <<endl;

   Matrix maa = Matrix(AA, 3, 3);
   maa.debug=true;
   maa = *maa.sub(&mm);
   cout << maa.str() << maa.display() <<endl;
   Matrix mi = Matrix(I, 3, 3);   
   cout << mi.str() <<endl;
   
   ma.dot(&ma, &mi);   
   cout << ma.str() <<endl;
   
   ma.add(&mi);   
   cout << ma.str() <<endl;
   
   ma.mul(&mi);   
   cout <<"mul "<< ma.str() <<endl;
   
   
   return 0;
    
}
