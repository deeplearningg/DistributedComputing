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
int search_process(std::vector<process*> pool){
    for(int i = 0;i < pool.size();i++){
        if(pool[i]->isbusy == false){
          //  printf("%d ",i);
            return i;
        }
    }
    return 0;
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
//    DAGNode::init();

#if 1
	MPI_Init(NULL, NULL);

	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	DAGNode* root_node;
	root_node = DAGNode::init();

	int dag_size=0, layers=0;
    //root_node->dag_info(root_node, &dag_size, &layers);

 //   printf("DAG node number:%d\nProcess pool size:%d\n",dag_size,world_size);

   // int *process_pool = new int[world_size];
    vector<process*> process_pool;
    process_pool.resize(world_size);
    process_pool[0] = new process(0,true);
    for(int i = 1;i < world_size;i++)
        process_pool[i] = new process(i,false);


    int *index = new int[dag_size];
    for(int i = 0;i < dag_size;i++)
        index[i] = i;

    if(world_rank == 0){

        while(1){
            queue<DAGNode*> cur_layer;
            vector<pair_pp*> node_set;
            root_node->zero_indegree(cur_layer,root_node);
            printf("layer size %d\n",cur_layer.size());
            if(cur_layer.empty() == true){
                int over[1];
                over[0] = 42;
                for(int i = 1;i < process_pool.size();i++)// send over signal to 1,2,3...process
                    MPI_Send(over,1,MPI_INT,process_pool[i]->rank,0,MPI_COMM_WORLD);
                break;
            }

            while(cur_layer.empty()!=true){
                printf("layer size has %d\n",cur_layer.size());
                DAGNode* cur_node = cur_layer.front();
                cur_layer.pop();
                cur_node->del_dept();

                int process_id = 0;
                while( 1 ){;//wait until we get a free process
                    process_id = search_process(process_pool);
                    if(process_id != 0)
                        break;
                }
                printf("pick process %d\n",process_id);
                process_pool[process_id]->isbusy = true;
   //             for(int i = 0;i < process_pool.size();i++)
                //    printf("%d:%d",i,process_pool[i]->isbusy);
           //     getchar();
                pair_pp* temp_pair = new pair_pp(process_id,cur_node);

                node_set.push_back(temp_pair);
                int fake[1];
                cur_node->get_node_number(fake);
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
                    result = Task::ResulttoMPI(parent->task);
                    //parent->get_result(recv_data);
                    MPI_Send(result, arr_len, MPI_DOUBLE, process_id, p_node_num, MPI_COMM_WORLD);
                    printf("\t \t %d is sending data to %d ...\n", 0, process_id);

                }

            }// end traversal node in the cur_layer
           // printf("out of cur_layer\n");
            for(int i = 0; i<node_set.size();i++){//recv data from slaves

                MPI_Status status;
                int num_recv = 0;
                int c_node_num;
                node_set[i]->node->get_node_number(&c_node_num);
                printf("waiting for node %d from process %d\n",c_node_num,node_set[i]->process_id);
                MPI_Probe(node_set[i]->process_id, c_node_num, MPI_COMM_WORLD, &status);
                MPI_Get_count(&status, MPI_DOUBLE, &num_recv);
                double *recv_data = new double[num_recv];
                //parent->get_result(recv_data);
                MPI_Recv(recv_data, num_recv, MPI_DOUBLE, node_set[i]->process_id, c_node_num, MPI_COMM_WORLD,
                                         MPI_STATUS_IGNORE);
                //printf("here !!!\n");
                process_pool[ node_set[i]->process_id ]->isbusy = false;
                Task::MPIDecode(recv_data,node_set[i]->node->task);
            }
        }//traversal layers of DAG
    }//end master
    else{//for slave node
        while(1){
            DAGNode* cur_node;
            MPI_Status status;
            int num_recv = 0;
  //          if(world_rank == 1) printf("here0\n");
            MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
       //     if(world_rank == 1) printf("here1\n");
            MPI_Get_count(&status, MPI_INT, &num_recv);
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
                Task::MPIDecode(recv_data,parent->task);
                printf("\t \t %d is recieving data from %d ...\n", c_node_num, 0);
            }


            DAGNode::execute(cur_node);
            double* result ;
            printf("205\n");
            result = Task::ResulttoMPI(cur_node->task);
            printf("207\n");
            //send result to master
            int c_node_num;
            cur_node->get_node_number(&c_node_num);
            printf("%d\n",c_node_num);
            MPI_Send(result, arr_len, MPI_DOUBLE, 0, c_node_num, MPI_COMM_WORLD);
            printf("%d send node %d result to 0\n",world_rank,c_node_num);


        }//end while(1)
    }//end slave


    MPI_Finalize();


#endif
    return 0;

}
