#ifndef FILTER_H
#define FILTER_H

#include <vector>

class Filter{
public:
    Filter(int size, int** input);
    Filter(std::vector<std::vector<int>> input);
    int size();
    int sum();
    void print();
    int get(int row, int column);

    /*static Filter* SobelHorizontal(){
        const int filtersize = 3;

        int** sobelX = new int*[filtersize];
        for(int i = 0; i< filtersize ; i++){
            sobelX[i] = new int[filtersize];
        }
        sobelX[0][0] = -1;
        sobelX[0][1] = 0;
        sobelX[0][2] = 1;
        sobelX[1][0] = -2;
        sobelX[1][1] = 0;
        sobelX[1][2] = 2;
        sobelX[2][0] = -1;
        sobelX[2][1] = 0;
        sobelX[2][2] = 1;

        Filter* SobelX = new Filter(sobelX,filtersize,filtersize);
        return SobelX;
    }

    static Filter* SobelVertical(){
        const int filtersize = 3;

        int** sobelY = new int*[filtersize];
        for(int i = 0; i< filtersize ; i++){
            sobelY[i] = new int[filtersize];
        }
        sobelY[0][0] = -1;
        sobelY[0][1] = -2;
        sobelY[0][2] = -1;
        sobelY[1][0] = 0;
        sobelY[1][1] = 0;
        sobelY[1][2] = 0;
        sobelY[2][0] = 1;
        sobelY[2][1] = 2;
        sobelY[2][2] = 1;

        Filter* SobelY = new Filter(sobelY,filtersize,filtersize);
        return SobelY;
    }

    static Filter* Gauss3(){
        const int filtersize = 3;

        int** gauss = new int*[filtersize];
        for(int i = 0; i< filtersize ; i++){
            gauss[i] = new int[filtersize];
        }
        gauss[0][0] = 1;
        gauss[0][1] = 2;
        gauss[0][2] = 1;
        gauss[1][0] = 2;
        gauss[1][1] = 4;
        gauss[1][2] = 2;
        gauss[2][0] = 1;
        gauss[2][1] = 2;
        gauss[2][2] = 1;

        Filter* Gauss = new Filter(gauss,filtersize,filtersize);
        return Gauss;
    }

    static Filter* LaPlace(){
        const int filtersize = 3;

        int** laplace = new int*[filtersize];
        for(int i = 0; i< filtersize ; i++){
            laplace[i] = new int[filtersize];
        }
        laplace[0][0] = 0;
        laplace[0][1] = 1;
        laplace[0][2] = 0;
        laplace[1][0] = 1;
        laplace[1][1] = -4;
        laplace[1][2] = 1;
        laplace[2][0] = 0;
        laplace[2][1] = 1;
        laplace[2][2] = 0;

        Filter* LaPlace = new Filter(laplace,filtersize,filtersize);
        return LaPlace;
    }

    static double** getGaussFilter(int size,double sigma);*/
private:
    int matrix_size;
    std::vector<std::vector<int>> data;
};

#endif // FILTER_H
