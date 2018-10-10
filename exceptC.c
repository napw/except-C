//
// Created by wpans on 2018/9/29.
//

#include <stdio.h>
#include <stdlib.h>
#include "excepC.h"

__thread Except *ExceptStack = NULL;

__thread BackTrace TraceStack[MAXCOUNTER];
__thread int BTCounter = 0;
__thread ExceptMessage *SignalMapper[65];


static void PrintTrace() {
    --BTCounter;
    while (BTCounter >= 0) {
        fprintf(stderr, "in func: %s at file: %s, line: %d\n", TraceStack[BTCounter].FuncCalled,
                TraceStack[BTCounter].FileName, TraceStack[BTCounter].Line);
        --BTCounter;
    }

}

void RaiseExcept(const ExceptMessage *m, const char *filename, int line, char IsSignal) {
    Except *latest = ExceptStack;
    assert(m);
    if (latest == NULL) {
        fprintf(stderr, "Uncaught exception");
        if (m->message != NULL) {
            fprintf(stderr, " %s", m->message);
        } else {
            fprintf(stderr, " at 0x%p", m);
        }
        if (!IsSignal) {
            if (filename != NULL && line > 0) {
                fprintf(stderr, " raised at %s:%d\n", filename, line);
            }
        } else {
            fprintf(stderr, " raised by signal %d\n", IsSignal);
        }

        PrintTrace();
        fflush(stderr);
        abort();
    }
    latest->What = m;
    latest->FileName = filename;
    latest->Line = line;
    latest->IsSignal = IsSignal;
    ExceptStack = ExceptStack->Prev;
    longjmp(latest->env, ExceptRaised);
}

int BindSignal2Except(int sig, ExceptMessage *e) {
    if (sig > 64 || sig < 1) {
        return -1;
    } else {
        SignalMapper[sig] = e;
        return signal(sig, Signal2Except) < 0 ? -1 : 0;
    }
}


void Signal2Except(int sig) {
    assert(SignalMapper[sig]);
    RaiseExcept(SignalMapper[sig], NULL, 0, sig);
}

void ReSetSignalBind(int sig) {
    SignalMapper[sig]=NULL;
    signal(sig,SIG_DFL);
}
