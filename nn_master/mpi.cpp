#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <queue>

#include "dagnode.h"

using namespace std;
class process{
public:
    process(){
        rank = 0;
        isbusy = true;
        node = NULL;
    }
    process(int _rank,int _isbusy){
        rank = _rank;
        isbusy = _isbusy;
        node = NULL;
    }

    int rank;
    bool isbusy;
    DAGNode* node;
};
bool search_process(std::vector<process*> pool,int* process_number){
    for(int i = 0;i < pool.size();i++){
        if(pool[i]->isbusy == false){
            *process_number = pool[i]->rank;
            return true;
        }
    }
    return false;
}
class pair_pp{
public:
    pair_pp(){
        process_id = 42;
        node = NULL;
    }
    pair_pp(int _process_id,DAGNode* _node){
        process_id = _process_id;
        node = _node;
    }
    int process_id;
    DAGNode* node;
};
int main(int argc, char** argv){
	MPI_Init(NULL, NULL);

	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	DAGNode* root_node;
	root_node = DAGNode::init();

	int dag_size=0, layers=0;
	root_node->dag_info(root_node, &dag_size, &layers);

    printf("DAG node number:%d\nProcess pool size:%d\n",dag_size,world_size);

   // int *process_pool = new int[world_size];
    vector<process*> process_pool;
    process_pool.resize(world_size);
    for(int i = 0;i < world_size;i++)
        process_pool[i] = new process(i,false);

    vector<pair_pp*> node_set;
    int *index = new int[dag_size];
    for(int i = 0;i < dag_size;i++)
        index[i] = i;

    if(world_rank == 0){

        while(1){
            queue<DAGNode*> cur_layer;
            root_node->zero_indegree(cur_layer,root_node);
            if(cur_layer.empty() == true){
                int over[1];
                over[0] = 42;
                for(int i = 1;i < process_pool.size();i++)// send over signal to 1,2,3...process
                    MPI_Send(over,1,MPI_INT,process_pool[i]->rank,0,MPI_COMM_WORLD);
                break;
            }

            if(cur_layer.empty()!=true){
                DAGNode* cur_node = cur_layer.front();
                cur_layer.pop();
                cur_node->del_dept();

                int process_id;
                while( search_process(process_pool,&process_id) == false);//wait until we get a free process

                pair_pp* temp_pair = new pair_pp(process_id,cur_node);
                node_set.push_back(temp_pair);
                int fake[1];
                fake[0] = process_id;
                MPI_Send(fake,1,MPI_INT,process_id,0,MPI_COMM_WORLD);// start signal
                queue<DAGNode*> parents ;
                cur_node->get_parents(cur_node, parents);


                while( (!parents.empty()) ){// has parents

                    DAGNode* parent=parents.front();
                    parents.pop();
                    int p_node_num, c_node_num;
                    parent->get_node_number(&p_node_num);
                    cur_node->get_node_number(&c_node_num);

                    double* result ;
                    result = parent->task->ResulttoMPI();
                    //parent->get_result(recv_data);
                    int arr_len = (parent->task->n1+1)*(parent->task->n2+1)
                            +(parent->task->n2+1)*(parent->task->n3+1);
                    MPI_Send(result, arr_len, MPI_DOUBLE, process_id, p_node_num, MPI_COMM_WORLD);
                    printf("\t \t %d is sending data to %d ...\n", 0, process_id);

                }

                for(int i = 0; i<node_set.size();i++){//recv data from slaves

                    MPI_Status status;
                    int num_recv = 0;
                    int c_node_num;
                    node_set[i]->node->get_node_number(&c_node_num);
                    MPI_Probe(node_set[i]->process_id, c_node_num, MPI_COMM_WORLD, &status);
                    MPI_Get_count(&status, MPI_DOUBLE, &num_recv);
                    double *recv_data = new double[num_recv];
                    //parent->get_result(recv_data);
                    MPI_Recv(recv_data, num_recv, MPI_DOUBLE, node_set[i]->process_id, c_node_num, MPI_COMM_WORLD,
                                             MPI_STATUS_IGNORE);
                    node_set[i]->node->task->MPIDecode(recv_data);

                }

            }// traversal node in the cur_layer
        }//traversal layers of DAG
    }//end master
    else{//for slave node
        while(1){
            DAGNode* cur_node;
            MPI_Status status;
            int num_recv = 0;
            MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
            MPI_Get_count(&status, MPI_DOUBLE, &num_recv);
            int *recv_data = new int[num_recv];
            MPI_Recv(recv_data, num_recv, MPI_INT, 0, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
            printf("rank %d recv %d from rank %d\n",world_rank,recv_data[0],0);

            if(num_recv == 1){
                if(recv_data[0] == 42)
                    break;
                else
                    cur_node = DAGNode::get_node(root_node,recv_data[0]);
            }

            queue<DAGNode*> parents ;
            cur_node->get_parents(cur_node, parents);

            while( (!parents.empty()) ){// has parents and not layer 1
                DAGNode* parent=parents.front();
                parents.pop();
                int p_node_num, c_node_num;
                parent->get_node_number(&p_node_num);
                cur_node->get_node_number(&c_node_num);
                MPI_Status status;
                int num_recv = 0;
                MPI_Probe(0, p_node_num, MPI_COMM_WORLD, &status);
                MPI_Get_count(&status, MPI_DOUBLE, &num_recv);
                double *recv_data = new double[num_recv];
                //parent->get_result(recv_data);
                MPI_Recv(recv_data, num_recv, MPI_DOUBLE, 0, p_node_num, MPI_COMM_WORLD,
                                         MPI_STATUS_IGNORE);
                parent->task->MPIDecode(recv_data);
                printf("\t \t %d is recieving data from %d ...\n", c_node_num, 0);
            }

            DAGNode::execute(cur_node);
            double* result ;
            result = cur_node->task->ResulttoMPI();
            //send result to master
            int arr_len = (cur_node->task->n1+1)*(cur_node->task->n2+1)
                      +(cur_node->task->n2+1)*(cur_node->task->n3+1);
            int c_node_num;
            cur_node->get_node_number(&c_node_num);
            MPI_Send(result, arr_len, MPI_DOUBLE, 0, c_node_num, MPI_COMM_WORLD);
            printf("%d send node %d result to 0\n",world_rank,c_node_num);


        }//end while(1)
    }//end slave


    MPI_Finalize();
    return 0;

}
