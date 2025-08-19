#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define QCAP 16

struct LogMsg {
    char buf[32];
    size_t len;
};

static struct LogMsg *queue[QCAP];
static size_t head = 0;
static size_t tail = 0;

static FILE *logfp = NULL;

#endif