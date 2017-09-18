#include <cstddef>
#include <string>

#ifndef __MATRIX_CLASS
#define __MATRIX_CLASS
#include <iostream>
#include <cstring>
#include <cmath>
using namespace std;

class Matrix {
    
    protected:
    
    float* values = NULL;
    int rows=0, cols=0;
    
    
    public:
    
    Matrix(int rows, int cols): rows(rows), cols(cols) {
        values = new float[rows*cols];
    }
    
    /**
     * make sure values length=rows*cols 
     */
    Matrix(float* values2, int rows, int cols): rows(rows), cols(cols) {
        values = new float[rows*cols];
        memcpy(values, values2, rows*cols*sizeof(float));
    }
    
    Matrix(const Matrix &m): rows(m.rows), cols(m.cols) {
        values = new float[rows*cols];
        memcpy(values, m.values, rows*cols*sizeof(float));
    }
    
    /**
     * Matmul
     */
    Matrix* dot(Matrix A, Matrix B) throw(std::string);
    
    /**
     * Broadcast add
     */
    Matrix add(Matrix B)throw(std::string);
    
    /**
     * Broadcast sub
     */
    Matrix sub(Matrix B)throw(std::string);
    
    /**
     * Broadcast multiply
     */
    Matrix mul(Matrix B)throw(std::string);
    
    float at(int r, int c) {
        return values[r + rows*c];
    };
    
    Matrix set(int r, int c, float v) {
        int pos = r + rows*c;
        values[pos] = v;
        return *this;
    };
    
    const std::string str();
    
    int get_rows(){return rows;}
    int get_cols(){return cols;}
    
    void randomize(float , float);
    Matrix relu();
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