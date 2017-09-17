
#include <stb_image.h>
#include "matrix.hpp"
#include <map>
#include <cmath>

using namespace std;

int input_count[] = {1228, 300, 25, 10};
int batch_size = 10;

float one_over_m_[]={1.0/batch_size};
Matrix one_over_m = Matrix(one_over_m_,1,1);

float learning_rate_[]={0.07};
Matrix learning_rate = Matrix(learning_rate_,1,1);

typedef  map<string, Matrix*> matrix_map;

map<string, Matrix*> init_parameters(int* input_count, int layers, int batch_size) {
    map<string, Matrix*> params = map<string, Matrix*>();    
    
    for (int i=0; i< layers; i++){
        Matrix* w = new Matrix(input_count[i+1], input_count[i]);
        //cout << "norm " << std::to_string(sqrt(2.0/input_count[i+1])) <<" -- "<< std::to_string(i) << endl;
        w->randomize(sqrt(2.0/input_count[i+1]), 0.01);
        params.insert(make_pair(string("W").append(to_string(i+1)), w));
        
        Matrix* b = new Matrix(input_count[i+1], 1);
        params.insert(make_pair(string("b").append(to_string(i+1)), b));
        
        Matrix* A = new Matrix(input_count[i+1], batch_size);
        params.insert(make_pair(string("A").append(to_string(i+1)), A));
        
        Matrix* dz = new Matrix(input_count[i+1], batch_size);
        params.insert(make_pair(string("dZ").append(to_string(i+1)), dz));
    }
    
    Matrix* ones = new Matrix(batch_size, 1);
    for(int i=0; i<batch_size; i++){ones->set(i,0,1);}
    params.insert(make_pair("ones", ones));
    
    return params;
 }

 map<string, Matrix*> feedforward(Matrix* X, map<string, Matrix*> params, int layers) {
    map<string, Matrix> activations = map<string, Matrix>();
    
    Matrix* x=X;
    
    for (int i=0; i< layers; i++){
        Matrix* W = params[string("W").append(to_string(i+1))];
        Matrix* b = params[string("b").append(to_string(i+1))];
        Matrix* A = params[string("A").append(to_string(i+1))];
        
        A->dot(*W, *x)->add(*b);
        A->relu();
        x = A;
    }
 }
 
 map<string, Matrix*> backprop(Matrix* X, Matrix* Y, map<string, Matrix*> params, int layers) {
    map<string, Matrix> activations = map<string, Matrix>();
    
    Matrix* x=X;
    Matrix* y=Y;
    Matrix* ones = params["ones"];
    Matrix dZ = Matrix(*Y);
    for (int i=(layers-1); i>0; i--){
        Matrix* W = params[string("W").append(to_string(i))];
        Matrix* b = params[string("b").append(to_string(i))];
        Matrix* A = params[string("A").append(to_string(i))];
        Matrix* Ai = params[string("A").append(to_string(i-1))];
         
        
        if (i == (layers -1)){
            dZ = Matrix(*A);
            dZ.sub(*y);
        } 
        //dW3 = 1./m * np.dot(dZ3, A2.T)
        Matrix dW = Matrix(*W);//copy size
        dW.dot(dZ, Ai->T())->mul(one_over_m);
        
        W->sub(dW.mul(learning_rate));
        
        Matrix db = Matrix(*b);//copy size
        db.dot(dZ, *ones)->mul(one_over_m);
        
        b->sub(db.mul(learning_rate));
        y = A;
        
        if (i>0) {
            Matrix dAi = Matrix(*Ai);
            dAi.dot(W->T(), dZ);
            dAi.relu_grad();
            
            dZ=dAi;
        }
        
    }
 }

void display(map<string, Matrix*> params) {
    for (map<string, Matrix*>::iterator i=params.begin(); i != params.end(); ++i){
        cout << i->first <<   ""<< i->second->str() << endl;
    }
}
int  main(int argc, char** argv){
    map<string, Matrix*> params = init_parameters(input_count, 3, batch_size);
    Matrix* ones = params["ones"];
    
    Matrix X = Matrix(input_count[0], batch_size);
    X.randomize(1.0, 0.0);
    //display(params);
    matrix_map activations = feedforward(&X, params, 3);
    
    Matrix* Y = new Matrix(*params[string("A3")]);
    Y->add(*ones);
    for(int i=0;i<10 ; i++){
        feedforward(&X, params, 3);
        
        Matrix A3 = Matrix(*params[string("A3")]);
        
        A3.sub(*Y);
        A3.dot(A3, *ones);
        cout << "err: "<< A3.str() <<endl;
        backprop(&X, Y, params, 3);
    }
    

}