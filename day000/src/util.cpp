#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <functional>
#include <memory>
 void errinfo(bool bRes, const char* info)
 {
    if(bRes)
    {
        perror(info);
        exit(EXIT_FAILURE);
    }
 }

 typedef struct s_Teststruct
{
    int inta = 0;
    double doubleb = 1.0;
    s_Teststruct(int a,double b) : inta(a),doubleb(b){};

    /* data */
} sTeststruct;

bool sortST(const sTeststruct &a, const sTeststruct &b)
{
    return a.doubleb < b.doubleb;
}


typedef int (*TestCallback)(int a, int b);

int useCallback(int a, int b, const TestCallback &func)
{
    return func(a, b);
}

int add(int a, int b)
{
    return a + b;
}

typedef std::function<int(int,int)> callInt;
int CallFunc(int a, int b, const callInt &func)
{
    return func(a,b);
}


bool CompareInt(int a,int b)
{
    return a<b;
}
std::function<bool(int,int)> callCompareFun = CompareInt;


class BrainToolBox
{
private:
    /* data */
public:
    BrainToolBox(/* args */);
    ~BrainToolBox();
     int Add(int a, int b) { return a + b; };
};

BrainToolBox::BrainToolBox(/* args */)
{
}

BrainToolBox::~BrainToolBox()
{
}

int main2()
{
    CallFunc(3,6,add);

    useCallback(1, 2, add);


    std::vector<sTeststruct> vTest;
    vTest.push_back(sTeststruct(1, 2.5));
    vTest.push_back(sTeststruct(8, 2.3));
    vTest.push_back(sTeststruct(5, 2.4));
    vTest.push_back(sTeststruct(4, 2.7));
    vTest.push_back(sTeststruct(2, 2.9));
    std::sort(vTest.begin(), vTest.end(), sortST);

    std::vector<int> vInt;
    vInt.push_back(1);
    vInt.push_back(5);
    vInt.push_back(3);
    vInt.push_back(7);

    std::sort(vInt.begin(), vInt.end(), CompareInt);


    std::shared_ptr<BrainToolBox> brain = std::make_shared<BrainToolBox>();
    std::function<int(int,int)> addFunc = std::bind(&BrainToolBox::Add, brain,std::placeholders::_1,std::placeholders::_2);
    int c = addFunc(2,5);
    return 0;
}