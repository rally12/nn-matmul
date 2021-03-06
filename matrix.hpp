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
	
    int index(int r, int c){return c + cols*r;}

    public:
    bool debug = false;
    Matrix(int rows, int cols, string name="M"): rows(rows), cols(cols) {
    	len = rows*cols;
        values = new float[len]();
        this->name = name;
    }
    
    /**
     * make sure values length=rows*cols 
     */
    Matrix(float* values2, int rows, int cols, string name="M"): rows(rows), cols(cols) {
    	len = rows*cols;
    	values = new float[len]();
    	this->name = name;
        memcpy(values, values2, rows*cols*sizeof(float));
    }
    
    Matrix(const Matrix &m): rows(m.rows), cols(m.cols) {
    	len = rows*cols;
        values = new float[len];
        name = m.name;
        memcpy(values, m.values, rows*cols*sizeof(float));
    }
    
    Matrix(const Matrix &m, string name): rows(m.rows), cols(m.cols) {
        	len = rows*cols;
            values = new float[len];
            this->name = name;
            memcpy(values, m.values, rows*cols*sizeof(float));
        }

    /**
     * Matmul
     */
    Matrix* dot(Matrix *A, Matrix *B) throw(std::invalid_argument);
    
    /**
     * Broadcast add
     */
    Matrix* add(Matrix *B)throw(std::invalid_argument);
    
    /**
	 * Broadcast add
	 */
	Matrix* add(float x)throw(std::invalid_argument);

    /**
     * Broadcast sub
     */
    Matrix* sub(Matrix *B)throw(std::invalid_argument);
    
    /**
     * Broadcast multiply
     */
    Matrix* mul(Matrix *B)throw(std::invalid_argument);
    
    /**
	 * Broadcast multiply
	 */
	Matrix* mul(float x)throw(std::invalid_argument);

    float at(int r, int c) {
		 if (debug) cout<<" pos="<<to_string(r)<<", "<<to_string(c)<<
		" v="<<to_string(values[c + cols*r])<<endl;
			
        return values[index(r,c)];
    };
    
    Matrix* set(int r, int c, float v) {
        int pos = index(r,c);
        if (debug) cout<<" set="<<to_string(r)<<", "<<to_string(c)<<
        		" to "<<to_string(v)<<endl;
		if (pos >= (len)){
			cout<<" pos="<<to_string(pos)<<" len="<<to_string(rows*cols)<<endl;
			throw new std::invalid_argument(
					"dimensions, out of range." + to_string(pos) + " <> "
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
    
    void zero() { memset(values, 0, sizeof(float)*len);}
    Matrix T();
    
    ~Matrix() {
        if(values != NULL) delete[] values;
        values = NULL;
    }
    
};

#endif //__MATRIX_CLASS
