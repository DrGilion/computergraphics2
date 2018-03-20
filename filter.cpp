#include "filter.h"

#include <vector>
#include <stdexcept>


/*double** Filter::getGaussFilter(int size, double sigma){
    double** gauss= new double*[size];
    for(int i = 0; i<size; ++i){
        gauss[i] = new double[size];
    }

    int half = size/2;

    for (int x = -half; x <= half; x++){
        for (int y = -half; y <= half; y++){
          gauss[x+half][y+half] = pow(M_E,-1*((((x*x)+(y*y)) / (2.0 *sigma*sigma))));
        }
    }

    return gauss;
}*/

Filter::Filter(int size, int** input){
    if(size > 0){
        //initialize data matrix
        matrix_size = size;
        data = std::vector<std::vector<int>>(size,std::vector<int>(size,0));
        for(int i=0 ; i<matrix_size ; ++i){
            //data[i] = std::vector<int>(size,0);
            for(int j=0 ; j<matrix_size ; ++j){
                data[i][j]= input[i][j];
            }
        }
    }else throw std::invalid_argument("Size of the filtermatrix must be greater than 0");
}

Filter::Filter(std::vector<std::vector<int> > input){
    if(input.size() > 0 && (input.size() == input[0].size()) ){
        //initialize data matrix
        matrix_size = input.size();
        data = input;
    }else throw std::invalid_argument("Height and width of the filtermatrix must be equal and bigger than 0");
}

int Filter::size(){
    return matrix_size;
}

int Filter::sum(){
    int s = 0;
    for(int i=0 ; i<matrix_size ; ++i){
        for(int j=0 ; j<matrix_size ; ++j){
            s+= data[i][j];
        }
    }
    return s;
}

void Filter::print(){

}

int Filter::get(int row, int column){
    return data[row][column];
}
