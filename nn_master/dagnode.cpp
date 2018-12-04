#include <iostream>
#include <queue>
#include <cstdarg>
#include <vector>
#include <pthread.h>
#include <cstdlib>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "dagnode.h"
using namespace std;

int episode_number = 0;
int episode_count  = 0;
pthread_mutex_t episode_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  episode_done = PTHREAD_COND_INITIALIZER;

DAGNode::DAGNode()
{
    root      = NULL;
    parallel  = NULL;
    data      = NULL;
    result    = NULL;
    task      = NULL;
    flag      = false;
    node_number = 0;
}
DAGNode::DAGNode(Task *_task, void (*_exec)(void*), void* _data, void *_result, DAGNode* start, int _node_number)
{
    result = 0;
    flag   = true;
    parallel  = NULL;
    exec = _exec;
    node_number = _node_number;
    task = _task;

    data    = _data ;
    result = _result;

    if(start != NULL){
        root = start;
        DAGNode* temp = root;
        while(temp->parallel)
            temp = temp -> parallel;
        temp -> parallel = this;
    }
    else{
        root = this;
        parallel = NULL;
    }
}
bool DAGNode::set_depedence(DAGNode* depce)
{
    depedence.push_back(depce);
    depce->childs.push_back(this);
    return true;
}

bool DAGNode::del_dept()
{
    flag = false;
    return true;
}
bool DAGNode::display()
{
    if(root == NULL && parallel == NULL)
        return false;
    for(DAGNode* temp = parallel;temp != NULL;temp = temp -> parallel)
    {
        cout << "node number is:" << temp->node_number << endl;
        cout << "depedence: ";
        vector<DAGNode*>::iterator it;
        for(it = (temp->depedence).begin();it != (temp->depedence).end();it++){
            cout << (*it)->node_number << "  ";
        }
        cout << "child: ";
        //vector<DAGNode*>::iterator it;
        for(it = (temp->childs).begin();it != (temp->childs).end();it++){
            cout << (*it)->node_number << "  ";
        }
        cout << endl;
    }
    return true;
}
void DAGNode::execute(void* args)
{
    DAGNode* node_dag =(DAGNode*)args;
    (*node_dag->exec)(node_dag->data);
/*
    pthread_mutex_lock(&episode_lock);
    episode_count++;
    if( episode_count == episode_number )
        pthread_cond_signal(&episode_done);
    pthread_mutex_unlock(&episode_lock);
*/

}
bool schedular(DAGNode* root)
{
    return true;
}
bool DAGNode::zero_indegree(queue<DAGNode*>& q,DAGNode* root)
{
    bool flag;
    for(DAGNode* target = root;target != NULL;target = target -> parallel){
        if( (target->flag) == false )
            continue;
        if( (target->depedence).size() == 0 ){
            q.push(target);
            continue;
        }
        vector<DAGNode*>::iterator it;
        for(it = (target->depedence).begin();it != (target->depedence).end();it++){
            if( (*it)->flag == true ){
                break;
            }
        }
        if(it == (target->depedence).end() )
            q.push(target);
    }
    return true;

}

void DAGNode::dag_info(DAGNode* cur_node,int* dag_size,int* layers){
    DAGNode *_root = cur_node->root;
    *dag_size = 3;
    *layers   = 2;
}// DAG information
void DAGNode::get_parents(DAGNode* cur_node, std::queue<DAGNode*> &q){// get parent node
    for(int i = 0;i < cur_node->depedence.size();i++)
        q.push(cur_node->depedence[i]);
}
void DAGNode::get_childs(DAGNode* cur_node,std::queue<DAGNode*>& q){
    for(int i = 0;i < cur_node->childs.size();i++)
        q.push(cur_node->childs[i]);
}
void DAGNode::get_result(void* _result){
    _result = result;
}
void DAGNode::set_data(void* _data){
    _data = data;
}
void DAGNode::get_node_number(int *_node_number){
    *_node_number = node_number;
}
DAGNode* DAGNode::get_node(DAGNode* _root,int number){
    if(number == 0) return _root;
    for(DAGNode* temp = _root->parallel;temp != NULL;temp = temp -> parallel)
    {
       if( temp->node_number == number)
           return temp;
    }
}
DAGNode* DAGNode::init(){
    Task *task0 = new Task(10,2,0);//30000,2,0
    Task *task1 = new Task(10,2,1);
    Task *task2 = new Task;

    DAGNode *DAGNode0 = new DAGNode(task0,Task::train,
                                     (void*)NULL,(void*)task0,NULL,0);// for createArr node
    DAGNode *DAGNode1 = new DAGNode(task1,Task::train,
                                     (void*)NULL,(void*)task1,DAGNode0,1);// for createArr node
    DAGNode *DAGNode2 = new DAGNode(task2,Task::merge,
                                     (void*)NULL,(void*)task2,DAGNode0,2);// merge two dicts

    DAGNode2->set_depedence(DAGNode0);
    DAGNode2->set_depedence(DAGNode1);


    DAGNode0->init_data();
    DAGNode1->init_data();
    DAGNode2->init_data();

    DAGNode0->display();

    return DAGNode0;
   // _root = DAGNode0;
}
void DAGNode::init_data(){
    std::vector<Task*> input_task;
    input_task.push_back(this->task);
    for(int i = 0;i < depedence.size();i++)
        input_task.push_back(depedence[i]->task);

    Task_set *task_input = new Task_set(input_task);
    data = (void*)(task_input);
}
dag_set::dag_set(){
    task_input = NULL;
}
dag_set::dag_set(std::vector<DAGNode*> input){
    std::vector<Task*> input_task;
    for(int i = 0;i < input.size();i++)
        input_task.push_back(input[i]->task);
    task_input = new Task_set(input_task);
}
