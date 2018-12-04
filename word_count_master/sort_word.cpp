#include "sort_word.h"
using namespace std;

inline int HashValue(char *temp){
    int i,sum = 0;
    for(i = 0;temp[i];i++)
        sum += (int)(temp[i]);
    return sum - 'a';//
}
data_struct::data_struct(){
    input  = NULL;
    output = NULL;

    filename = NULL;
    WordsHash = NULL;

    n_WordsCount = 0;
    nwords = 0;
}
data_struct::data_struct(char* _filename){
    filename = new char[max_];
    strcpy(filename,_filename);

    input  = NULL;
    output = NULL;
    WordsHash = NULL;
    n_WordsCount = 0;
    nwords = 0;
}
data_struct::data_struct(data_struct* _data_struct){
    ;
}
RecordWords::RecordWords(){
    for(int i = 0;i<max_;word[i++] = 0);
    position = 0;
    brother = NULL;
}

RecordWords::RecordWords(char w1[],int p1,RecordWords *b1){
    strcpy(word,w1);
    position = p1;
    brother = b1;// ?
}
Task::Task(){
    data_task = new data_struct;
}
Task::Task(char *_filename){
    data_task = new data_struct(_filename);
}
Task::Task(data_struct* _data_task){
    ;
}
void Task::SwapWord(Task* _task,int num1,int num2){   //
    char cword[max_];
    strcpy(cword,_task->data_task->WordsArr[num1]);
    strcpy(_task->data_task->WordsArr[num1],
           _task->data_task->WordsArr[num2]);
    strcpy(_task->data_task->WordsArr[num2],cword);
}

bool Task::Comp(Task* _task,int n1,int temp,char wtemp[]){   //
    if(_task->data_task->WordsCount[n1] > temp)
        return true;
    if(_task->data_task->WordsCount[n1] < temp)
        return false;
    int i ;
    for(i = 0;(i<max_)&&(_task->data_task->WordsArr[n1][i] == wtemp[i]);i++);
    if(_task->data_task->WordsArr[n1][i] < wtemp[i]) return false;
    else return true;
}

