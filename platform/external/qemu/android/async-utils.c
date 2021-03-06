/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "android/async-utils.h"
#include "android/utils/eintr_wrapper.h"
#include "unistd.h"

void
asyncReader_init(AsyncReader* ar,
                 void*        buffer,
                 size_t       buffsize,
                 LoopIo*      io)
{
    ar->buffer   = buffer;
    ar->buffsize = buffsize;
    ar->pos      = 0;
    ar->io       = io;
    if (buffsize > 0)
        loopIo_wantRead(io);
}

AsyncStatus
asyncReader_read(AsyncReader*  ar)
{
    int  ret;

    if (ar->pos >= ar->buffsize) {
        return ASYNC_COMPLETE;
    }

    do {
        ret = HANDLE_EINTR(
                socket_recv(ar->io->fd,
                            ar->buffer + ar->pos,
                            ar->buffsize - ar->pos));
        if (ret == 0) {
            
            errno = ECONNRESET;
            return ASYNC_ERROR;
        }
        if (ret < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                loopIo_wantRead(ar->io);
                return ASYNC_NEED_MORE;
            }
            return ASYNC_ERROR;
        }
        ar->pos += ret;

    } while (ar->pos < ar->buffsize);

    loopIo_dontWantRead(ar->io);
    return ASYNC_COMPLETE;
}

void
asyncWriter_init(AsyncWriter*  aw,
                 const void*   buffer,
                 size_t        buffsize,
                 LoopIo*       io)
{
    aw->buffer   = buffer;
    aw->buffsize = buffsize;
    aw->pos      = 0;
    aw->io       = io;
    if (buffsize > 0)
        loopIo_wantWrite(io);
}

AsyncStatus
asyncWriter_write(AsyncWriter* aw)
{
    int  ret;

    if (aw->pos >= aw->buffsize) {
        return ASYNC_COMPLETE;
    }

    do {
        ret = HANDLE_EINTR(
                socket_send(aw->io->fd,
                            aw->buffer + aw->pos,
                            aw->buffsize - aw->pos));
        if (ret == 0) {
            
            errno = ECONNRESET;
            return ASYNC_ERROR;
        }
        if (ret < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                return ASYNC_NEED_MORE;
            }
            return ASYNC_ERROR;
        }
        aw->pos += ret;

    } while (aw->pos < aw->buffsize);

    loopIo_dontWantWrite(aw->io);
    return ASYNC_COMPLETE;
}


void
asyncLineReader_init(AsyncLineReader* alr,
                     void*            buffer,
                     size_t           buffsize,
                     LoopIo*          io)
{
    alr->buffer   = buffer;
    alr->buffsize = buffsize;
    alr->pos      = 0;
    alr->io       = io;
    alr->eol      = '\n';
    if (buffsize > 0)
        loopIo_wantRead(io);
}

AsyncStatus
asyncLineReader_read(AsyncLineReader* alr)
{
    int  ret;

    if (alr->pos >= alr->buffsize) {
        errno = ENOMEM;
        return ASYNC_ERROR;
    }

    do {
        char ch;
        ret = HANDLE_EINTR(socket_recv(alr->io->fd, &ch, 1));
        if (ret == 0) {
            
            errno = ECONNRESET;
            return ASYNC_ERROR;
        }
        if (ret < 0) {
            if (errno == EWOULDBLOCK || errno == EAGAIN) {
                loopIo_wantRead(alr->io);
                return ASYNC_NEED_MORE;
            }
            return ASYNC_ERROR;
        }
        alr->buffer[alr->pos++] = (uint8_t)ch;
        if (ch == alr->eol) {
            loopIo_dontWantRead(alr->io);
            return ASYNC_COMPLETE;
        }
    } while (alr->pos < alr->buffsize);

    
    loopIo_dontWantRead(alr->io);
    errno = ENOMEM;
    return ASYNC_ERROR;
}

const char*
asyncLineReader_getLineRaw(AsyncLineReader* alr, int *pLength)
{
    if (alr->pos == 0 || alr->pos > alr->buffsize)
        return NULL;

    if (pLength != 0)
        *pLength = alr->pos;

    return (const char*) alr->buffer;
}

const char*
asyncLineReader_getLine(AsyncLineReader* alr)
{
    
    size_t  pos = alr->pos;
    char*   buffer = (char*) alr->buffer;

    if (pos == 0 || pos > alr->buffsize)
        return NULL;

    pos--;

    
    if (alr->eol == '\n') {
        if (buffer[pos] != '\n')
            return NULL;

        buffer[pos] = '\0';

        
        if (pos > 0 && buffer[--pos] == '\r') {
            buffer[pos] = '\0';
        }
    }

    return (const char*) buffer;
}


enum {
    CONNECT_ERROR = 0,
    CONNECT_CONNECTING,
    CONNECT_COMPLETED
};

AsyncStatus
asyncConnector_init(AsyncConnector*    ac,
                    const SockAddress* address,
                    LoopIo*            io)
{
    int ret;
    ac->error = 0;
    ac->io    = io;
    ret = socket_connect(io->fd, address);
    if (ret == 0) {
        ac->state = CONNECT_COMPLETED;
        return ASYNC_COMPLETE;
    }
    if (errno == EINPROGRESS || errno == EWOULDBLOCK || errno == EAGAIN) {
        ac->state = CONNECT_CONNECTING;
        loopIo_wantWrite(io);
        return ASYNC_NEED_MORE;
    }
    ac->error = errno;
    ac->state = CONNECT_ERROR;
    return ASYNC_ERROR;
}

AsyncStatus
asyncConnector_run(AsyncConnector* ac)
{
    switch (ac->state) {
    case CONNECT_ERROR:
        errno = ac->error;
        return ASYNC_ERROR;

    case CONNECT_CONNECTING:
        loopIo_dontWantWrite(ac->io);
        ac->error = socket_get_error(ac->io->fd);
        if (ac->error == 0) {
            ac->state = CONNECT_COMPLETED;
            return ASYNC_COMPLETE;
        }
        ac->state = CONNECT_ERROR;
        errno = ac->error;
        return ASYNC_ERROR;

    default:
        return ASYNC_COMPLETE;
    }
}

int
asyncConnector_stop(AsyncConnector* ac)
{
    if (ac->state == CONNECT_CONNECTING) {
        loopIo_dontWantWrite(ac->io);
        ac->state = CONNECT_COMPLETED;
        return 0;
    }
    return -1;
}
