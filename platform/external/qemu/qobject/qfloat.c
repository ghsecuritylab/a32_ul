/*
 * QFloat Module
 *
 * Copyright IBM, Corp. 2009
 *
 * Authors:
 *  Anthony Liguori   <aliguori@us.ibm.com>
 *
 * This work is licensed under the terms of the GNU LGPL, version 2.1 or later.
 * See the COPYING.LIB file in the top-level directory.
 *
 */

#include "qapi/qmp/qfloat.h"
#include "qapi/qmp/qobject.h"
#include "qemu-common.h"

static void qfloat_destroy_obj(QObject *obj);

static const QType qfloat_type = {
    .code = QTYPE_QFLOAT,
    .destroy = qfloat_destroy_obj,
};

QFloat *qfloat_from_double(double value)
{
    QFloat *qf;

    qf = g_malloc(sizeof(*qf));
    qf->value = value;
    QOBJECT_INIT(qf, &qfloat_type);

    return qf;
}

double qfloat_get_double(const QFloat *qf)
{
    return qf->value;
}

QFloat *qobject_to_qfloat(const QObject *obj)
{
    if (qobject_type(obj) != QTYPE_QFLOAT)
        return NULL;

    return container_of(obj, QFloat, base);
}

static void qfloat_destroy_obj(QObject *obj)
{
    assert(obj != NULL);
    g_free(qobject_to_qfloat(obj));
}