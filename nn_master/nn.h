#ifndef NN_H
#define NN_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <set>
#include <iterator>
#include <algorithm>

#include "nn.h"
using namespace std;
class data_struct{
public:
    data_struct();
    // Training image file name
    const string training_image_fn = "mnist/train-images.idx3-ubyte";

    // Training label file name
    const string training_label_fn = "mnist/train-labels.idx1-ubyte";

    // Weights file name
    const string model_fn = "model-neural-network.dat";

    // Report file name
    const string report_fn = "training-report.dat";

    // Number of training samples
    const int nTraining = 60000;

    // Image size in MNIST database
    const int width = 28;
    const int height = 28;

    int n1; // = 784, without bias neuron
    int n2;
    int n3; // Ten classes: 0 - 9
    const int epochs = 512;
    const double learning_rate = 1e-3;
    const double momentum = 0.9;
    const double epsilon = 1e-3;

    // From layer 1 to layer 2. Or: Input layer - Hidden layer

    //  double *w1[n1 + 1], *delta1[n1 + 1];
    double *out1;

    double **w1,**delta1;
    // From layer 2 to layer 3. Or; Hidden layer - Output layer
 //   double *w2[n2 + 1], *delta2[n2 + 1];
    double *in2, *out2, *theta2;

    double **w2,**delta2;
    // Layer 3 - Output layer
    double *in3, *out3, *theta3;
//    double expected[n3 + 1];
    double *expected;
    // Image. In MNIST: 28x28 gray scale images.
//    int d[width + 1][height + 1];
    int **d;
//    double *train_data[nTraining];
 //   double *train_expected[nTraining];
    double **train_data,**train_expected;
    // File stream to read data (image, label) and write down a report
    ifstream image;
    ifstream label;
    ofstream report;

    //hdgao added
    int sample_per_process;
    int tot_machine;
    int num_machine;
};
class Task:public data_struct
{
public:
    Task();
    Task(int _sample_per_process,int _tot_machine,int _num_machine);
    void about();
    double sigmoid(double x);
    void perceptron();
    double square_error();
    void back_propagation();
    int  learning_process();
    void input();
    void fetch_data();
    void rand_image();
    void get_image(int lucky_index);
    void write_matrix(std::string file_name);
    static void train(void* args);
    static void merge(void* args);

    //add
    double* ResulttoMPI();
    void MPIDecode(double* mix_result);



};
class Task_set{
public:
  Task_set();
  Task_set(std::vector<Task*> dep);
  Task** task_input;
  int size_data;
};
#endif // NN_H
