//
// Created by wpans on 2018/9/29.
//

#include <stdio.h>
#include <stdlib.h>
#include "excepC.h"

Except *ExceptStack = NULL;


//#ifdef ENABLEBACKTRACE
BackTrace* TraceStack=NULL;
//#endif

static void PrintTrace(){
    while(TraceStack!=NULL){
        fprintf(stderr,"in func: %s at file: %s, line: %d\n",TraceStack->FuncCalled,TraceStack->FileName,TraceStack->Line);
        TraceStack=TraceStack->Prev;
    }

}

void RaiseExcept(const ExceptMessage *m, const char *filename, int line) {
    Except *latest = ExceptStack;
    assert(m);
    if (latest == NULL) {
        fprintf(stderr, "Uncaught exception");
        if (m->message != NULL) {
            fprintf(stderr, " %s", m->message);
        } else {
            fprintf(stderr, " at 0x%p", m);
        }
        if (filename != NULL && line > 0) {
            fprintf(stderr, " raised at %s:%d\n", filename, line);
        }
        //fprintf(stderr, "program aborting\n");
        PrintTrace();
        fflush(stderr);
        abort();
    }
    latest->What = m;
    latest->FileName = filename;
    latest->Line = line;
    ExceptStack = ExceptStack->Prev;
    longjmp(latest->env, ExceptRaised);
}
