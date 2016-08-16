/*
 * QDict Module
 *
 * Copyright (C) 2009 Red Hat Inc.
 *
 * Authors:
 *  Luiz Capitulino <lcapitulino@redhat.com>
 *
 * This work is licensed under the terms of the GNU LGPL, version 2.1 or later.
 * See the COPYING.LIB file in the top-level directory.
 */

#include "qapi/qmp/qint.h"
#include "qapi/qmp/qfloat.h"
#include "qapi/qmp/qdict.h"
#include "qapi/qmp/qbool.h"
#include "qapi/qmp/qstring.h"
#include "qapi/qmp/qobject.h"
#include "qemu/queue.h"
#include "qemu-common.h"

static void qdict_destroy_obj(QObject *obj);

static const QType qdict_type = {
    .code = QTYPE_QDICT,
    .destroy = qdict_destroy_obj,
};

QDict *qdict_new(void)
{
    QDict *qdict;

    qdict = g_malloc0(sizeof(*qdict));
    QOBJECT_INIT(qdict, &qdict_type);

    return qdict;
}

QDict *qobject_to_qdict(const QObject *obj)
{
    if (qobject_type(obj) != QTYPE_QDICT)
        return NULL;

    return container_of(obj, QDict, base);
}

static unsigned int tdb_hash(const char *name)
{
    unsigned value;	
    unsigned   i;	

    
    for (value = 0x238F13AF * strlen(name), i=0; name[i]; i++)
        value = (value + (((const unsigned char *)name)[i] << (i*5 % 24)));

    return (1103515243 * value + 12345);
}

static QDictEntry *alloc_entry(const char *key, QObject *value)
{
    QDictEntry *entry;

    entry = g_malloc0(sizeof(*entry));
    entry->key = g_strdup(key);
    entry->value = value;

    return entry;
}

QObject *qdict_entry_value(const QDictEntry *entry)
{
    return entry->value;
}

const char *qdict_entry_key(const QDictEntry *entry)
{
    return entry->key;
}

static QDictEntry *qdict_find(const QDict *qdict,
                              const char *key, unsigned int bucket)
{
    QDictEntry *entry;

    QLIST_FOREACH(entry, &qdict->table[bucket], next)
        if (!strcmp(entry->key, key))
            return entry;

    return NULL;
}

void qdict_put_obj(QDict *qdict, const char *key, QObject *value)
{
    unsigned int bucket;
    QDictEntry *entry;

    bucket = tdb_hash(key) % QDICT_BUCKET_MAX;
    entry = qdict_find(qdict, key, bucket);
    if (entry) {
        
        qobject_decref(entry->value);
        entry->value = value;
    } else {
        
        entry = alloc_entry(key, value);
        QLIST_INSERT_HEAD(&qdict->table[bucket], entry, next);
        qdict->size++;
    }
}

QObject *qdict_get(const QDict *qdict, const char *key)
{
    QDictEntry *entry;

    entry = qdict_find(qdict, key, tdb_hash(key) % QDICT_BUCKET_MAX);
    return (entry == NULL ? NULL : entry->value);
}

int qdict_haskey(const QDict *qdict, const char *key)
{
    unsigned int bucket = tdb_hash(key) % QDICT_BUCKET_MAX;
    return (qdict_find(qdict, key, bucket) == NULL ? 0 : 1);
}

size_t qdict_size(const QDict *qdict)
{
    return qdict->size;
}

static QObject *qdict_get_obj(const QDict *qdict, const char *key,
                              qtype_code type)
{
    QObject *obj;

    obj = qdict_get(qdict, key);
    assert(obj != NULL);
    assert(qobject_type(obj) == type);

    return obj;
}

double qdict_get_double(const QDict *qdict, const char *key)
{
    QObject *obj = qdict_get(qdict, key);

    assert(obj);
    switch (qobject_type(obj)) {
    case QTYPE_QFLOAT:
        return qfloat_get_double(qobject_to_qfloat(obj));
    case QTYPE_QINT:
        return qint_get_int(qobject_to_qint(obj));
    default:
        abort();
    }
}

int64_t qdict_get_int(const QDict *qdict, const char *key)
{
    QObject *obj = qdict_get_obj(qdict, key, QTYPE_QINT);
    return qint_get_int(qobject_to_qint(obj));
}

int qdict_get_bool(const QDict *qdict, const char *key)
{
    QObject *obj = qdict_get_obj(qdict, key, QTYPE_QBOOL);
    return qbool_get_int(qobject_to_qbool(obj));
}

QList *qdict_get_qlist(const QDict *qdict, const char *key)
{
    return qobject_to_qlist(qdict_get_obj(qdict, key, QTYPE_QLIST));
}

QDict *qdict_get_qdict(const QDict *qdict, const char *key)
{
    return qobject_to_qdict(qdict_get_obj(qdict, key, QTYPE_QDICT));
}

const char *qdict_get_str(const QDict *qdict, const char *key)
{
    QObject *obj = qdict_get_obj(qdict, key, QTYPE_QSTRING);
    return qstring_get_str(qobject_to_qstring(obj));
}

