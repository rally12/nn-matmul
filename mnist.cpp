 
//#include <stb_image.h>
#include "matrix.hpp"
#include <map>
#include <cmath>

using namespace std;

int input_count[] = {1228, 300, 25, 10};
int batch_size = 10;

float one_over_m = 1.0/batch_size;
//Matrix one_over_m = Matrix(one_over_m_,1,1);

float learning_rate=0.00000007;

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
        b->zero();
        params.insert(make_pair(name, b));
        
        cout<<" W ["<< w->get_rows() <<", "<<w->get_cols() <<"]  b["<< b->get_rows() <<", "<<b->get_cols() <<"] "<<endl;
        
		name = string("A").append(to_string(i+1));
		Matrix* A = new Matrix(input_count[i+1], batch_size, name);
		A->zero();
		params.insert(make_pair(name, A));

        
        name = string("dz").append(to_string(i+1));
        Matrix* dz = new Matrix(input_count[i+1], batch_size, name);
        dz->zero();
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
    params.insert(make_pair("A0", x));

    for (int i=0; i< (layers); i++){
        Matrix* W = params[string("W").append(to_string(i+1))];
        Matrix* b = params[string("b").append(to_string(i+1))];
        Matrix* A = params[string("A").append(to_string(i+1))];
        
        //cout <<" ff "<< i << endl << W->display() <<endl;

        cout<< line++ << "  - "<<i<<endl;
        cout<<" W "<< W->str() <<"  x "<< x->str() <<"  b "<< b->str() <<endl;
        
        Matrix Wt = W->T();
        A->dot(W, x);
        A->add(b);
        A->relu();
        cout<<" A"<<i+1<<" ["<< A->get_rows() <<", "<<A->get_cols() <<"]  x["<< x->get_rows() <<", "<<x->get_cols() <<"] "<<endl;
        x = A;
        cout <<" ff "<< i << endl << A->display() <<endl;
    }
 }
 
 void backprop(Matrix* X, Matrix* Y, map<string, Matrix*> params, int layers) {
    map<string, Matrix> activations = map<string, Matrix>();
    
    Matrix* x=X;
    Matrix* y=Y;
    Matrix* ones = params["ones"];
    Matrix* dZ = NULL;//new Matrix(*Y);
    for (int i=(layers); i>0; i--){
        Matrix* W = params[string("W").append(to_string(i))];
        Matrix* b = params[string("b").append(to_string(i))];
        Matrix* A = params[string("A").append(to_string(i))];
        Matrix* Ai = params[string("A").append(to_string(i-1))];
         
        cout<<" bp W ["<< W->get_rows() <<", "<<W->get_cols() <<"]  b["<< b->get_rows() <<", "<<b->get_cols() <<"] "<<endl;
        
        if (i == (layers)){
            dZ = new Matrix(*A, "dZ");
            dZ->sub(y);
        } 
        //dW3 = 1./m * np.dot(dZ3, A2.T)
        Matrix dW = Matrix(*W, "dW");//copy size
        Matrix dAi = Ai->T();

        cout<< dAi.display() <<endl;
        cout<< dZ->display() <<endl;
        dW.dot(dZ, &dAi)->mul(one_over_m);
        cout<< dW.display() <<endl;
        dW.mul(learning_rate);
        W->sub(&dW);
        
        cout<< W->display()<<endl;

        Matrix db = Matrix(*b);//copy size
        db.dot(dZ, ones);
        db.mul(one_over_m);
        db.mul(learning_rate);
        b->sub(&db);
        y = A;
        

        if (i>0) {
            Matrix* dAi = new Matrix(*Ai,"dZ");
            Matrix Wt = W->T();
            dAi->dot(&Wt, dZ);
            dAi->relu_grad();
            delete dZ;
            dZ=dAi;
        }
    }
    delete dZ;
 }

void display(map<string, Matrix*> params) {
    for (map<string, Matrix*>::iterator i=params.begin(); i != params.end(); ++i){
        cout << i->first <<   " " << i->second->str() << endl;
    }
}
int  main(int argc, char** argv){
    map<string, Matrix*> params = init_parameters(input_count, 3, batch_size);
    Matrix* ones = params["ones"];
    
    Matrix X = Matrix(input_count[0], batch_size);
    X.randomize(0.01, 0.03);
    display(params);
    feedforward(&X, params, 3);
    display(params);
    cout<< "end ff m"<< endl; //<<params[string("A3")]<<endl; 
    Matrix Y = Matrix(input_count[3], batch_size);
    Y.randomize(0.01, 0.03);
    
    cout<< "end ff m2"<<endl; 
    //Y->add(ones);
    for(int i=0;i<2 ; i++){
        cout<< "batch: "<<i<<endl;
        feedforward(&X, params, 3);
        
        Matrix A3 = Matrix(*params[string("A3")]);
        //cout<< "batch: "<<i<<endl<<A3.display()<<endl;
        A3.sub(&Y);
        Matrix err = Matrix(*ones,"err");
        err.dot(&A3, ones);
        cout << "err: "<< err.display() <<endl;
        backprop(&X, &Y, params, 3);
    }
    
    params.erase("A0");
    for (map<string, Matrix*>::iterator it= params.begin(); it != params.end(); ++it){
    	Matrix *m = it->second;
    	params.erase(it);
    	delete m;
    }

}
