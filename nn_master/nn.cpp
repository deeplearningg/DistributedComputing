#include "nn.h"
data_struct::data_struct(){

    n1 = width*height;
    n2 = 128;
    n3 = 10;
    w1 = new double*[n1+1];
    delta1 = new double*[n1+1];

    w2 = new double*[n2+1];
    delta2 = new double*[n2+1];

    expected = new double[n3+1];

    d = new int*[width+1];
    for(int i = 0;i < width +1;i++)
        d[i] = new int[height+1];

    train_data = new double*[nTraining];
    train_expected = new double*[nTraining];

}
Task::Task()
{
    // Layer 1 - Layer 2 = Input layer - Hidden layer
    for (int i = 1; i <= n1; ++i) {
        w1[i] = new double [n2 + 1];
        delta1[i] = new double [n2 + 1];
    }

    out1 = new double [n1 + 1];

    // Layer 2 - Layer 3 = Hidden layer - Output layer
    for (int i = 1; i <= n2; ++i) {
        w2[i] = new double [n3 + 1];
        delta2[i] = new double [n3 + 1];
    }

    in2 = new double [n2 + 1];
    out2 = new double [n2 + 1];
    theta2 = new double [n2 + 1];

    // Layer 3 - Output layer
    in3 = new double [n3 + 1];
    out3 = new double [n3 + 1];
    theta3 = new double [n3 + 1];

    // Initialization for weights from Input layer to Hidden layer
    for (int i = 1; i <= n1; ++i) {
        for (int j = 1; j <= n2; ++j) {
            int sign = rand() % 2;

            // Another strategy to randomize the weights - quite good
            // w1[i][j] = (double)(rand() % 10 + 1) / (10 * n2);

            w1[i][j] = (double)(rand() % 6) / 10.0;
            if (sign == 1) {
                w1[i][j] = - w1[i][j];
            }
        }
    }

    // Initialization for weights from Hidden layer to Output layer
    for (int i = 1; i <= n2; ++i) {
        for (int j = 1; j <= n3; ++j) {
            int sign = rand() % 2;

            // Another strategy to randomize the weights - quite good
            // w2[i][j] = (double)(rand() % 6) / 10.0;

            w2[i][j] = (double)(rand() % 10 + 1) / (10.0 * n3);
            if (sign == 1) {
                w2[i][j] = - w2[i][j];
            }
        }
    }
    //
    report.open(report_fn.c_str(), ios::out);
    image.open(training_image_fn.c_str(), ios::in | ios::binary); // Binary image file
    label.open(training_label_fn.c_str(), ios::in | ios::binary ); // Binary label file

    // Reading file headers
    char number;
    for (int i = 1; i <= 16; ++i) {
        image.read(&number, sizeof(char));
    }
    for (int i = 1; i <= 8; ++i) {
        label.read(&number, sizeof(char));
    }

    // Neural Network Initialization
    fetch_data();

}
Task::Task(int _sample_per_process,int _tot_machine,int _num_machine){

    {
        // Layer 1 - Layer 2 = Input layer - Hidden layer
        for (int i = 1; i <= n1; ++i) {
            w1[i] = new double [n2 + 1];
            delta1[i] = new double [n2 + 1];
        }

        out1 = new double [n1 + 1];

        // Layer 2 - Layer 3 = Hidden layer - Output layer
        for (int i = 1; i <= n2; ++i) {
            w2[i] = new double [n3 + 1];
            delta2[i] = new double [n3 + 1];
        }

        in2 = new double [n2 + 1];
        out2 = new double [n2 + 1];
        theta2 = new double [n2 + 1];

        // Layer 3 - Output layer
        in3 = new double [n3 + 1];
        out3 = new double [n3 + 1];
        theta3 = new double [n3 + 1];

        // Initialization for weights from Input layer to Hidden layer
        for (int i = 1; i <= n1; ++i) {
            for (int j = 1; j <= n2; ++j) {
                int sign = rand() % 2;

                // Another strategy to randomize the weights - quite good
                // w1[i][j] = (double)(rand() % 10 + 1) / (10 * n2);

                w1[i][j] = (double)(rand() % 6) / 10.0;
                if (sign == 1) {
                    w1[i][j] = - w1[i][j];
                }
            }
        }

        // Initialization for weights from Hidden layer to Output layer
        for (int i = 1; i <= n2; ++i) {
            for (int j = 1; j <= n3; ++j) {
                int sign = rand() % 2;

                // Another strategy to randomize the weights - quite good
                // w2[i][j] = (double)(rand() % 6) / 10.0;

                w2[i][j] = (double)(rand() % 10 + 1) / (10.0 * n3);
                if (sign == 1) {
                    w2[i][j] = - w2[i][j];
                }
            }
        }
        //
        report.open(report_fn.c_str(), ios::out);
        image.open(training_image_fn.c_str(), ios::in | ios::binary); // Binary image file
        label.open(training_label_fn.c_str(), ios::in | ios::binary ); // Binary label file

        // Reading file headers
        char number;
        for (int i = 1; i <= 16; ++i) {
            image.read(&number, sizeof(char));
        }
        for (int i = 1; i <= 8; ++i) {
            label.read(&number, sizeof(char));
        }

        // Neural Network Initialization
        fetch_data();

    }

    sample_per_process = _sample_per_process;
    tot_machine = _tot_machine;
    num_machine = _num_machine;
}
void Task::about(){
    // Details
    cout << "**************************************************" << endl;
    cout << "*** Training Neural Network for MNIST database ***" << endl;
    cout << "**************************************************" << endl;
    cout << endl;
    cout << "No. input neurons: " << n1 << endl;
    cout << "No. hidden neurons: " << n2 << endl;
    cout << "No. output neurons: " << n3 << endl;
    cout << endl;
    cout << "No. iterations: " << epochs << endl;
    cout << "Learning rate: " << learning_rate << endl;
    cout << "Momentum: " << momentum << endl;
    cout << "Epsilon: " << epsilon << endl;
    cout << endl;
    cout << "Training image data: " << training_image_fn << endl;
    cout << "Training label data: " << training_label_fn << endl;
    cout << "No. training sample: " << nTraining << endl << endl;
}
double Task::sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}
void Task::perceptron() {
    for (int i = 1; i <= n2; ++i) {
        in2[i] = 0.0;
    }

    for (int i = 1; i <= n3; ++i) {
        in3[i] = 0.0;
    }

    for (int i = 1; i <= n1; ++i) {
        for (int j = 1; j <= n2; ++j) {
            in2[j] += out1[i] * w1[i][j];
        }
    }

    for (int i = 1; i <= n2; ++i) {
        out2[i] = sigmoid(in2[i]);
    }

    for (int i = 1; i <= n2; ++i) {
        for (int j = 1; j <= n3; ++j) {
            in3[j] += out2[i] * w2[i][j];
        }
    }

    for (int i = 1; i <= n3; ++i) {
        out3[i] = sigmoid(in3[i]);
    }
}
double Task::square_error(){
    double res = 0.0;
    for (int i = 1; i <= n3; ++i) {
        res += (out3[i] - expected[i]) * (out3[i] - expected[i]);
    }
    res *= 0.5;
    return res;
}
void Task::back_propagation() {
    double sum;

    for (int i = 1; i <= n3; ++i) {
        theta3[i] = out3[i] * (1 - out3[i]) * (expected[i] - out3[i]);
    }

    for (int i = 1; i <= n2; ++i) {
        sum = 0.0;
        for (int j = 1; j <= n3; ++j) {
            sum += w2[i][j] * theta3[j];
        }
        theta2[i] = out2[i] * (1 - out2[i]) * sum;
    }

    for (int i = 1; i <= n2; ++i) {
        for (int j = 1; j <= n3; ++j) {
            delta2[i][j] = (learning_rate * theta3[j] * out2[i]) + (momentum * delta2[i][j]);
            w2[i][j] += delta2[i][j];
        }
    }

    for (int i = 1; i <= n1; ++i) {
        for (int j = 1 ; j <= n2 ; j++ ) {
            delta1[i][j] = (learning_rate * theta2[j] * out1[i]) + (momentum * delta1[i][j]);
            w1[i][j] += delta1[i][j];
        }
    }
}
int Task::learning_process() {
    for (int i = 1; i <= n1; ++i) {
        for (int j = 1; j <= n2; ++j) {
            delta1[i][j] = 0.0;
        }
    }

    for (int i = 1; i <= n2; ++i) {
        for (int j = 1; j <= n3; ++j) {
            delta2[i][j] = 0.0;
        }
    }

    for (int i = 1; i <= epochs; ++i) {
        perceptron();
        back_propagation();
        if (square_error() < epsilon) {
            return i;
        }
    }
    return epochs;
}
void Task::input() {
    // Reading image
    char number;
    for (int j = 1; j <= height; ++j) {
        for (int i = 1; i <= width; ++i) {
            image.read(&number, sizeof(char));
            if (number == 0) {
                d[i][j] = 0;
            } else {
                d[i][j] = 1;
            }
        }
    }
    /*
    cout << "Image:" << endl;
    for (int j = 1; j <= height; ++j) {
        for (int i = 1; i <= width; ++i) {
            cout << d[i][j];
        }
        cout << endl;
    }
*/
    for (int j = 1; j <= height; ++j) {
        for (int i = 1; i <= width; ++i) {
            int pos = i + (j - 1) * width;
            out1[pos] = d[i][j];
        }
    }

    // Reading label
    label.read(&number, sizeof(char));
    for (int i = 1; i <= n3; ++i) {
        expected[i] = 0.0;
    }
    expected[number + 1] = 1.0;

    //   cout << "Label: " << (int)(number) << endl;
}
void Task::fetch_data(){
    for(int i = 0; i < 60000;i++){
        train_data[i] = new double[784];
        train_expected[i] = new double[10];
        input();// out1,expected get
        for(int k = 1;k <= 784;k++)
            train_data[i][k-1] = out1[k];
        for(int k = 1;k <= 10;k++)
            train_expected[i][k-1] = expected[k];
    }
}
void Task::rand_image(){
    srand(time(NULL));
    int lucky_index = rand() % 60000;
    for(int i = 1;i <= 784;i++)
        out1[i] = train_data[lucky_index][i-1];
    for(int i = 1;i <= 10; i++)
        expected[i] = train_expected[lucky_index][i-1];

    //display
    if(0){
        cout << "Image:" << endl;
        for (int j = 1; j <= height; ++j) {
            for (int i = 1; i <= width; ++i) {
                cout << out1[i + (j - 1) * width];
            }
            cout << endl;
        }
        cout << "flag:" ;
        for(int i = 1;i <= 10; i++)
            if(expected[i] != 0)
                cout << i-1;
        cout << endl;
    }

}

