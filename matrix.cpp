
#include "matrix.hpp"
#include <iostream>

using namespace std;
    /**
     * Matmul
     */
    Matrix Matrix::dot( Matrix B) throw(std::string) {
        Matrix ret = Matrix( cols, B.rows);
        if(cols != B.rows) throw "dimensions, don't match.";
        float cell=0;
        for (int c=0; c< cols; c++) {
            for (int r=0;  r < B.rows; r++) {
                cell=0;
                for ( int i = 0; i<cols; i++) {
                    cell += at(r, i) * B.at(i, c);
                }
                ret.set(c,r, cell);
            }
        }
        return ret;
    }
    
    /**
     * Broadcast add
     */
    Matrix Matrix::add(Matrix B) throw(std::string){
        if (0 != (cols % B.cols) || 0 != (rows % B.rows)) throw "dimensions, don't match.";
        float cell=0;
        for (int c=0; c< cols; c++) {
            int bc = c % B.cols;
            for (int r=0;  r < rows; r++) {
                int br = r % B.rows;
                cell=at(c,r) + B.at(bc, br);
                set(c,r, cell);
            }
        }
        return *this;
    }
    
    /**
     * Broadcast multiply
     */
    Matrix Matrix::mul(Matrix B) throw(std::string){
        if (0 != (cols % B.cols) || 0 != (rows % B.rows)) throw "dimensions, don't match.";
        float cell=0;
        for (int c=0; c< cols; c++) {
            int bc = c % B.cols;
            for (int r=0;  r < rows; r++) {
                int br = r % B.rows;
                cell=at(c,r) * B.at(bc, br);
                set(c,r, cell);
            }
        }
        return *this;
    }
    
    const std::string Matrix::str() {
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
    
    void Matrix::randomize(float norm, float offset){
        int e = cols*rows;
        for (int i=0; i< e; i++) {
            values[i] = offset + (std::rand()*norm)/RAND_MAX;
        }
    }