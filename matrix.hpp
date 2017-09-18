#include <cstddef>
#include <string>
#include <iostream>
#include <cstring>
#include <cmath>

#ifndef __MATRIX_CLASS
#define __MATRIX_CLASS

using namespace std;

class Matrix {
    
    protected:
    
    float* values = NULL;
    int rows=0, cols=0;
    string name="M";
    int len = 0;
    
    public:
    
    Matrix(int rows, int cols, string name="M"): rows(rows), cols(cols) {
    	len = rows*cols;
        values = new float[len];
    }
    
    /**
     * make sure values length=rows*cols 
     */
    Matrix(float* values2, int rows, int cols): rows(rows), cols(cols) {
    	len = rows*cols;
    	values = new float[len];
        memcpy(values, values2, rows*cols*sizeof(float));
    }
    
    Matrix(const Matrix &m): rows(m.rows), cols(m.cols) {
    	len = rows*cols;
        values = new float[len];
        name = m.name;
        memcpy(values, m.values, rows*cols*sizeof(float));
    }
    
    /**
     * Matmul
     */
    Matrix* dot(Matrix *A, Matrix *B) throw(std::string);
    
    /**
     * Broadcast add
     */
    Matrix* add(Matrix *B)throw(std::string);
    
    /**
     * Broadcast sub
     */
    Matrix* sub(Matrix *B)throw(std::string);
    
    /**
     * Broadcast multiply
     */
    Matrix* mul(Matrix *B)throw(std::string);
    
    float at(int r, int c) {
        return values[r + rows*c];
    };
    
    Matrix* set(int r, int c, float v) {
        int pos = r + rows*c;

		if (pos >= (len)){
			cout<<" pos="<<to_string(pos)<<" len="<<to_string(rows*cols)<<endl;
			throw new std::invalid_argument(
					"dimensions, don't match." + to_string(pos) + " <> "
							+ to_string(len));
		}
        values[pos] = v;
        return this;
    };
    
    const std::string display();
    const std::string str(){return "M("+name+")["+to_string(rows)+", "+to_string(cols)+"] ";};
    
    int get_rows(){return rows;}
    int get_cols(){return cols;}
    
    void randomize(float , float);
    Matrix* relu();
    Matrix relu_grad();
    Matrix softmax();
    
    Matrix zero() { std::memset(values, 0, sizeof(float)*cols*rows);}
    Matrix T();
    
    ~Matrix() {
        if(values != NULL) delete values;
        values = NULL;
    }
    
};

#endif //__MATRIX_CLASS