void Task::get_image(int lucky_index){
    for(int i = 1;i <= 784;i++)
        out1[i] = train_data[lucky_index][i-1];
    for(int i = 1;i <= 10; i++)
        expected[i] = train_expected[lucky_index][i-1];

    //display
    if(0){
        cout << "Image:" << endl;
        for (int j = 1; j <= height; ++j) {
            for (int i = 1; i <= width; ++i) {
                cout << out1[i + (j - 1) * width];
            }
            cout << endl;
        }
        cout << "flag:" ;
        for(int i = 1;i <= 10; i++)
            if(expected[i] != 0)
                cout << i-1;
        cout << endl;
    }

}
void Task::write_matrix(std::string file_name) {
    ofstream file(file_name.c_str(), ios::out);

    // Input layer - Hidden layer
    for (int i = 1; i <= n1; ++i) {
        for (int j = 1; j <= n2; ++j) {
            file << w1[i][j] << " ";
        }
        file << endl;
    }

    // Hidden layer - Output layer
    for (int i = 1; i <= n2; ++i) {
        for (int j = 1; j <= n3; ++j) {
            file << w2[i][j] << " ";
        }
        file << endl;
    }

    file.close();
}
void Task::train(void* args){
    Task_set *temp = (Task_set*)args;
    Task *self_task   = temp->task_input[0];
    int start_sample = (self_task->nTraining/self_task->tot_machine) * self_task->num_machine;

    for(int sample = 1; sample <= self_task->sample_per_process; ++sample){
        // input();
        // rand_image();
        self_task->get_image(start_sample + sample - 1);
        // Learning process: Perceptron (Forward procedure) - Back propagation
        int nIterations = self_task->learning_process();

        // Save the current network (weights)
        if (sample % 100 == 0) {

            cout << "Sample " << sample << endl;
            cout << "No. iterations: " << nIterations << endl;
            printf("Error: %0.6lf\n\n", self_task->square_error());

        } // end sample == 100
    }// for done


}
void Task::merge(void *args){
    Task_set *temp = (Task_set*)args;
    Task *self_task   = temp->task_input[0];
    Task *input1_task = temp->task_input[1];
    Task *input2_task = temp->task_input[2];

    for(int i = 1;i <= self_task->n1;i++)//merge w1
        for(int j = 1;j <= self_task->n2;j++){
            self_task->w1[i][j] = (input1_task->w1[i][j]
                                   + input2_task->w1[i][j])/2;
        }
    for(int i = 1;i <= self_task->n2;i++)//merge w2
        for(int j = 1;j <= self_task->n3;j++){
            self_task->w2[i][j] = (input1_task->w2[i][j]
                                   + input2_task->w2[i][j])/2;
        }

   self_task->write_matrix(self_task->model_fn);
}

