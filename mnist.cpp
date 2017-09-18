 
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
        string name = string("W").append(to_string(i+1));
        Matrix* w = new Matrix(input_count[i+1], input_count[i], name);
        //cout << "norm " << std::to_string(sqrt(2.0/input_count[i+1])) <<" -- "<< std::to_string(i) << endl;
        w->randomize(sqrt(2.0/input_count[i+1]), 0.01);
        params.insert(make_pair(string("W").append(to_string(i+1)), w));
        
        name = string("b").append(to_string(i+1));
        Matrix* b = new Matrix(input_count[i+1], 1, name);
        params.insert(make_pair(name, b));
        
        cout<<" W ["<< w->get_rows() <<", "<<w->get_cols() <<"]  b["<< b->get_rows() <<", "<<b->get_cols() <<"] "<<endl;
        
        name = string("A").append(to_string(i+1));
        Matrix* A = new Matrix(input_count[i+1], batch_size, name);
        params.insert(make_pair(name, A));
        
        name = string("dz").append(to_string(i+1));
        Matrix* dz = new Matrix(input_count[i+1], batch_size, name);
        params.insert(make_pair(name, dz));
    }
    
    Matrix* ones = new Matrix(batch_size, 1, "ones");
    for(int i=0; i<batch_size; i++){ones->set(i,0,1);}
    params.insert(make_pair("ones", ones));
    
    return params;
 }

 void feedforward(Matrix* X, map<string, Matrix*> &params, int layers) {
    map<string, Matrix> activations = map<string, Matrix>();
    int line =0;
    cout<< line++ <<endl;
    Matrix* x=X;
    cout<< line++ <<endl;
    
    for (int i=0; i< (layers); i++){
        Matrix* W = params[string("W").append(to_string(i+1))];
        Matrix* b = params[string("b").append(to_string(i+1))];
        Matrix* A = params[string("A").append(to_string(i+1))];
        
        cout<< line++ << "  - "<<i<<endl;
        cout<<" W ["<< W->get_rows() <<", "<<W->get_cols() <<"]  b["<< b->get_rows() <<", "<<b->get_cols() <<"] "<<endl;
        
        
        A->dot(*W, *x);
        A->add(*b);
        //A->relu();
        cout<<" A"<<i+1<<" ["<< A->get_rows() <<", "<<A->get_cols() <<"]  x["<< x->get_rows() <<", "<<x->get_cols() <<"] "<<endl;
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
         
        cout<<" bp W ["<< W->get_rows() <<", "<<W->get_cols() <<"]  b["<< b->get_rows() <<", "<<b->get_cols() <<"] "<<endl;
        
        if (i == (layers -1)){
            dZ = Matrix(*A);
            dZ.sub(*y);
        } 
        //dW3 = 1./m * np.dot(dZ3, A2.T)
        Matrix dW = Matrix(*W);//copy size
        Matrix Ait = Ai->T();
        dW.dot(dZ, Ait)->mul(one_over_m);
        
        dW.mul(learning_rate);
        W->sub(dW);
        
        Matrix db = Matrix(*b);//copy size
        db.dot(dZ, *ones);
        db.mul(one_over_m);
        db.mul(learning_rate);
        b->sub(db);
        y = A;
        
        if (i>0) {
            Matrix dAi = Matrix(*Ai);
            Matrix Wt = W->T();
            dAi.dot(Wt, dZ);
            dAi.relu_grad();
            
            dZ=dAi;
        }
        
    }
 }

void display(map<string, Matrix*> params) {
    for (map<string, Matrix*>::iterator i=params.begin(); i != params.end(); ++i){
        cout << i->first <<   ""<< endl ;//i->second->get_cols() << endl;
    }
}
int  main(int argc, char** argv){
    map<string, Matrix*> params = init_parameters(input_count, 3, batch_size);
    Matrix* ones = params["ones"];
    
    Matrix X = Matrix(input_count[0], batch_size);
    X.randomize(1.0, 0.0);
    display(params);
    feedforward(&X, params, 3);
    display(params);
    cout<< "end ff m"<< endl; //<<params[string("A3")]<<endl; 
    Matrix* Y = new Matrix(*params[string("A3")]);
    
    cout<< "end ff m2"<<endl; 
    Y->add(*ones);
    for(int i=0;i<10 ; i++){
        cout<< "batch: "<<i<<endl;
        feedforward(&X, params, 3);
        
        Matrix A3 = Matrix(*params[string("A3")]);
        cout<< "batch: "<<i<<endl;
        A3.sub(*Y);
        A3.dot(A3, *ones);
        cout << "err: "<< A3.str() <<endl;
        backprop(&X, Y, params, 3);
    }
    

}