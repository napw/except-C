//
// Created by wpans on 2018/9/29.
//

#ifndef SETJMP_EXCEPC_H
#define SETJMP_EXCEPC_H

#include <setjmp.h>

typedef struct ExceptMessage {
    char *message;
} ExceptMessage;
typedef struct Except {
    struct Except *Prev;
    jmp_buf env;
    const char *FileName;
    int Line;
    const ExceptMessage *What;

} Except;

enum {
    ExceptEntered = 0, ExceptRaised, ExceptHandled, ExceptFinalized
};

extern Except *ExceptStack;
extern const ExceptMessage AssertFailed;

void RaiseExcept(const ExceptMessage *m, const char *filename, int line);

#define THROW(m) RaiseExcept(&(m),__FILE__,__LINE__)
#define RERAISE RaiseExcept(except.What,except.FileName,except.Line)
#define RETURN switch(ExceptStack=ExceptStack->Prev,0){default:return}
#define CANCELUNRAISEDEXCEPTION if(ExceptFlag==ExceptEntered){ ExceptStack=ExceptStack->Prev;}

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


#endif //SETJMP_EXCEPC_H