double* Task::ResulttoMPI(){
    int arr_len,index_count;
    arr_len = (n1+1)*(n2+1)+(n2+1)*(n3+1);
    index_count = (n1+1)*(n2+1);
    //static double mix_result[arr_len];
    double *mix_result = new double[arr_len];
    for (int i = 1; i <= n1; ++i) {
        for (int j = 1; j <= n2; ++j) {
            mix_result[i*(n2+1)+j]=w1[i][j];
        }
    }

    for (int k = 1; k <= n2; ++k) {
        for (int l = 1; l <= n3; ++l) {
            mix_result[index_count+k*(n3+1)+l]=w2[k][l];
        }
    }

    return mix_result;

}

void Task::MPIDecode(double* mix_result){
    int arr_len,index_count;
    arr_len = (n1+1)*(n2+1)+(n2+1)*(n3+1);
    index_count = (n1+1)*(n2+1);

    for (int i = 1; i <= n1; ++i) {
        for (int j = 1; j <= n2; ++j) {
            w1[i][j]= mix_result[i*(n2+1)+j];
        }
    }

    for (int k = 1; k <= n2; ++k) {
        for (int l = 1; l <= n3; ++l) {
           w2[k][l] = mix_result[index_count+k*(n3+1)+l];
        }
    }

}




Task_set::Task_set(){
    size_data = 0;
    task_input = NULL;
}
#if 1
Task_set::Task_set(std::vector<Task*> dep){
    size_data = dep.size();
    task_input = new Task*[size_data];
    for(int i = 0;i < size_data;i++){
        task_input[i] = dep[i];
    }
}
#endif