int Task::Partition(Task* _task,int left,int right){
    int l = left;
    int r = right;
    int temp = _task->data_task->WordsCount[r];
    char wtemp[max_];
    strcpy(wtemp,_task->data_task->WordsArr[r]);
    while(l != r){
        while(!Comp(_task,l,temp,wtemp) && r > l)
            l++;
        if(l < r){
            _task->data_task->WordsCount[r] = _task->data_task->WordsCount[l];
            strcpy(_task->data_task->WordsArr[r],_task->data_task->WordsArr[l]);
            r--;
        }
        while(Comp(_task,r,temp,wtemp) && r > l)
            r--;
        if(l<r){
            _task->data_task->WordsCount[l] = _task->data_task->WordsCount[r];
            strcpy(_task->data_task->WordsArr[l] , _task->data_task->WordsArr[r]);
            l++;
        }
    }
    _task->data_task->WordsCount[l] = temp;
    strcpy(_task->data_task->WordsArr[l] , wtemp);
    return l;
}
void Task::QuickSort_shell(void* args){

//    Task_set2* task_set2 = (Task_set2*)args;
 //   Task *temp_task = task_set2->task1;
//    Task *self_task = task_set2->task2;
    Task_set *task_st = (Task_set*)args;
    Task *self_task = task_st->task_input[0];
    Task *temp_task = task_st->task_input[1];

    printf("enter quicksort shell\n");
    if(1){
        printf("n_wordsCount = %d\n",temp_task->data_task->n_WordsCount);
        printf("nwords = %d\n",temp_task->data_task->nwords);
        for(int i = 0;i < temp_task->data_task->n_WordsCount;i++){

            printf("%s:\t",temp_task->data_task->WordsArr[i]);
            printf("%d\n",temp_task->data_task->WordsCount[i]);

        }
    }
    printf("print done!\n");
    QuickSort(temp_task,0,temp_task->data_task->n_WordsCount -1);
    printf("after Quick Sort\n");
    for(int i = 0;i < temp_task->data_task->n_WordsCount;i++){
        printf("%d\n",i);
        strcpy(self_task->data_task->WordsArr[i],temp_task->data_task->WordsArr[i]);
        self_task->data_task->WordsCount[i] = temp_task->data_task->WordsCount[i];
    }
    self_task->data_task->nwords = temp_task->data_task->nwords;
    self_task->data_task->n_WordsCount = temp_task->data_task->n_WordsCount;
    printf("quick sort shell done!\n");
}
void Task::QuickSort(Task* _task,int left,int right){    //
    if(right <= left ) return;
    int pivot = (left + right)/2; //
    int itemp = _task->data_task->WordsCount[pivot];
    _task->data_task->WordsCount[pivot] = _task->data_task->WordsCount[right];
    _task->data_task->WordsCount[right] = itemp;
    SwapWord(_task,pivot,right);
    pivot = Partition(_task,left,right);
    Task::QuickSort(_task,left,pivot-1);
    Task::QuickSort(_task,pivot+1,right);
}
void Task::CreatArr(void* args){
    Task_set *temp_ts = (Task_set*)args;
    Task *temp_task = temp_ts->task_input[0];
    data_struct *temp_ds = temp_task->data_task;
    FILE *fpr;
    if( ( fpr=fopen(temp_ds->filename,"r") )== NULL ){//UnSortedFile
        cout <<"open fail\n";
    }
    temp_ds->WordsHash = new RecordWords[HashMax];
    char tempword[max_];
    int ch,n_tempw,state,value;
    state = OutWord;
    temp_ds->nwords = n_tempw = 0;
    temp_ds->n_WordsCount = 0;
    for(int i = 0;i<max_;tempword[i++] = 0);
    int i_ch = 0;
    while((i_ch < 64000000)&& (temp_ds->TotalCh[i_ch++] = fgetc(fpr)) != EOF);
    for(int i_ch_temp = 0;i_ch_temp < i_ch;i_ch_temp++){// the total characters: i_ch
        ch = temp_ds->TotalCh[i_ch_temp];
        if( Wanted(ch,i_ch_temp) ){
            if(ch != 45){
                tempword[n_tempw++] = tolower(ch);
                state = InWord;
            }
        }
        else{// now we get the word tempword
            if(state == InWord){
                temp_ds->nwords++;
                value =  HashValue(tempword);

                if(temp_ds->WordsHash[value].word[0] == '0'){
                    temp_ds->WordsHash[value].position = temp_ds->n_WordsCount;
                    strcpy(temp_ds->WordsHash[value].word,tempword);
                    strcpy(temp_ds->WordsArr[temp_ds->n_WordsCount],tempword);
                    temp_ds->WordsCount[temp_ds->n_WordsCount] = 1;
                    temp_ds->n_WordsCount++;
                }

                else{   //
                    RecordWords *u;
                    RecordWords *v;
                    for(v = &(temp_ds->WordsHash[value]);(v != NULL)&&(strcmp(tempword,v->word) != 0);v = v->brother)
                        u = v;
                    if(v == NULL){   //
                        v = new RecordWords(tempword,temp_ds->n_WordsCount,NULL);
                        u->brother = v;
                        strcpy(temp_ds->WordsArr[temp_ds->n_WordsCount],tempword);
                        temp_ds->WordsCount[temp_ds->n_WordsCount] = 1;
                        temp_ds->n_WordsCount++;
                    }
                    else{    //
                        temp_ds->WordsCount[v->position] ++;
                    }
                }

                for(int i = 0;i<max_;tempword[i++] = 0);  //
                n_tempw = 0;
                state = OutWord;
            }
        }
    }
    fclose(fpr);
}
void Task::write(void* args){
    Task_set *task_st = (Task_set*)args;
    Task *temp_task = task_st->task_input[1];
    //Task *temp_task = (Task*)args;
    data_struct *temp_ds = temp_task->data_task;
    FILE *fpw;
    if( (fpw = fopen("SortedFile","w")) == NULL){
        cout <<"fpw open fail!\n";
        return;
    }
    fprintf(fpw,"%-15s\t%-15s\n","Words","Frequency");

    for(int i = 0;i < temp_ds->n_WordsCount;i++)
        fprintf(fpw,"%-15s\t%-15d\n",temp_ds->WordsArr[i],temp_ds->WordsCount[i]);

    fprintf(fpw,"%-15s\t%-15d\n","total words:",temp_ds->nwords);
    fprintf(fpw,"%-15s\t%-15d\n","kinds words:",temp_ds->n_WordsCount);
}
void Task::merge_dict(void* args){
    printf("enter merge_dict\n");
    Task_set* task_set = (Task_set*)args;
    Task *self_task  = task_set->task_input[0];
    Task *dict1_task = task_set->task_input[1];//WordsArr,  WordsCount
    Task *dict2_task = task_set->task_input[2];
    printf("debug 0\n");
    printf("self = %ox,self = %ox,self = %ox\n",self_task,dict1_task,dict2_task);
    self_task->data_task->nwords = dict1_task->data_task->nwords +  dict2_task->data_task->nwords;
    self_task->data_task->n_WordsCount = 0;
    int i_wordscount = 0;
    // hash
    RecordWords *WordsHash = new RecordWords[HashMax];
    int wc1 = dict1_task->data_task->n_WordsCount;
    int wc2 = dict2_task->data_task->n_WordsCount;
    int wc = wc1 + wc2;
    for(int i = 0;i < wc;i++){

        char tempword[max_];
        if(i < wc1)
            strcpy( tempword, (dict1_task->data_task->WordsArr[i]) );
        else
            strcpy( tempword,(dict2_task->data_task->WordsArr[i-wc1]) );

        int value = HashValue(tempword);

        if(WordsHash[value].word[0] == '0'){// first arrive
            WordsHash[value].position = i_wordscount;
            strcpy(WordsHash[value].word, tempword);

            strcpy(self_task->data_task->WordsArr[i_wordscount],tempword);
            self_task->data_task->WordsCount[i_wordscount] = 1;
            i_wordscount++;
        }

        else{   //
            RecordWords *u;
            RecordWords *v;
            for(v = &(WordsHash[value]);(v != NULL)&&(strcmp(tempword,v->word) != 0);v = v->brother)
                u = v;
            if(v == NULL){   //
                v = new RecordWords(tempword,i_wordscount,NULL);
                u->brother = v;
                strcpy( self_task->data_task->WordsArr[i_wordscount],tempword);
                 self_task->data_task->WordsCount[i_wordscount] = 1;
                i_wordscount++;
            }
            else{    //
                self_task->data_task->WordsCount[v->position]++;
            }
        }

    }// end for wc
    printf("debug 1\n");
    self_task->data_task->n_WordsCount = i_wordscount;
}
double* Task::ResulttoMPI(Task* _task){
    static double mix_result[arr_len];
    int index_count1,index_count2;
    index_count1 = _task->data_task->n_WordsCount*max_;
    index_count2 = index_count1 + _task->data_task->n_WordsCount;
    mix_result[0] = _task->data_task->n_WordsCount;
    for(int i=0;i<_task->data_task->n_WordsCount;i++){
        for(int j=0;j<max_;j++){
            mix_result[i*max_+j +1] = _task->data_task->WordsArr[i][j];
        }

    }
    for(int k=0;k<_task->data_task->n_WordsCount;k++){
        mix_result[index_count1+k +1]= _task->data_task->WordsCount[k];
    }

  //  mix_result[index_count2] = _task->data_task->n_WordsCount;
    mix_result[index_count2+1] = _task->data_task->nwords;

    return mix_result;
}


void  Task::MPIDecode(double* mix_result,Task* _task){

    int index_count1,index_count2;
    _task->data_task->n_WordsCount = mix_result[0];
    index_count1 = _task->data_task->n_WordsCount*max_;
    index_count2 = index_count1 + _task->data_task->n_WordsCount;

    for(int i=0;i<_task->data_task->n_WordsCount;i++){
        for(int j=0;j<max_;j++){
             _task->data_task->WordsArr[i][j]=mix_result[i*max_+j + 1];
        }

    }
    for(int k=0;k<_task->data_task->n_WordsCount;k++){
        _task->data_task->WordsCount[k]=mix_result[index_count1+k +1];

    }

  //  _task->data_task->n_WordsCount=mix_result[index_count2];
    _task->data_task->nwords = mix_result[index_count2+1];

}

Task_set3::Task_set3(){
    task1 = NULL;
    task2 = NULL;
    task3 = NULL;
}
Task_set3::Task_set3(Task *_task1,Task *_task2,Task *_task3){
    task1 = _task1;
    task2 = _task2;
    task3 = _task3;
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
