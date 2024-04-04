#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



typedef struct {
    unsigned int valid;
    unsigned int tag;
    int LFU;
    int LRU;
}Line;


void LRUOrganize(Line* setLine,int i,int E){
    int oldLFU = setLine[i].LFU;
    setLine[i].LFU=E-1;
    int j=0;
    while(j < E){
        if((j != i) && (setLine[j].LRU > oldLFU)){
            setLine[j].LRU--;
        }
        j++;
    }
}

void solveSame(Line* SetLine,int p,int h,int tagsize,int hit,int miss,int cycle,long value) {
    if (SetLine->valid == 0 || (SetLine->valid == 1 && SetLine->tag != tagsize)) {
        printf("%lx M\n",value);
        SetLine->valid = 1;
        SetLine->tag = tagsize;
        miss++;
        cycle += h + p;
    } else {
        printf("%lx H\n",value);
        SetLine->tag = tagsize;
        hit++;
        cycle += h;
    }
}






int main() {
    printf("The first four lines containing the fundamental parameters of the cache,one per line\n");
    int unsigned S;   //number of sets per cache
    int unsigned E;   //number of lines per set
    int unsigned B;   //number of block per line
    int unsigned m;   //bit address
    int unsigned h;  //hit time
    int unsigned p;  //miss penalty
    long value=0;  //it is used for the input value
    char replacementType[4];   //to determine LRU and LFU
    scanf("%d",&S);
    scanf("%d",&E);
    scanf("%d",&B);
    scanf("%d",&m);
    scanf("%s",replacementType);
    scanf("%d",&h);
    scanf("%d",&p);
    printf("s");
    int s= (int) log2(S);   //no. of bit in set
    int b= (int) log2(B);   //no. of bit in block
    int t= m-s-b;    //no. of bits in tag
    int hits=0;    //to account for no. of hits
    int misses=0;  //to account for no. of misses
    int cycle=0;   //cycle=h+p;
    int C= S * E * B;



    //Lets set up for the cache
    Line** set = malloc (sizeof(Line) * S);
    for(int i = 0 ; i < S ; i++){
        set[i] = malloc(sizeof(Line) * E);
        for(int j=0;j<E;j++){
            set[i][j].valid=0;
            set[i][j].tag=0;
            set[i][j].LFU=0;
            set[i][j].LRU=j;
        }
    }
    while(value != -1){
        scanf("%lx",&value);
        if(value == -1){
            break;
        }
            long setsize = (value>>b)&((1<<s)-1);
            long tagsize = (value>>(s+b)&((1<<t)-1));
        Line* setLine= set[setsize];

        if(strcmp(replacementType,"LFU")==0){
            if(E==1){
                   solveSame(setLine,p,h,tagsize,hits,misses,cycle,value);
               }
            else if((E > 1 && E < C/B) || E == C/B){
                int setCapacity = 0;
                for(int i = 0 ; i < E ; i++){
                    if(setLine[i].valid == 0){    // valid is 0
                        printf("%lx M\n",value);
                        setLine[i].valid=1;
                        setLine[i].tag=tagsize;
                        setLine[i].LFU = 0;
                        misses++;
                        cycle+= p+h ;
                        break;
                    }

                    else if(setLine[i].tag!=tagsize){
                        setCapacity++;
                    }
                    else{
                        printf("%lx H\n",value);
                        setLine[i].LFU++;
                        hits++;
                        cycle +=h;
                        break;
                    }
                    if(setCapacity == E){
                        misses++;
                        cycle += p+h;
                        printf("%lx M\n",value);
                        int leastLFU= setLine[0].LFU;
                        int index = 0;
                        for(int j = 0; j < E; j++){
                            if(setLine[j].LFU<leastLFU){
                                leastLFU = setLine[j].LFU;
                                index = j;
                        }
                        }
                        setLine[index].tag=tagsize;
                        setLine[index].LFU=0;
                    }
                }
            }
            }
        if(strcmp(replacementType,"LRU")==0){
            if(E==1){
                solveSame(setLine,p,h,tagsize,h,m,cycle,value);
            }else if((E > 1 && E < C/B ) || E == C/B){
                int capacity=0;
                for(int i = 0;i < E;i++){
                    if(setLine[i].valid==0){
                        printf("%lx M\n",value);
                        setLine[i].valid=1;
                        setLine[i].tag=tagsize;
                        LRUOrganize(setLine,i,E);
                        misses++;
                        cycle += p+h;
                        break;
                    }
                    else if(setLine[i].valid==1 && setLine[i].tag!=tagsize){
                        capacity++;
                    }else{
                        printf("%lx H\n",value);
                        LRUOrganize(setLine,i,E);
                        hits++;
                        cycle+=h;
                        break;
                    }
                    if(capacity==E){
                        misses++;
                        cycle+=p+h;
                        printf("%lx M\n",value);
                        int index=0;
                        for(int j=0; j<E;j++){
                            if(setLine[j].LRU == 0){
                                index=j;
                            }
                        }
                        LRUOrganize(setLine,index,E);
                        setLine[index].tag=tagsize;
                    }
                }
            }
        }

        }
    double rate=(double)(misses*100)/(hits+misses);
    printf("%.f %d \n",rate,cycle);
    for(int i=0; i<S;i++){
        free(set[i]);
    }
    free(set);

    return 0;
}
