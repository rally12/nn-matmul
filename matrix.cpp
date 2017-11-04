
#include "matrix.hpp"
#include <iostream>
#include <stdexcept>
#include <random>

using namespace std;
    /**
     * Matmul
     */
    Matrix* Matrix::dot(Matrix *A, Matrix *B) throw(std::invalid_argument) {
        
    	if(this == A || this == B) throw std::invalid_argument("Dot doas not work with self. "+A->str()+" <dot> "+B->str());


        if(A->cols != B->rows) throw std::invalid_argument("dimensions, don't match dot a b. "+A->str()+" <dot> "+B->str());
        //if(cols != B->cols) throw std::invalid_argument("dimensions, don't match cols. "+A->str()+" <dot> "+B->str());
        //if(A->rows != rows) throw std::invalid_argument("dimensions, don't match rows. "+A->str()+" <dot> "+B->str());
		if (values != NULL) delete[] values;
		this->cols = B->cols;
		this->rows = A->rows;
		len = rows*cols;
		values = new float[len]();
        len = rows * cols;
		float cell=0;
		int square = A->cols;
		int myrow=0;
        for (int c=0; c < B->cols; c++) {
        	myrow=0;
            for (int r=0;  r < A->len; r+=A->cols) {
                cell=0;
                for ( int i = r, j=c; j < B->len; i++, j+=B->cols) {
                	if (debug) cout<<" pos="<<to_string(i)<<", "<<to_string(j)<<
                			" v1="<<to_string(A->values[i])<<
                			" v2="<<to_string(B->values[j])<<endl;
                    cell += A->values[i] * B->values[j];
                }
                this->values[c+myrow]=cell;
                myrow+=cols;
            }

        }
        return this;
    }
    
    /**
     * Broadcast add
     */
    Matrix* Matrix::add(Matrix *B) throw(std::invalid_argument){
        if (0 != (cols % B->cols) || 0 != (rows % B->rows)) throw std::invalid_argument("dimensions, don't match."+str()+" <> "+B->str());
        float cell=0;
        for (int c=0; c< cols; c++) {
            int bc = c % B->cols;
            for (int r=0;  r < rows; r++) {
                int br = r % B->rows;
                cell=at(r,c) + B->at(br, bc);
                
                set(r, c, cell);
            }
        }
        return this;
    }
    
    /**
   	 * Broadcast add
   	 */
   	Matrix* Matrix::add(float x) throw(std::invalid_argument){
   		for (int i=0; i< len; i++) {
   			values[i]= values[i] + x;
   		}
   		return this;
   	}

    /**
     * Broadcast sub
     */
    Matrix* Matrix::sub(Matrix *B) throw(std::invalid_argument){
        if (0 != (cols % B->cols) || 0 != (rows % B->rows)) throw std::invalid_argument("dimensions, don't match."+str()+" <> "+B->str());
        float cell=0;
        for (int c=0; c< cols; c++) {
            int bc = c % B->cols;
            for (int r=0;  r < rows; r++) {
                int br = r % B->rows;
                cell=at(r, c) - B->at(br, bc);
                set(r, c, cell);
            }
        }
        return this;
    }
    
    /**
     * Broadcast multiply
     */
    Matrix* Matrix::mul(Matrix *B) throw(std::invalid_argument){
        if (0 != (cols % B->cols) || 0 != (rows % B->rows)) throw std::invalid_argument("dimensions, don't match. "+str()+" <> "+B->str());
        float cell=0;
        for (int c=0; c< cols; c++) {
            int bc = c % B->cols;
            for (int r=0;  r < rows; r++) {
                int br = r % B->rows;
                cell=at(r, c) * B->at(br, bc);
                set(r, c, cell);
            }
        }
        return this;
    }
    
    /**
	 * Broadcast multiply
	 */
	Matrix* Matrix::mul(float x) throw(std::invalid_argument){
		for (int i=0; i< len; i++) {
			values[i]= values[i] * x;
		}
		return this;
	}

    const std::string Matrix::display() {
        std::string ret = string("");
        ret.append(" ").append(name).append("(");
        ret.append(to_string(cols));
        ret.append(", ");
        ret.append(to_string(rows));
        ret.append(")\n");
        for (int i=0; i < len; i++) {
            if ( i%cols==0){
            	if(i==0) {
            		ret.append("[");
            	} else {
            		ret.append(",\n ");
            	}
            }

            	if (i%cols > 0) ret.append(",");
                string val = to_string(values[i]);
                ret.append(" ").append(val);


        }
        ret.append("]");
        return ret;
    }
    
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(1, 2);

    void Matrix::randomize(float norm, float offset){
    	std::uniform_real_distribution<> dis(0, norm);
        for (int i=0; i< len; i++) {
            values[i] = dis(gen) + offset;
        }
    }
    
    Matrix* Matrix::relu() {
        int e = cols*rows;
        for (int i=0; i< e; i++) {
            values[i] = max(0.0, (double)values[i]);
        }
        return this;
    }
    
    Matrix Matrix::relu_grad() {
        Matrix ret = Matrix(rows, cols);
        int e = cols*rows;
        for (int i=0; i< e; i++) {
            ret.values[i] = (values[i]>0)?0:1;
        }
        return ret;
    }
    
    Matrix Matrix::softmax() {
        Matrix ret = Matrix(rows, cols);
        int e = cols*rows;
        float ex[cols];
        for (int c=0; c<cols; c++){
            for (int r=0; r< rows; r++) {
                float ei = std::exp(at(r,c));
                ex[c] +=ei;
                ret.set(r,c,ei);
            }
            for (int r=0; r< rows; r++) {
                ret.set(r,c,  at(r,c) / ex[c]);
            }
        }
        return ret;
    }
    
    Matrix Matrix::T() {
        Matrix ret = Matrix(cols, rows);
        for (int c=0; c<cols; c++){
            for (int r=0; r< rows; r++) {
                ret.set(c,r,at(r,c));
            }
        }
        ret.name=name+".T";
        return ret;
    }
