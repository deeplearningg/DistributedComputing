#ifndef SORT_WORD_H
#define SORT_WORD_H

#include <cstdio>
#include <iostream>
#include <string.h>
#include <vector>

#define max_ 80
#define HashMax (max_*122)
#define Max 1100000
#define ChMax 64000000
#define InWord  1
#define OutWord 0
#define Wanted(ch,n)  isalpha(ch) || ((ch == 39 || ch == 45) && isalpha(temp_ds->TotalCh[n-1]) && isalpha(temp_ds->TotalCh[n+1]))
#define arr_len Max*max_+2

inline int HashValue(char* temp);

class RecordWords{
public:
    RecordWords();
    RecordWords(char w1[],int p1,RecordWords *b1);
    char word[max_];
    int position;
    RecordWords *brother;
};
class data_struct{
public:
    data_struct();
    data_struct(char* _filename);
    data_struct(data_struct* _data_struct);

    void* input;
    void* output;

    char* filename;
    RecordWords *WordsHash;

    char WordsArr[Max][max_];
    int  WordsCount[Max];
    int  n_WordsCount;
    long int  nwords;

    int  TotalCh[ChMax];
};
class Task
{
public:
    Task();
    Task(char* _filename);
    Task(data_struct* _data_task);
    static void CreatArr(void *args);
    static void QuickSort(Task* _task,int left,int right);
    static void QuickSort_shell(void* args);
    static int  Partition(Task* _task,int left,int right);
    static bool Comp(Task* _task,int n1,int temp,char wtemp[]);
    static void SwapWord(Task* _task,int num1,int num2);
    static void write(void* args);
    static void merge_dict(void* args);

    //add
    static double* ResulttoMPI(Task* _task);
    static void  MPIDecode(double* mix_result,Task* _task);
//private:
    data_struct *data_task;
};

class Task_set2{
public:
    Task *task1,*task2;
};
class Task_set3{
public:
    Task_set3();
    Task_set3(Task *_task1,Task *_task2,Task *_task3);
    Task *task1,*task2,*task3;
};
class Task_set{
public:
  Task_set();
  Task_set(std::vector<Task*> dep);
  Task** task_input;
  int size_data;
};
#endif // SORT_WORD_H
