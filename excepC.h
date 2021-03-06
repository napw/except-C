//
// Created by wpans on 2018/9/29.
//

#ifndef SETJMP_EXCEPC_H
#define SETJMP_EXCEPC_H

#include <setjmp.h>
#include <assert.h>
#include <signal.h>

typedef struct ExceptMessage {
    char *message;
} ExceptMessage;
typedef struct Except {
    struct Except *Prev;
    jmp_buf env;
    const char *FileName;
    int Line;
    const ExceptMessage *What;
    char IsSignal;
} Except;


enum {
    ExceptEntered = 0, ExceptRaised, ExceptHandled, ExceptFinalized
};

extern __thread Except *ExceptStack;

void RaiseExcept(const ExceptMessage *m, const char *filename, int line, char IsSignal);
int BindSignal2Except(int signal, ExceptMessage *e);
void Signal2Except(int signal);
void ReSetSignalBind(int signal);

#define THROW(m) RaiseExcept(&(m),__FILE__,__LINE__,0)
#define RERAISE RaiseExcept(except.What,except.FileName,except.Line,except.IsSignal)
#define RETURN switch(ExceptStack=ExceptStack->Prev,0){default:return}
#define CANCELUNRAISEDEXCEPTION if(ExceptFlag==ExceptEntered){ ExceptStack=ExceptStack->Prev;}
#define BINDSIGNALTOEXCEPTION(sig,Except) BindSignal2Except(sig,&(Except))

#define TRY \
    do{\
        Except except;\
        except.Prev=ExceptStack;\
        ExceptStack=&except;\
        volatile int ExceptFlag=setjmp(except.env);\
        if (ExceptFlag==ExceptEntered){\

#define CATCH(e)\
            CANCELUNRAISEDEXCEPTION\
        }else if(except.What==&(e)){\
            ExceptFlag=ExceptHandled;\

#define FINALLY\
            CANCELUNRAISEDEXCEPTION\
        } {\
            if (ExceptFlag==ExceptEntered){\
                ExceptFlag=ExceptFinalized;\
            }

#define ELSE\
            CANCELUNRAISEDEXCEPTION\
        }else{\
            ExceptFlag=ExceptHandled;

#define ENDTRY\
            CANCELUNRAISEDEXCEPTION\
        }\
        if(ExceptFlag==ExceptRaised){\
            RERAISE;\
        }\
    }while(0);

typedef struct BackTrace{
    struct BackTrace* Prev;
    const char* FuncCalled;
    const char* FileName;
    int Line;
}BackTrace;

#ifndef MAXCOUNTER
#define MAXCOUNTER 256
#endif

//backtrace info declaration
extern __thread int BTCounter;
extern __thread BackTrace TraceStack[MAXCOUNTER];

#define PUSHTRACEINFO(func) do{\
    int pushed=0;\
    if (BTCounter<MAXCOUNTER){\
        pushed=1;\
        TraceStack[BTCounter].FileName=__FILE__;\
        TraceStack[BTCounter].Line=__LINE__;\
        TraceStack[BTCounter].FuncCalled=#func;\
        ++BTCounter;\
    }\


#define POPTRACEINFO \
    assert(TraceStack>0);\
    if (pushed){\
        --BTCounter;\
    }\
}while(0);\

#define TRACK(func) \
    PUSHTRACEINFO(func)\
    (func);\
    POPTRACEINFO\


extern __thread  ExceptMessage* SignalMapper[65];

#endif //SETJMP_EXCEPC_H
