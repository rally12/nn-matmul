
#include "matrix.hpp"
#include <iostream>
#include <stdexcept>
#include <random>

using namespace std;
    /**
     * Matmul
     */
    Matrix* Matrix::dot(Matrix *A, Matrix *B) throw(std::string) {
        
        if(A->cols != B->rows) throw std::invalid_argument("dimensions, don't match dot a b. "+A->str()+" <dot> "+B->str());
        if(cols != B->cols) throw std::invalid_argument("dimensions, don't match cols. "+A->str()+" <dot> "+B->str());
        if(A->rows != rows) throw std::invalid_argument("dimensions, don't match rows. "+A->str()+" <dot> "+B->str());
        float cell=0;
        for (int c=0; c< cols; c++) {
            for (int r=0;  r < rows; r++) {
                cell=0;
                for ( int i = 0; i<cols; i++) {
                    cell += A->at(r, i) * B->at(i, c);
                }
                set(r, c, cell);
            }
        }
        return this;
    }
    
    /**
     * Broadcast add
     */
    Matrix* Matrix::add(Matrix *B) throw(std::string){
        if (0 != (cols % B->cols) || 0 != (rows % B->rows)) throw std::invalid_argument("dimensions, don't match."+str()+" <> "+B->str());
        float cell=0;
        for (int c=0; c< cols; c++) {
            int bc = c % B->cols;
            for (int r=0;  r < rows; r++) {
                int br = r % B->rows;
                cell=at(r,c) + B->at(r, c);
                
                set(r, c, cell);
            }
        }
        return this;
    }
    
    /**
     * Broadcast sub
     */
    Matrix* Matrix::sub(Matrix *B) throw(std::string){
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
    Matrix* Matrix::mul(Matrix *B) throw(std::string){
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
    
    const std::string Matrix::display() {
        std::string ret = string("");
        ret.append(" M(");
        ret.append(to_string(cols));
        ret.append(", ");
        ret.append(to_string(rows));
        ret.append(")\n");
        for (int r=0; r < rows; r++) {
            if (r > 0) ret.append(",\n");
            ret.append("[");
            for (int c=0; c < cols; c++) {
                if (c > 0) ret.append(",");
                string val = to_string(at(c, r));
                ret.append(" ").append(val);
            }
            ret.append(" ]");
        }
        return ret;
    }
    
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(1, 2);

    void Matrix::randomize(float norm, float offset){
    	std::uniform_real_distribution<> dis(0, norm);
        for (int i=0; i< len; i++) {
            values[i] = dis(gen) + offset;
            cout<< values[i]<<endl;
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
        return ret;
    }
