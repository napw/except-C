# except-C
利用setjmp()/longjmp()实现的C简易异常处理机制
## 
## **example**

``` c
    #include "exceptC.h"
    //define global variables as exception instance
    //except-C use the memory address to identified exception instance
    ExceptMessage except1 = {"except1"};
    ExceptMessage except2 = {"except2"};
    ExceptMessage except3 = {"except3"};
    
    int func4(int x){
        //THROW exception when necessary
        THROW(except1);
        return x;
    }

    void func3(){
        //the TRACK macro are used to generate backtrace infomation
        //only trival function call like "func()" or assignment like "v=func()" are valid
        TRACK(func4(6))
    }

    void func2(){
        TRACK(func3())
    }

    int func1(){
        TRACK(func2())
        return 88;
    }


    int main() {
        int x;
        TRACK(x=func1())
        printf("%d",x);
    }
```