int64_t qdict_get_try_int(const QDict *qdict, const char *key,
                          int64_t def_value)
{
    QObject *obj;

    obj = qdict_get(qdict, key);
    if (!obj || qobject_type(obj) != QTYPE_QINT)
        return def_value;

    return qint_get_int(qobject_to_qint(obj));
}

int qdict_get_try_bool(const QDict *qdict, const char *key, int def_value)
{
    QObject *obj;

    obj = qdict_get(qdict, key);
    if (!obj || qobject_type(obj) != QTYPE_QBOOL)
        return def_value;

    return qbool_get_int(qobject_to_qbool(obj));
}

const char *qdict_get_try_str(const QDict *qdict, const char *key)
{
    QObject *obj;

    obj = qdict_get(qdict, key);
    if (!obj || qobject_type(obj) != QTYPE_QSTRING)
        return NULL;

    return qstring_get_str(qobject_to_qstring(obj));
}

void qdict_iter(const QDict *qdict,
                void (*iter)(const char *key, QObject *obj, void *opaque),
                void *opaque)
{
    int i;
    QDictEntry *entry;

    for (i = 0; i < QDICT_BUCKET_MAX; i++) {
        QLIST_FOREACH(entry, &qdict->table[i], next)
            iter(entry->key, entry->value, opaque);
    }
}

static QDictEntry *qdict_next_entry(const QDict *qdict, int first_bucket)
{
    int i;

    for (i = first_bucket; i < QDICT_BUCKET_MAX; i++) {
        if (!QLIST_EMPTY(&qdict->table[i])) {
            return QLIST_FIRST(&qdict->table[i]);
        }
    }

    return NULL;
}

const QDictEntry *qdict_first(const QDict *qdict)
{
    return qdict_next_entry(qdict, 0);
}

const QDictEntry *qdict_next(const QDict *qdict, const QDictEntry *entry)
{
    QDictEntry *ret;

    ret = QLIST_NEXT(entry, next);
    if (!ret) {
        unsigned int bucket = tdb_hash(entry->key) % QDICT_BUCKET_MAX;
        ret = qdict_next_entry(qdict, bucket + 1);
    }

    return ret;
}

QDict *qdict_clone_shallow(const QDict *src)
{
    QDict *dest;
    QDictEntry *entry;
    int i;

    dest = qdict_new();

    for (i = 0; i < QDICT_BUCKET_MAX; i++) {
        QLIST_FOREACH(entry, &src->table[i], next) {
            qobject_incref(entry->value);
            qdict_put_obj(dest, entry->key, entry->value);
        }
    }

    return dest;
}

static void qentry_destroy(QDictEntry *e)
{
    assert(e != NULL);
    assert(e->key != NULL);
    assert(e->value != NULL);

    qobject_decref(e->value);
    g_free(e->key);
    g_free(e);
}

void qdict_del(QDict *qdict, const char *key)
{
    QDictEntry *entry;

    entry = qdict_find(qdict, key, tdb_hash(key) % QDICT_BUCKET_MAX);
    if (entry) {
        QLIST_REMOVE(entry, next);
        qentry_destroy(entry);
        qdict->size--;
    }
}

static void qdict_destroy_obj(QObject *obj)
{
    int i;
    QDict *qdict;

    assert(obj != NULL);
    qdict = qobject_to_qdict(obj);

    for (i = 0; i < QDICT_BUCKET_MAX; i++) {
        QDictEntry *entry = QLIST_FIRST(&qdict->table[i]);
        while (entry) {
            QDictEntry *tmp = QLIST_NEXT(entry, next);
            QLIST_REMOVE(entry, next);
            qentry_destroy(entry);
            entry = tmp;
        }
    }

    g_free(qdict);
}

static void qdict_do_flatten(QDict *qdict, QDict *target, const char *prefix)
{
    QObject *value;
    const QDictEntry *entry, *next;
    char *new_key;
    bool delete;

    entry = qdict_first(qdict);

    while (entry != NULL) {

        next = qdict_next(qdict, entry);
        value = qdict_entry_value(entry);
        new_key = NULL;
        delete = false;

        if (prefix) {
            new_key = g_strdup_printf("%s.%s", prefix, entry->key);
        }

        if (qobject_type(value) == QTYPE_QDICT) {
            qdict_do_flatten(qobject_to_qdict(value), target,
                             new_key ? new_key : entry->key);
            delete = true;
        } else if (prefix) {
            
            qobject_incref(value);
            qdict_put_obj(target, new_key, value);
            delete = true;
        }

        g_free(new_key);

        if (delete) {
            qdict_del(qdict, entry->key);

            
            entry = qdict_first(qdict);
            continue;
        }

        entry = next;
    }
}

void qdict_flatten(QDict *qdict)
{
    qdict_do_flatten(qdict, qdict, NULL);
}

void qdict_extract_subqdict(QDict *src, QDict **dst, const char *start)

{
    const QDictEntry *entry, *next;
    const char *p;

    *dst = qdict_new();
    entry = qdict_first(src);

    while (entry != NULL) {
        next = qdict_next(src, entry);
        if (strstart(entry->key, start, &p)) {
            qobject_incref(entry->value);
            qdict_put_obj(*dst, p, entry->value);
            qdict_del(src, entry->key);
        }
        entry = next;
    }
}