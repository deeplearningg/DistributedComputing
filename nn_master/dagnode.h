#ifndef DAGNODE_H
#define DAGNODE_H
#include <queue>
#include <vector>
#include "nn.h"


class DAGNode
{
public:
    DAGNode();
    DAGNode(Task *_task, void (*_exec)(void*), void* _data,
            void* _result, DAGNode* start, int _node_number);

    void dag_info(DAGNode* cur_node,int* dag_size,int* layers);// DAG information
    void get_parents(DAGNode* cur_node,std::queue<DAGNode*>& q);// get parent node
    void get_childs (DAGNode* cur_node,std::queue<DAGNode*>& q);
    void get_result(void* _result);
    void set_data(void* _data);
    void get_node_number(int* _node_number);
    static DAGNode *get_node(DAGNode* _root,int number);

    static DAGNode *init();
    virtual bool set_depedence(DAGNode* depce);
    virtual bool del_dept();
    virtual bool display();
    bool zero_indegree(std::queue<DAGNode*>& q,DAGNode* root);
    friend bool schedular(DAGNode* root);
    static void execute(void* args);
    void init_data();

//private:
    Task *task;
    void* data;
    void* result;
    void (*exec)(void*);

    DAGNode* root;
    int node_number;

    std::vector<DAGNode*> depedence;
    std::vector<DAGNode*> childs;
    DAGNode*  parallel;
    bool      flag;
};
class dag_set{
public:
  dag_set();
  dag_set( std::vector<DAGNode*> dep);
  Task_set* task_input;
};
struct dag_set2{
  DAGNode* node1;
  DAGNode* node2;
};

#endif // DAGNODE_H
