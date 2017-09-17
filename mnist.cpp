
#include <stb_image.h>
#include "matrix.hpp"
#include <map>
#include <cmath>

using namespace std;

int input_count[] = {1228, 300, 25, 10};

map<string, Matrix*> init_parameters(int* input_count, int layers) {
    map<string, Matrix*> params = map<string, Matrix*>();    
    
    for (int i=0; i< layers; i++){
        Matrix* w = new Matrix(input_count[i+1], input_count[i]);
        Matrix* b = new Matrix(input_count[i+1], 1);
        cout << "norm " << std::to_string(sqrt(2.0/input_count[i+1])) <<" -- "<< std::to_string(i) << endl;
        w->randomize(sqrt(2.0/input_count[i+1]), 0.01);
        params.insert(make_pair(string("W").append(to_string(i+1)), w));
        params.insert(make_pair(string("b").append(to_string(i+1)), b));
    }
    return params;
 }

 map<string, Matrix*> feedforward(Matrix X, map<string, Matrix*> parameters) {
    map<string, Matrix> gradients = map<string, Matrix>();    
    
     
 }



 int  main(int argc, char** argv){
    map<string, Matrix*> params = init_parameters(input_count, 3);
    
    for (map<string, Matrix*>::iterator i=params.begin(); i != params.end(); ++i){
        cout << i->first <<   ""<< i->second->str() << endl;
    }
    
    
}