#include "Python.h"
#include "structmember.h"
#if PY_VERSION_HEX < 0x02070000 && !defined(PyOS_string_to_double)
#define PyOS_string_to_double json_PyOS_string_to_double
static double
json_PyOS_string_to_double(const char *s, char **endptr, PyObject *overflow_exception);
static double
json_PyOS_string_to_double(const char *s, char **endptr, PyObject *overflow_exception) {
    double x;
    assert(endptr == NULL);
    assert(overflow_exception == NULL);
    PyFPE_START_PROTECT("json_PyOS_string_to_double", return -1.0;)
    x = PyOS_ascii_atof(s);
    PyFPE_END_PROTECT(x)
    return x;
}
#endif
#if PY_VERSION_HEX < 0x02060000 && !defined(Py_TYPE)
#define Py_TYPE(ob)     (((PyObject*)(ob))->ob_type)
#endif
#if PY_VERSION_HEX < 0x02060000 && !defined(Py_SIZE)
#define Py_SIZE(ob)     (((PyVarObject*)(ob))->ob_size)
#endif
#if PY_VERSION_HEX < 0x02050000 && !defined(PY_SSIZE_T_MIN)
typedef int Py_ssize_t;
#define PY_SSIZE_T_MAX INT_MAX
#define PY_SSIZE_T_MIN INT_MIN
#define PyInt_FromSsize_t PyInt_FromLong
#define PyInt_AsSsize_t PyInt_AsLong
#endif
#ifndef Py_IS_FINITE
#define Py_IS_FINITE(X) (!Py_IS_INFINITY(X) && !Py_IS_NAN(X))
#endif

#ifdef __GNUC__
#define UNUSED __attribute__((__unused__))
#else
#define UNUSED
#endif

#define DEFAULT_ENCODING "utf-8"

#define PyScanner_Check(op) PyObject_TypeCheck(op, &PyScannerType)
#define PyScanner_CheckExact(op) (Py_TYPE(op) == &PyScannerType)
#define PyEncoder_Check(op) PyObject_TypeCheck(op, &PyEncoderType)
#define PyEncoder_CheckExact(op) (Py_TYPE(op) == &PyEncoderType)

static PyTypeObject PyScannerType;
static PyTypeObject PyEncoderType;

typedef struct _PyScannerObject {
    PyObject_HEAD
    PyObject *encoding;
    PyObject *strict;
    PyObject *object_hook;
    PyObject *pairs_hook;
    PyObject *parse_float;
    PyObject *parse_int;
    PyObject *parse_constant;
    PyObject *memo;
} PyScannerObject;

static PyMemberDef scanner_members[] = {
    {"encoding", T_OBJECT, offsetof(PyScannerObject, encoding), READONLY, "encoding"},
    {"strict", T_OBJECT, offsetof(PyScannerObject, strict), READONLY, "strict"},
    {"object_hook", T_OBJECT, offsetof(PyScannerObject, object_hook), READONLY, "object_hook"},
    {"object_pairs_hook", T_OBJECT, offsetof(PyScannerObject, pairs_hook), READONLY, "object_pairs_hook"},
    {"parse_float", T_OBJECT, offsetof(PyScannerObject, parse_float), READONLY, "parse_float"},
    {"parse_int", T_OBJECT, offsetof(PyScannerObject, parse_int), READONLY, "parse_int"},
    {"parse_constant", T_OBJECT, offsetof(PyScannerObject, parse_constant), READONLY, "parse_constant"},
    {NULL}
};

typedef struct _PyEncoderObject {
    PyObject_HEAD
    PyObject *markers;
    PyObject *defaultfn;
    PyObject *encoder;
    PyObject *indent;
    PyObject *key_separator;
    PyObject *item_separator;
    PyObject *sort_keys;
    PyObject *skipkeys;
    PyObject *key_memo;
    PyObject *Decimal;
    int fast_encode;
    int allow_nan;
    int use_decimal;
    int namedtuple_as_object;
    int tuple_as_array;
    int bigint_as_string;
    PyObject *item_sort_key;
} PyEncoderObject;

static PyMemberDef encoder_members[] = {
    {"markers", T_OBJECT, offsetof(PyEncoderObject, markers), READONLY, "markers"},
    {"default", T_OBJECT, offsetof(PyEncoderObject, defaultfn), READONLY, "default"},
    {"encoder", T_OBJECT, offsetof(PyEncoderObject, encoder), READONLY, "encoder"},
    {"indent", T_OBJECT, offsetof(PyEncoderObject, indent), READONLY, "indent"},
    {"key_separator", T_OBJECT, offsetof(PyEncoderObject, key_separator), READONLY, "key_separator"},
    {"item_separator", T_OBJECT, offsetof(PyEncoderObject, item_separator), READONLY, "item_separator"},
    {"sort_keys", T_OBJECT, offsetof(PyEncoderObject, sort_keys), READONLY, "sort_keys"},
    {"skipkeys", T_OBJECT, offsetof(PyEncoderObject, skipkeys), READONLY, "skipkeys"},
    {"key_memo", T_OBJECT, offsetof(PyEncoderObject, key_memo), READONLY, "key_memo"},
    {"item_sort_key", T_OBJECT, offsetof(PyEncoderObject, item_sort_key), READONLY, "item_sort_key"},
    {NULL}
};

static PyObject *
maybe_quote_bigint(PyObject *encoded, PyObject *obj);

static Py_ssize_t
ascii_escape_char(Py_UNICODE c, char *output, Py_ssize_t chars);
static PyObject *
ascii_escape_unicode(PyObject *pystr);
static PyObject *
ascii_escape_str(PyObject *pystr);
static PyObject *
py_encode_basestring_ascii(PyObject* self UNUSED, PyObject *pystr);
void init_speedups(void);
static PyObject *
scan_once_str(PyScannerObject *s, PyObject *pystr, Py_ssize_t idx, Py_ssize_t *next_idx_ptr);
static PyObject *
scan_once_unicode(PyScannerObject *s, PyObject *pystr, Py_ssize_t idx, Py_ssize_t *next_idx_ptr);
static PyObject *
_build_rval_index_tuple(PyObject *rval, Py_ssize_t idx);
static PyObject *
scanner_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static int
scanner_init(PyObject *self, PyObject *args, PyObject *kwds);
static void
scanner_dealloc(PyObject *self);
static int
scanner_clear(PyObject *self);
static PyObject *
encoder_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
static int
encoder_init(PyObject *self, PyObject *args, PyObject *kwds);
static void
encoder_dealloc(PyObject *self);
static int
encoder_clear(PyObject *self);
static int
encoder_listencode_list(PyEncoderObject *s, PyObject *rval, PyObject *seq, Py_ssize_t indent_level);
static int
encoder_listencode_obj(PyEncoderObject *s, PyObject *rval, PyObject *obj, Py_ssize_t indent_level);
static int
encoder_listencode_dict(PyEncoderObject *s, PyObject *rval, PyObject *dct, Py_ssize_t indent_level);
static PyObject *
_encoded_const(PyObject *obj);
static void
raise_errmsg(char *msg, PyObject *s, Py_ssize_t end);
static PyObject *
encoder_encode_string(PyEncoderObject *s, PyObject *obj);
static int
_convertPyInt_AsSsize_t(PyObject *o, Py_ssize_t *size_ptr);
static PyObject *
_convertPyInt_FromSsize_t(Py_ssize_t *size_ptr);
static PyObject *
encoder_encode_float(PyEncoderObject *s, PyObject *obj);
static int
_is_namedtuple(PyObject *obj);

#define S_CHAR(c) (c >= ' ' && c <= '~' && c != '\\' && c != '"')
#define IS_WHITESPACE(c) (((c) == ' ') || ((c) == '\t') || ((c) == '\n') || ((c) == '\r'))

#define MIN_EXPANSION 6
#ifdef Py_UNICODE_WIDE
#define MAX_EXPANSION (2 * MIN_EXPANSION)
#else
#define MAX_EXPANSION MIN_EXPANSION
#endif

static PyObject *
maybe_quote_bigint(PyObject *encoded, PyObject *obj)
{
    static PyObject *big_long = NULL;
    static PyObject *small_long = NULL;
    if (big_long == NULL) {
        big_long = PyLong_FromLongLong(1LL << 53);
        if (big_long == NULL) {
            Py_DECREF(encoded);
            return NULL;
        }
    }
    if (small_long == NULL) {
        small_long = PyLong_FromLongLong(-1LL << 53);
        if (small_long == NULL) {
            Py_DECREF(encoded);
            return NULL;
        }
    }
    if (PyObject_RichCompareBool(obj, big_long, Py_GE) ||
        PyObject_RichCompareBool(obj, small_long, Py_LE)) {
        PyObject* quoted = PyString_FromFormat("\"%s\"",
                                               PyString_AsString(encoded));
        Py_DECREF(encoded);
        encoded = quoted;
    }
    return encoded;
}

static int
_is_namedtuple(PyObject *obj)
{
    int rval = 0;
    PyObject *_asdict = PyObject_GetAttrString(obj, "_asdict");
    if (_asdict == NULL) {
        PyErr_Clear();
        return 0;
    }
    rval = PyCallable_Check(_asdict);
    Py_DECREF(_asdict);
    return rval;
}

static int
_convertPyInt_AsSsize_t(PyObject *o, Py_ssize_t *size_ptr)
{
    
    *size_ptr = PyInt_AsSsize_t(o);
    if (*size_ptr == -1 && PyErr_Occurred())
        return 0;
    return 1;
}

static PyObject *
_convertPyInt_FromSsize_t(Py_ssize_t *size_ptr)
{
    
    return PyInt_FromSsize_t(*size_ptr);
}

static Py_ssize_t
ascii_escape_char(Py_UNICODE c, char *output, Py_ssize_t chars)
{
    output[chars++] = '\\';
    switch (c) {
        case '\\': output[chars++] = (char)c; break;
        case '"': output[chars++] = (char)c; break;
        case '\b': output[chars++] = 'b'; break;
        case '\f': output[chars++] = 'f'; break;
        case '\n': output[chars++] = 'n'; break;
        case '\r': output[chars++] = 'r'; break;
        case '\t': output[chars++] = 't'; break;
        default:
#ifdef Py_UNICODE_WIDE
            if (c >= 0x10000) {
                
                Py_UNICODE v = c - 0x10000;
                c = 0xd800 | ((v >> 10) & 0x3ff);
                output[chars++] = 'u';
                output[chars++] = "0123456789abcdef"[(c >> 12) & 0xf];
                output[chars++] = "0123456789abcdef"[(c >>  8) & 0xf];
                output[chars++] = "0123456789abcdef"[(c >>  4) & 0xf];
                output[chars++] = "0123456789abcdef"[(c      ) & 0xf];
                c = 0xdc00 | (v & 0x3ff);
                output[chars++] = '\\';
            }
#endif
            output[chars++] = 'u';
            output[chars++] = "0123456789abcdef"[(c >> 12) & 0xf];
            output[chars++] = "0123456789abcdef"[(c >>  8) & 0xf];
            output[chars++] = "0123456789abcdef"[(c >>  4) & 0xf];
            output[chars++] = "0123456789abcdef"[(c      ) & 0xf];
    }
    return chars;
}

static PyObject *
ascii_escape_unicode(PyObject *pystr)
{
    
    Py_ssize_t i;
    Py_ssize_t input_chars;
    Py_ssize_t output_size;
    Py_ssize_t max_output_size;
    Py_ssize_t chars;
    PyObject *rval;
    char *output;
    Py_UNICODE *input_unicode;

    input_chars = PyUnicode_GET_SIZE(pystr);
    input_unicode = PyUnicode_AS_UNICODE(pystr);

    
    output_size = 2 + (MIN_EXPANSION * 4) + input_chars;
    max_output_size = 2 + (input_chars * MAX_EXPANSION);
    rval = PyString_FromStringAndSize(NULL, output_size);
    if (rval == NULL) {
        return NULL;
    }
    output = PyString_AS_STRING(rval);
    chars = 0;
    output[chars++] = '"';
    for (i = 0; i < input_chars; i++) {
        Py_UNICODE c = input_unicode[i];
        if (S_CHAR(c)) {
            output[chars++] = (char)c;
        }
        else {
            chars = ascii_escape_char(c, output, chars);
        }
        if (output_size - chars < (1 + MAX_EXPANSION)) {
            
            Py_ssize_t new_output_size = output_size * 2;
            
            if (new_output_size > max_output_size) {
                new_output_size = max_output_size;
            }
            
            if (new_output_size != output_size) {
                output_size = new_output_size;
                if (_PyString_Resize(&rval, output_size) == -1) {
                    return NULL;
                }
                output = PyString_AS_STRING(rval);
            }
        }
    }
    output[chars++] = '"';
    if (_PyString_Resize(&rval, chars) == -1) {
        return NULL;
    }
    return rval;
}

static PyObject *
ascii_escape_str(PyObject *pystr)
{
    
    Py_ssize_t i;
    Py_ssize_t input_chars;
    Py_ssize_t output_size;
    Py_ssize_t chars;
    PyObject *rval;
    char *output;
    char *input_str;

    input_chars = PyString_GET_SIZE(pystr);
    input_str = PyString_AS_STRING(pystr);

    
    for (i = 0; i < input_chars; i++) {
        Py_UNICODE c = (Py_UNICODE)(unsigned char)input_str[i];
        if (!S_CHAR(c)) {
            
            Py_ssize_t j;
            for (j = i; j < input_chars; j++) {
                c = (Py_UNICODE)(unsigned char)input_str[j];
                if (c > 0x7f) {
                    
                    PyObject *uni;
                    uni = PyUnicode_DecodeUTF8(input_str, input_chars, "strict");
                    if (uni == NULL) {
                        return NULL;
                    }
                    rval = ascii_escape_unicode(uni);
                    Py_DECREF(uni);
                    return rval;
                }
            }
            break;
        }
    }

    if (i == input_chars) {
        
        output_size = 2 + input_chars;
    }
    else {
        
        output_size = 2 + (MIN_EXPANSION * 4) + input_chars;
    }
    rval = PyString_FromStringAndSize(NULL, output_size);
    if (rval == NULL) {
        return NULL;
    }
    output = PyString_AS_STRING(rval);
    output[0] = '"';

    
    chars = i + 1;
    memcpy(&output[1], input_str, i);

    for (; i < input_chars; i++) {
        Py_UNICODE c = (Py_UNICODE)(unsigned char)input_str[i];
        if (S_CHAR(c)) {
            output[chars++] = (char)c;
        }
        else {
            chars = ascii_escape_char(c, output, chars);
        }
        
        if (output_size - chars < (1 + MIN_EXPANSION)) {
            
            output_size *= 2;
            if (output_size > 2 + (input_chars * MIN_EXPANSION)) {
                output_size = 2 + (input_chars * MIN_EXPANSION);
            }
            if (_PyString_Resize(&rval, output_size) == -1) {
                return NULL;
            }
            output = PyString_AS_STRING(rval);
        }
    }
    output[chars++] = '"';
    if (_PyString_Resize(&rval, chars) == -1) {
        return NULL;
    }
    return rval;
}

static void
raise_errmsg(char *msg, PyObject *s, Py_ssize_t end)
{
    static PyObject *JSONDecodeError = NULL;
    PyObject *exc;
    if (JSONDecodeError == NULL) {
        PyObject *decoder = PyImport_ImportModule("simplejson.decoder");
        if (decoder == NULL)
            return;
        JSONDecodeError = PyObject_GetAttrString(decoder, "JSONDecodeError");
        Py_DECREF(decoder);
        if (JSONDecodeError == NULL)
            return;
    }
    exc = PyObject_CallFunction(JSONDecodeError, "(zOO&)", msg, s, _convertPyInt_FromSsize_t, &end);
    if (exc) {
        PyErr_SetObject(JSONDecodeError, exc);
        Py_DECREF(exc);
    }
}

static PyObject *
join_list_unicode(PyObject *lst)
{
    
    static PyObject *joinfn = NULL;
    if (joinfn == NULL) {
        PyObject *ustr = PyUnicode_FromUnicode(NULL, 0);
        if (ustr == NULL)
            return NULL;

        joinfn = PyObject_GetAttrString(ustr, "join");
        Py_DECREF(ustr);
        if (joinfn == NULL)
            return NULL;
    }
    return PyObject_CallFunctionObjArgs(joinfn, lst, NULL);
}

static PyObject *
join_list_string(PyObject *lst)
{
    
    static PyObject *joinfn = NULL;
    if (joinfn == NULL) {
        PyObject *ustr = PyString_FromStringAndSize(NULL, 0);
        if (ustr == NULL)
            return NULL;

        joinfn = PyObject_GetAttrString(ustr, "join");
        Py_DECREF(ustr);
        if (joinfn == NULL)
            return NULL;
    }
    return PyObject_CallFunctionObjArgs(joinfn, lst, NULL);
}

static PyObject *
_build_rval_index_tuple(PyObject *rval, Py_ssize_t idx) {
    
    PyObject *tpl;
    PyObject *pyidx;
    if (rval == NULL) {
        return NULL;
    }
    pyidx = PyInt_FromSsize_t(idx);
    if (pyidx == NULL) {
        Py_DECREF(rval);
        return NULL;
    }
    tpl = PyTuple_New(2);
    if (tpl == NULL) {
        Py_DECREF(pyidx);
        Py_DECREF(rval);
        return NULL;
    }
    PyTuple_SET_ITEM(tpl, 0, rval);
    PyTuple_SET_ITEM(tpl, 1, pyidx);
    return tpl;
}

#define APPEND_OLD_CHUNK \
    if (chunk != NULL) { \
        if (chunks == NULL) { \
            chunks = PyList_New(0); \
            if (chunks == NULL) { \
                goto bail; \
            } \
        } \
        if (PyList_Append(chunks, chunk)) { \
            goto bail; \
        } \
        Py_CLEAR(chunk); \
    }

static PyObject *
scanstring_str(PyObject *pystr, Py_ssize_t end, char *encoding, int strict, Py_ssize_t *next_end_ptr)
{
    PyObject *rval;
    Py_ssize_t len = PyString_GET_SIZE(pystr);
    Py_ssize_t begin = end - 1;
    Py_ssize_t next = begin;
    int has_unicode = 0;
    char *buf = PyString_AS_STRING(pystr);
    PyObject *chunks = NULL;
    PyObject *chunk = NULL;

    if (len == end) {
        raise_errmsg("Unterminated string starting at", pystr, begin);
    }
    else if (end < 0 || len < end) {
        PyErr_SetString(PyExc_ValueError, "end is out of bounds");
        goto bail;
    }
    while (1) {
        
        Py_UNICODE c = 0;
        for (next = end; next < len; next++) {
            c = (unsigned char)buf[next];
            if (c == '"' || c == '\\') {
                break;
            }
            else if (strict && c <= 0x1f) {
                raise_errmsg("Invalid control character at", pystr, next);
                goto bail;
            }
            else if (c > 0x7f) {
                has_unicode = 1;
            }
        }
        if (!(c == '"' || c == '\\')) {
            raise_errmsg("Unterminated string starting at", pystr, begin);
            goto bail;
        }
        
        if (next != end) {
            PyObject *strchunk;
            APPEND_OLD_CHUNK
            strchunk = PyString_FromStringAndSize(&buf[end], next - end);
            if (strchunk == NULL) {
                goto bail;
            }
            if (has_unicode) {
                chunk = PyUnicode_FromEncodedObject(strchunk, encoding, NULL);
                Py_DECREF(strchunk);
                if (chunk == NULL) {
                    goto bail;
                }
            }
            else {
                chunk = strchunk;
            }
        }
        next++;
        if (c == '"') {
            end = next;
            break;
        }
        if (next == len) {
            raise_errmsg("Unterminated string starting at", pystr, begin);
            goto bail;
        }
        c = buf[next];
        if (c != 'u') {
            
            end = next + 1;
            switch (c) {
                case '"': break;
                case '\\': break;
                case '/': break;
                case 'b': c = '\b'; break;
                case 'f': c = '\f'; break;
                case 'n': c = '\n'; break;
                case 'r': c = '\r'; break;
                case 't': c = '\t'; break;
                default: c = 0;
            }
            if (c == 0) {
                raise_errmsg("Invalid \\escape", pystr, end - 2);
                goto bail;
            }
        }
        else {
            c = 0;
            next++;
            end = next + 4;
            if (end >= len) {
                raise_errmsg("Invalid \\uXXXX escape", pystr, next - 1);
                goto bail;
            }
            
            for (; next < end; next++) {
                Py_UNICODE digit = buf[next];
                c <<= 4;
                switch (digit) {
                    case '0': case '1': case '2': case '3': case '4':
                    case '5': case '6': case '7': case '8': case '9':
                        c |= (digit - '0'); break;
                    case 'a': case 'b': case 'c': case 'd': case 'e':
                    case 'f':
                        c |= (digit - 'a' + 10); break;
                    case 'A': case 'B': case 'C': case 'D': case 'E':
                    case 'F':
                        c |= (digit - 'A' + 10); break;
                    default:
                        raise_errmsg("Invalid \\uXXXX escape", pystr, end - 5);
                        goto bail;
                }
            }
#ifdef Py_UNICODE_WIDE
            
            if ((c & 0xfc00) == 0xd800) {
                Py_UNICODE c2 = 0;
                if (end + 6 >= len) {
                    raise_errmsg("Unpaired high surrogate", pystr, end - 5);
                    goto bail;
                }
                if (buf[next++] != '\\' || buf[next++] != 'u') {
                    raise_errmsg("Unpaired high surrogate", pystr, end - 5);
                    goto bail;
                }
                end += 6;
                
                for (; next < end; next++) {
                    c2 <<= 4;
                    Py_UNICODE digit = buf[next];
                    switch (digit) {
                        case '0': case '1': case '2': case '3': case '4':
                        case '5': case '6': case '7': case '8': case '9':
                            c2 |= (digit - '0'); break;
                        case 'a': case 'b': case 'c': case 'd': case 'e':
                        case 'f':
                            c2 |= (digit - 'a' + 10); break;
                        case 'A': case 'B': case 'C': case 'D': case 'E':
                        case 'F':
                            c2 |= (digit - 'A' + 10); break;
                        default:
                            raise_errmsg("Invalid \\uXXXX escape", pystr, end - 5);
                            goto bail;
                    }
                }
                if ((c2 & 0xfc00) != 0xdc00) {
                    raise_errmsg("Unpaired high surrogate", pystr, end - 5);
                    goto bail;
                }
                c = 0x10000 + (((c - 0xd800) << 10) | (c2 - 0xdc00));
            }
            else if ((c & 0xfc00) == 0xdc00) {
                raise_errmsg("Unpaired low surrogate", pystr, end - 5);
                goto bail;
            }
#endif
        }
        if (c > 0x7f) {
            has_unicode = 1;
        }
        APPEND_OLD_CHUNK
        if (has_unicode) {
            chunk = PyUnicode_FromUnicode(&c, 1);
            if (chunk == NULL) {
                goto bail;
            }
        }
        else {
            char c_char = Py_CHARMASK(c);
            chunk = PyString_FromStringAndSize(&c_char, 1);
            if (chunk == NULL) {
                goto bail;
            }
        }
    }

    if (chunks == NULL) {
        if (chunk != NULL)
            rval = chunk;
        else
            rval = PyString_FromStringAndSize("", 0);
    }
    else {
        APPEND_OLD_CHUNK
        rval = join_list_string(chunks);
        if (rval == NULL) {
            goto bail;
        }
        Py_CLEAR(chunks);
    }

    *next_end_ptr = end;
    return rval;
bail:
    *next_end_ptr = -1;
    Py_XDECREF(chunk);
    Py_XDECREF(chunks);
    return NULL;
}


static PyObject *
scanstring_unicode(PyObject *pystr, Py_ssize_t end, int strict, Py_ssize_t *next_end_ptr)
{
    PyObject *rval;
    Py_ssize_t len = PyUnicode_GET_SIZE(pystr);
    Py_ssize_t begin = end - 1;
    Py_ssize_t next = begin;
    const Py_UNICODE *buf = PyUnicode_AS_UNICODE(pystr);
    PyObject *chunks = NULL;
    PyObject *chunk = NULL;

    if (len == end) {
        raise_errmsg("Unterminated string starting at", pystr, begin);
    }
    else if (end < 0 || len < end) {
        PyErr_SetString(PyExc_ValueError, "end is out of bounds");
        goto bail;
    }
    while (1) {
        
        Py_UNICODE c = 0;
        for (next = end; next < len; next++) {
            c = buf[next];
            if (c == '"' || c == '\\') {
                break;
            }
            else if (strict && c <= 0x1f) {
                raise_errmsg("Invalid control character at", pystr, next);
                goto bail;
            }
        }
        if (!(c == '"' || c == '\\')) {
            raise_errmsg("Unterminated string starting at", pystr, begin);
            goto bail;
        }
        
        if (next != end) {
            APPEND_OLD_CHUNK
            chunk = PyUnicode_FromUnicode(&buf[end], next - end);
            if (chunk == NULL) {
                goto bail;
            }
        }
        next++;
        if (c == '"') {
            end = next;
            break;
        }
        if (next == len) {
            raise_errmsg("Unterminated string starting at", pystr, begin);
            goto bail;
        }
        c = buf[next];
        if (c != 'u') {
            
            end = next + 1;
            switch (c) {
                case '"': break;
                case '\\': break;
                case '/': break;
                case 'b': c = '\b'; break;
                case 'f': c = '\f'; break;
                case 'n': c = '\n'; break;
                case 'r': c = '\r'; break;
                case 't': c = '\t'; break;
                default: c = 0;
            }
            if (c == 0) {
                raise_errmsg("Invalid \\escape", pystr, end - 2);
                goto bail;
            }
        }
        else {
            c = 0;
            next++;
            end = next + 4;
            if (end >= len) {
                raise_errmsg("Invalid \\uXXXX escape", pystr, next - 1);
                goto bail;
            }
            
            for (; next < end; next++) {
                Py_UNICODE digit = buf[next];
                c <<= 4;
                switch (digit) {
                    case '0': case '1': case '2': case '3': case '4':
                    case '5': case '6': case '7': case '8': case '9':
                        c |= (digit - '0'); break;
                    case 'a': case 'b': case 'c': case 'd': case 'e':
                    case 'f':
                        c |= (digit - 'a' + 10); break;
                    case 'A': case 'B': case 'C': case 'D': case 'E':
                    case 'F':
                        c |= (digit - 'A' + 10); break;
                    default:
                        raise_errmsg("Invalid \\uXXXX escape", pystr, end - 5);
                        goto bail;
                }
            }
#ifdef Py_UNICODE_WIDE
            
            if ((c & 0xfc00) == 0xd800) {
                Py_UNICODE c2 = 0;
                if (end + 6 >= len) {
                    raise_errmsg("Unpaired high surrogate", pystr, end - 5);
                    goto bail;
                }
                if (buf[next++] != '\\' || buf[next++] != 'u') {
                    raise_errmsg("Unpaired high surrogate", pystr, end - 5);
                    goto bail;
                }
                end += 6;
                
                for (; next < end; next++) {
                    c2 <<= 4;
                    Py_UNICODE digit = buf[next];
                    switch (digit) {
                        case '0': case '1': case '2': case '3': case '4':
                        case '5': case '6': case '7': case '8': case '9':
                            c2 |= (digit - '0'); break;
                        case 'a': case 'b': case 'c': case 'd': case 'e':
                        case 'f':
                            c2 |= (digit - 'a' + 10); break;
                        case 'A': case 'B': case 'C': case 'D': case 'E':
                        case 'F':
                            c2 |= (digit - 'A' + 10); break;
                        default:
                            raise_errmsg("Invalid \\uXXXX escape", pystr, end - 5);
                            goto bail;
                    }
                }
                if ((c2 & 0xfc00) != 0xdc00) {
                    raise_errmsg("Unpaired high surrogate", pystr, end - 5);
                    goto bail;
                }
                c = 0x10000 + (((c - 0xd800) << 10) | (c2 - 0xdc00));
            }
            else if ((c & 0xfc00) == 0xdc00) {
                raise_errmsg("Unpaired low surrogate", pystr, end - 5);
                goto bail;
            }
#endif
        }
        APPEND_OLD_CHUNK
        chunk = PyUnicode_FromUnicode(&c, 1);
        if (chunk == NULL) {
            goto bail;
        }
    }

    if (chunks == NULL) {
        if (chunk != NULL)
            rval = chunk;
        else
            rval = PyUnicode_FromUnicode(NULL, 0);
    }
    else {
        APPEND_OLD_CHUNK
        rval = join_list_unicode(chunks);
        if (rval == NULL) {
            goto bail;
        }
        Py_CLEAR(chunks);
    }
    *next_end_ptr = end;
    return rval;
bail:
    *next_end_ptr = -1;
    Py_XDECREF(chunk);
    Py_XDECREF(chunks);
    return NULL;
}

PyDoc_STRVAR(pydoc_scanstring,
    "scanstring(basestring, end, encoding, strict=True) -> (str, end)\n"
    "\n"
    "Scan the string s for a JSON string. End is the index of the\n"
    "character in s after the quote that started the JSON string.\n"
    "Unescapes all valid JSON string escape sequences and raises ValueError\n"
    "on attempt to decode an invalid string. If strict is False then literal\n"
    "control characters are allowed in the string.\n"
    "\n"
    "Returns a tuple of the decoded string and the index of the character in s\n"
    "after the end quote."
);

static PyObject *
py_scanstring(PyObject* self UNUSED, PyObject *args)
{
    PyObject *pystr;
    PyObject *rval;
    Py_ssize_t end;
    Py_ssize_t next_end = -1;
    char *encoding = NULL;
    int strict = 1;
    if (!PyArg_ParseTuple(args, "OO&|zi:scanstring", &pystr, _convertPyInt_AsSsize_t, &end, &encoding, &strict)) {
        return NULL;
    }
    if (encoding == NULL) {
        encoding = DEFAULT_ENCODING;
    }
    if (PyString_Check(pystr)) {
        rval = scanstring_str(pystr, end, encoding, strict, &next_end);
    }
    else if (PyUnicode_Check(pystr)) {
        rval = scanstring_unicode(pystr, end, strict, &next_end);
    }
    else {
        PyErr_Format(PyExc_TypeError,
                     "first argument must be a string, not %.80s",
                     Py_TYPE(pystr)->tp_name);
        return NULL;
    }
    return _build_rval_index_tuple(rval, next_end);
}

PyDoc_STRVAR(pydoc_encode_basestring_ascii,
    "encode_basestring_ascii(basestring) -> str\n"
    "\n"
    "Return an ASCII-only JSON representation of a Python string"
);

static PyObject *
py_encode_basestring_ascii(PyObject* self UNUSED, PyObject *pystr)
{
    
    
    if (PyString_Check(pystr)) {
        return ascii_escape_str(pystr);
    }
    else if (PyUnicode_Check(pystr)) {
        return ascii_escape_unicode(pystr);
    }
    else {
        PyErr_Format(PyExc_TypeError,
                     "first argument must be a string, not %.80s",
                     Py_TYPE(pystr)->tp_name);
        return NULL;
    }
}

static void
scanner_dealloc(PyObject *self)
{
    
    scanner_clear(self);
    Py_TYPE(self)->tp_free(self);
}

static int
scanner_traverse(PyObject *self, visitproc visit, void *arg)
{
    PyScannerObject *s;
    assert(PyScanner_Check(self));
    s = (PyScannerObject *)self;
    Py_VISIT(s->encoding);
    Py_VISIT(s->strict);
    Py_VISIT(s->object_hook);
    Py_VISIT(s->pairs_hook);
    Py_VISIT(s->parse_float);
    Py_VISIT(s->parse_int);
    Py_VISIT(s->parse_constant);
    Py_VISIT(s->memo);
    return 0;
}

static int
scanner_clear(PyObject *self)
{
    PyScannerObject *s;
    assert(PyScanner_Check(self));
    s = (PyScannerObject *)self;
    Py_CLEAR(s->encoding);
    Py_CLEAR(s->strict);
    Py_CLEAR(s->object_hook);
    Py_CLEAR(s->pairs_hook);
    Py_CLEAR(s->parse_float);
    Py_CLEAR(s->parse_int);
    Py_CLEAR(s->parse_constant);
    Py_CLEAR(s->memo);
    return 0;
}

static PyObject *
_parse_object_str(PyScannerObject *s, PyObject *pystr, Py_ssize_t idx, Py_ssize_t *next_idx_ptr) {
    char *str = PyString_AS_STRING(pystr);
    Py_ssize_t end_idx = PyString_GET_SIZE(pystr) - 1;
    PyObject *rval = NULL;
    PyObject *pairs = NULL;
    PyObject *item;
    PyObject *key = NULL;
    PyObject *val = NULL;
    char *encoding = PyString_AS_STRING(s->encoding);
    int strict = PyObject_IsTrue(s->strict);
    int has_pairs_hook = (s->pairs_hook != Py_None);
    Py_ssize_t next_idx;
    if (has_pairs_hook) {
        pairs = PyList_New(0);
        if (pairs == NULL)
            return NULL;
    }
    else {
        rval = PyDict_New();
        if (rval == NULL)
            return NULL;
    }

    
    while (idx <= end_idx && IS_WHITESPACE(str[idx])) idx++;

    
    if (idx <= end_idx && str[idx] != '}') {
        while (idx <= end_idx) {
            PyObject *memokey;

            
            if (str[idx] != '"') {
                raise_errmsg(
                    "Expecting property name enclosed in double quotes",
                    pystr, idx);
                goto bail;
            }
            key = scanstring_str(pystr, idx + 1, encoding, strict, &next_idx);
            if (key == NULL)
                goto bail;
            memokey = PyDict_GetItem(s->memo, key);
            if (memokey != NULL) {
                Py_INCREF(memokey);
                Py_DECREF(key);
                key = memokey;
            }
            else {
                if (PyDict_SetItem(s->memo, key, key) < 0)
                    goto bail;
            }
            idx = next_idx;

            
            while (idx <= end_idx && IS_WHITESPACE(str[idx])) idx++;
            if (idx > end_idx || str[idx] != ':') {
                raise_errmsg("Expecting ':' delimiter", pystr, idx);
                goto bail;
            }
            idx++;
            while (idx <= end_idx && IS_WHITESPACE(str[idx])) idx++;

            
            val = scan_once_str(s, pystr, idx, &next_idx);
            if (val == NULL)
                goto bail;

            if (has_pairs_hook) {
                item = PyTuple_Pack(2, key, val);
                if (item == NULL)
                    goto bail;
                Py_CLEAR(key);
                Py_CLEAR(val);
                if (PyList_Append(pairs, item) == -1) {
                    Py_DECREF(item);
                    goto bail;
                }
                Py_DECREF(item);
            }
            else {
                if (PyDict_SetItem(rval, key, val) < 0)
                    goto bail;
                Py_CLEAR(key);
                Py_CLEAR(val);
            }
            idx = next_idx;

            
            while (idx <= end_idx && IS_WHITESPACE(str[idx])) idx++;

            
            if (idx > end_idx) break;
            if (str[idx] == '}') {
                break;
            }
            else if (str[idx] != ',') {
                raise_errmsg("Expecting ',' delimiter", pystr, idx);
                goto bail;
            }
            idx++;

            
            while (idx <= end_idx && IS_WHITESPACE(str[idx])) idx++;
        }
    }
    
    if (idx > end_idx || str[idx] != '}') {
        raise_errmsg("Expecting object", pystr, end_idx);
        goto bail;
    }

    
    if (s->pairs_hook != Py_None) {
        val = PyObject_CallFunctionObjArgs(s->pairs_hook, pairs, NULL);
        if (val == NULL)
            goto bail;
        Py_DECREF(pairs);
        *next_idx_ptr = idx + 1;
        return val;
    }

    
    if (s->object_hook != Py_None) {
        val = PyObject_CallFunctionObjArgs(s->object_hook, rval, NULL);
        if (val == NULL)
            goto bail;
        Py_DECREF(rval);
        rval = val;
        val = NULL;
    }
    *next_idx_ptr = idx + 1;
    return rval;
bail:
    Py_XDECREF(rval);
    Py_XDECREF(key);
    Py_XDECREF(val);
    Py_XDECREF(pairs);
    return NULL;
}

static PyObject *
_parse_object_unicode(PyScannerObject *s, PyObject *pystr, Py_ssize_t idx, Py_ssize_t *next_idx_ptr) {
    Py_UNICODE *str = PyUnicode_AS_UNICODE(pystr);
    Py_ssize_t end_idx = PyUnicode_GET_SIZE(pystr) - 1;
    PyObject *rval = NULL;
    PyObject *pairs = NULL;
    PyObject *item;
    PyObject *key = NULL;
    PyObject *val = NULL;
    int strict = PyObject_IsTrue(s->strict);
    int has_pairs_hook = (s->pairs_hook != Py_None);
    Py_ssize_t next_idx;

    if (has_pairs_hook) {
        pairs = PyList_New(0);
        if (pairs == NULL)
            return NULL;
    }
    else {
        rval = PyDict_New();
        if (rval == NULL)
            return NULL;
    }

    
    while (idx <= end_idx && IS_WHITESPACE(str[idx])) idx++;

    
    if (idx <= end_idx && str[idx] != '}') {
        while (idx <= end_idx) {
            PyObject *memokey;

            
            if (str[idx] != '"') {
                raise_errmsg(
                    "Expecting property name enclosed in double quotes",
                    pystr, idx);
                goto bail;
            }
            key = scanstring_unicode(pystr, idx + 1, strict, &next_idx);
            if (key == NULL)
                goto bail;
            memokey = PyDict_GetItem(s->memo, key);
            if (memokey != NULL) {
                Py_INCREF(memokey);
                Py_DECREF(key);
                key = memokey;
            }
            else {
                if (PyDict_SetItem(s->memo, key, key) < 0)
                    goto bail;
            }
            idx = next_idx;

            while (idx <= end_idx && IS_WHITESPACE(str[idx])) idx++;
            if (idx > end_idx || str[idx] != ':') {
                raise_errmsg("Expecting ':' delimiter", pystr, idx);
                goto bail;
            }
            idx++;
            while (idx <= end_idx && IS_WHITESPACE(str[idx])) idx++;

            
            val = scan_once_unicode(s, pystr, idx, &next_idx);
            if (val == NULL)
                goto bail;

            if (has_pairs_hook) {
                item = PyTuple_Pack(2, key, val);
                if (item == NULL)
                    goto bail;
                Py_CLEAR(key);
                Py_CLEAR(val);
                if (PyList_Append(pairs, item) == -1) {
                    Py_DECREF(item);
                    goto bail;
                }
                Py_DECREF(item);
            }
            else {
                if (PyDict_SetItem(rval, key, val) < 0)
                    goto bail;
                Py_CLEAR(key);
                Py_CLEAR(val);
            }
            idx = next_idx;

            
            while (idx <= end_idx && IS_WHITESPACE(str[idx])) idx++;

            if (idx > end_idx) break;
            if (str[idx] == '}') {
                break;
            }
            else if (str[idx] != ',') {
                raise_errmsg("Expecting ',' delimiter", pystr, idx);
                goto bail;
            }
            idx++;

            
            while (idx <= end_idx && IS_WHITESPACE(str[idx])) idx++;
        }
    }

    
    if (idx > end_idx || str[idx] != '}') {
        raise_errmsg("Expecting object", pystr, end_idx);
        goto bail;
    }

    
    if (s->pairs_hook != Py_None) {
        val = PyObject_CallFunctionObjArgs(s->pairs_hook, pairs, NULL);
        if (val == NULL)
            goto bail;
        Py_DECREF(pairs);
        *next_idx_ptr = idx + 1;
        return val;
    }

    
    if (s->object_hook != Py_None) {
        val = PyObject_CallFunctionObjArgs(s->object_hook, rval, NULL);
        if (val == NULL)
            goto bail;
        Py_DECREF(rval);
        rval = val;
        val = NULL;
    }
    *next_idx_ptr = idx + 1;
    return rval;
bail:
    Py_XDECREF(rval);
    Py_XDECREF(key);
    Py_XDECREF(val);
    Py_XDECREF(pairs);
    return NULL;
}

static PyObject *
_parse_array_str(PyScannerObject *s, PyObject *pystr, Py_ssize_t idx, Py_ssize_t *next_idx_ptr) {
    char *str = PyString_AS_STRING(pystr);
    Py_ssize_t end_idx = PyString_GET_SIZE(pystr) - 1;
    PyObject *val = NULL;
    PyObject *rval = PyList_New(0);
    Py_ssize_t next_idx;
    if (rval == NULL)
        return NULL;

    
    while (idx <= end_idx && IS_WHITESPACE(str[idx])) idx++;

    
    if (idx <= end_idx && str[idx] != ']') {
        while (idx <= end_idx) {

            
            val = scan_once_str(s, pystr, idx, &next_idx);
            if (val == NULL) {
                if (PyErr_ExceptionMatches(PyExc_StopIteration)) {
                    PyErr_Clear();
                    raise_errmsg("Expecting object", pystr, idx);
                }
                goto bail;
            }

            if (PyList_Append(rval, val) == -1)
                goto bail;

            Py_CLEAR(val);
            idx = next_idx;

            
            while (idx <= end_idx && IS_WHITESPACE(str[idx])) idx++;

            
            if (idx > end_idx) break;
            if (str[idx] == ']') {
                break;
            }
            else if (str[idx] != ',') {
                raise_errmsg("Expecting ',' delimiter", pystr, idx);
                goto bail;
            }
            idx++;

            
            while (idx <= end_idx && IS_WHITESPACE(str[idx])) idx++;
        }
    }

    
    if (idx > end_idx || str[idx] != ']') {
        raise_errmsg("Expecting object", pystr, end_idx);
        goto bail;
    }
    *next_idx_ptr = idx + 1;
    return rval;
bail:
    Py_XDECREF(val);
    Py_DECREF(rval);
    return NULL;
}

static PyObject *
_parse_array_unicode(PyScannerObject *s, PyObject *pystr, Py_ssize_t idx, Py_ssize_t *next_idx_ptr) {
    Py_UNICODE *str = PyUnicode_AS_UNICODE(pystr);
    Py_ssize_t end_idx = PyUnicode_GET_SIZE(pystr) - 1;
    PyObject *val = NULL;
    PyObject *rval = PyList_New(0);
    Py_ssize_t next_idx;
    if (rval == NULL)
        return NULL;

    
    while (idx <= end_idx && IS_WHITESPACE(str[idx])) idx++;

    
    if (idx <= end_idx && str[idx] != ']') {
        while (idx <= end_idx) {

            
            val = scan_once_unicode(s, pystr, idx, &next_idx);
            if (val == NULL) {
                if (PyErr_ExceptionMatches(PyExc_StopIteration)) {
                    PyErr_Clear();
                    raise_errmsg("Expecting object", pystr, idx);
                }
                goto bail;
            }

            if (PyList_Append(rval, val) == -1)
                goto bail;

            Py_CLEAR(val);
            idx = next_idx;

            
            while (idx <= end_idx && IS_WHITESPACE(str[idx])) idx++;

            
            if (idx > end_idx) break;
            if (str[idx] == ']') {
                break;
            }
            else if (str[idx] != ',') {
                raise_errmsg("Expecting ',' delimiter", pystr, idx);
                goto bail;
            }
            idx++;

            
            while (idx <= end_idx && IS_WHITESPACE(str[idx])) idx++;
        }
    }

    
    if (idx > end_idx || str[idx] != ']') {
        raise_errmsg("Expecting object", pystr, end_idx);
        goto bail;
    }
    *next_idx_ptr = idx + 1;
    return rval;
bail:
    Py_XDECREF(val);
    Py_DECREF(rval);
    return NULL;
}

static PyObject *
_parse_constant(PyScannerObject *s, char *constant, Py_ssize_t idx, Py_ssize_t *next_idx_ptr) {
    PyObject *cstr;
    PyObject *rval;
    
    cstr = PyString_InternFromString(constant);
    if (cstr == NULL)
        return NULL;

    
    rval = PyObject_CallFunctionObjArgs(s->parse_constant, cstr, NULL);
    idx += PyString_GET_SIZE(cstr);
    Py_DECREF(cstr);
    *next_idx_ptr = idx;
    return rval;
}

static PyObject *
_match_number_str(PyScannerObject *s, PyObject *pystr, Py_ssize_t start, Py_ssize_t *next_idx_ptr) {
    char *str = PyString_AS_STRING(pystr);
    Py_ssize_t end_idx = PyString_GET_SIZE(pystr) - 1;
    Py_ssize_t idx = start;
    int is_float = 0;
    PyObject *rval;
    PyObject *numstr;

    
    if (str[idx] == '-') {
        idx++;
        if (idx > end_idx) {
            PyErr_SetNone(PyExc_StopIteration);
            return NULL;
        }
    }

    
    if (str[idx] >= '1' && str[idx] <= '9') {
        idx++;
        while (idx <= end_idx && str[idx] >= '0' && str[idx] <= '9') idx++;
    }
    
    else if (str[idx] == '0') {
        idx++;
    }
    
    else {
        PyErr_SetNone(PyExc_StopIteration);
        return NULL;
    }

    
    if (idx < end_idx && str[idx] == '.' && str[idx + 1] >= '0' && str[idx + 1] <= '9') {
        is_float = 1;
        idx += 2;
        while (idx <= end_idx && str[idx] >= '0' && str[idx] <= '9') idx++;
    }

    
    if (idx < end_idx && (str[idx] == 'e' || str[idx] == 'E')) {

        
        Py_ssize_t e_start = idx;
        idx++;

        
        if (idx < end_idx && (str[idx] == '-' || str[idx] == '+')) idx++;

        
        while (idx <= end_idx && str[idx] >= '0' && str[idx] <= '9') idx++;

        
        if (str[idx - 1] >= '0' && str[idx - 1] <= '9') {
            is_float = 1;
        }
        else {
            idx = e_start;
        }
    }

    
    numstr = PyString_FromStringAndSize(&str[start], idx - start);
    if (numstr == NULL)
        return NULL;
    if (is_float) {
        
        if (s->parse_float != (PyObject *)&PyFloat_Type) {
            rval = PyObject_CallFunctionObjArgs(s->parse_float, numstr, NULL);
        }
        else {
            
            double d = PyOS_string_to_double(PyString_AS_STRING(numstr),
                                             NULL, NULL);
            if (d == -1.0 && PyErr_Occurred())
                return NULL;
            rval = PyFloat_FromDouble(d);
        }
    }
    else {
        
        if (s->parse_int != (PyObject *)&PyInt_Type) {
            rval = PyObject_CallFunctionObjArgs(s->parse_int, numstr, NULL);
        }
        else {
            rval = PyInt_FromString(PyString_AS_STRING(numstr), NULL, 10);
        }
    }
    Py_DECREF(numstr);
    *next_idx_ptr = idx;
    return rval;
}

static PyObject *
_match_number_unicode(PyScannerObject *s, PyObject *pystr, Py_ssize_t start, Py_ssize_t *next_idx_ptr) {
    Py_UNICODE *str = PyUnicode_AS_UNICODE(pystr);
    Py_ssize_t end_idx = PyUnicode_GET_SIZE(pystr) - 1;
    Py_ssize_t idx = start;
    int is_float = 0;
    PyObject *rval;
    PyObject *numstr;

    
    if (str[idx] == '-') {
        idx++;
        if (idx > end_idx) {
            PyErr_SetNone(PyExc_StopIteration);
            return NULL;
        }
    }

    
    if (str[idx] >= '1' && str[idx] <= '9') {
        idx++;
        while (idx <= end_idx && str[idx] >= '0' && str[idx] <= '9') idx++;
    }
    
    else if (str[idx] == '0') {
        idx++;
    }
    
    else {
        PyErr_SetNone(PyExc_StopIteration);
        return NULL;
    }

    
    if (idx < end_idx && str[idx] == '.' && str[idx + 1] >= '0' && str[idx + 1] <= '9') {
        is_float = 1;
        idx += 2;
        while (idx <= end_idx && str[idx] >= '0' && str[idx] <= '9') idx++;
    }

    
    if (idx < end_idx && (str[idx] == 'e' || str[idx] == 'E')) {
        Py_ssize_t e_start = idx;
        idx++;

        
        if (idx < end_idx && (str[idx] == '-' || str[idx] == '+')) idx++;

        
        while (idx <= end_idx && str[idx] >= '0' && str[idx] <= '9') idx++;

        
        if (str[idx - 1] >= '0' && str[idx - 1] <= '9') {
            is_float = 1;
        }
        else {
            idx = e_start;
        }
    }

    
    numstr = PyUnicode_FromUnicode(&str[start], idx - start);
    if (numstr == NULL)
        return NULL;
    if (is_float) {
        
        if (s->parse_float != (PyObject *)&PyFloat_Type) {
            rval = PyObject_CallFunctionObjArgs(s->parse_float, numstr, NULL);
        }
        else {
            rval = PyFloat_FromString(numstr, NULL);
        }
    }
    else {
        
        rval = PyObject_CallFunctionObjArgs(s->parse_int, numstr, NULL);
    }
    Py_DECREF(numstr);
    *next_idx_ptr = idx;
    return rval;
}

static PyObject *
scan_once_str(PyScannerObject *s, PyObject *pystr, Py_ssize_t idx, Py_ssize_t *next_idx_ptr)
{
    char *str = PyString_AS_STRING(pystr);
    Py_ssize_t length = PyString_GET_SIZE(pystr);
    PyObject *rval = NULL;
    int fallthrough = 0;
    if (idx >= length) {
        PyErr_SetNone(PyExc_StopIteration);
        return NULL;
    }
    if (Py_EnterRecursiveCall(" while decoding a JSON document"))
        return NULL;
    switch (str[idx]) {
        case '"':
            
            rval = scanstring_str(pystr, idx + 1,
                PyString_AS_STRING(s->encoding),
                PyObject_IsTrue(s->strict),
                next_idx_ptr);
            break;
        case '{':
            
            rval = _parse_object_str(s, pystr, idx + 1, next_idx_ptr);
            break;
        case '[':
            
            rval = _parse_array_str(s, pystr, idx + 1, next_idx_ptr);
            break;
        case 'n':
            
            if ((idx + 3 < length) && str[idx + 1] == 'u' && str[idx + 2] == 'l' && str[idx + 3] == 'l') {
                Py_INCREF(Py_None);
                *next_idx_ptr = idx + 4;
                rval = Py_None;
            }
            else
                fallthrough = 1;
            break;
        case 't':
            
            if ((idx + 3 < length) && str[idx + 1] == 'r' && str[idx + 2] == 'u' && str[idx + 3] == 'e') {
                Py_INCREF(Py_True);
                *next_idx_ptr = idx + 4;
                rval = Py_True;
            }
            else
                fallthrough = 1;
            break;
        case 'f':
            
            if ((idx + 4 < length) && str[idx + 1] == 'a' && str[idx + 2] == 'l' && str[idx + 3] == 's' && str[idx + 4] == 'e') {
                Py_INCREF(Py_False);
                *next_idx_ptr = idx + 5;
                rval = Py_False;
            }
            else
                fallthrough = 1;
            break;
        case 'N':
            
            if ((idx + 2 < length) && str[idx + 1] == 'a' && str[idx + 2] == 'N') {
                rval = _parse_constant(s, "NaN", idx, next_idx_ptr);
            }
            else
                fallthrough = 1;
            break;
        case 'I':
            
            if ((idx + 7 < length) && str[idx + 1] == 'n' && str[idx + 2] == 'f' && str[idx + 3] == 'i' && str[idx + 4] == 'n' && str[idx + 5] == 'i' && str[idx + 6] == 't' && str[idx + 7] == 'y') {
                rval = _parse_constant(s, "Infinity", idx, next_idx_ptr);
            }
            else
                fallthrough = 1;
            break;
        case '-':
            
            if ((idx + 8 < length) && str[idx + 1] == 'I' && str[idx + 2] == 'n' && str[idx + 3] == 'f' && str[idx + 4] == 'i' && str[idx + 5] == 'n' && str[idx + 6] == 'i' && str[idx + 7] == 't' && str[idx + 8] == 'y') {
                rval = _parse_constant(s, "-Infinity", idx, next_idx_ptr);
            }
            else
                fallthrough = 1;
            break;
        default:
            fallthrough = 1;
    }
    
    if (fallthrough)
        rval = _match_number_str(s, pystr, idx, next_idx_ptr);
    Py_LeaveRecursiveCall();
    return rval;
}

static PyObject *
scan_once_unicode(PyScannerObject *s, PyObject *pystr, Py_ssize_t idx, Py_ssize_t *next_idx_ptr)
{
    Py_UNICODE *str = PyUnicode_AS_UNICODE(pystr);
    Py_ssize_t length = PyUnicode_GET_SIZE(pystr);
    PyObject *rval = NULL;
    int fallthrough = 0;
    if (idx >= length) {
        PyErr_SetNone(PyExc_StopIteration);
        return NULL;
    }
    if (Py_EnterRecursiveCall(" while decoding a JSON document"))
        return NULL;
    switch (str[idx]) {
        case '"':
            
            rval = scanstring_unicode(pystr, idx + 1,
                PyObject_IsTrue(s->strict),
                next_idx_ptr);
            break;
        case '{':
            
            rval = _parse_object_unicode(s, pystr, idx + 1, next_idx_ptr);
            break;
        case '[':
            
            rval = _parse_array_unicode(s, pystr, idx + 1, next_idx_ptr);
            break;
        case 'n':
            
            if ((idx + 3 < length) && str[idx + 1] == 'u' && str[idx + 2] == 'l' && str[idx + 3] == 'l') {
                Py_INCREF(Py_None);
                *next_idx_ptr = idx + 4;
                rval = Py_None;
            }
            else
                fallthrough = 1;
            break;
        case 't':
            
            if ((idx + 3 < length) && str[idx + 1] == 'r' && str[idx + 2] == 'u' && str[idx + 3] == 'e') {
                Py_INCREF(Py_True);
                *next_idx_ptr = idx + 4;
                rval = Py_True;
            }
            else
                fallthrough = 1;
            break;
        case 'f':
            
            if ((idx + 4 < length) && str[idx + 1] == 'a' && str[idx + 2] == 'l' && str[idx + 3] == 's' && str[idx + 4] == 'e') {
                Py_INCREF(Py_False);
                *next_idx_ptr = idx + 5;
                rval = Py_False;
            }
            else
                fallthrough = 1;
            break;
        case 'N':
            
            if ((idx + 2 < length) && str[idx + 1] == 'a' && str[idx + 2] == 'N') {
                rval = _parse_constant(s, "NaN", idx, next_idx_ptr);
            }
            else
                fallthrough = 1;
            break;
        case 'I':
            
            if ((idx + 7 < length) && str[idx + 1] == 'n' && str[idx + 2] == 'f' && str[idx + 3] == 'i' && str[idx + 4] == 'n' && str[idx + 5] == 'i' && str[idx + 6] == 't' && str[idx + 7] == 'y') {
                rval = _parse_constant(s, "Infinity", idx, next_idx_ptr);
            }
            else
                fallthrough = 1;
            break;
        case '-':
            
            if ((idx + 8 < length) && str[idx + 1] == 'I' && str[idx + 2] == 'n' && str[idx + 3] == 'f' && str[idx + 4] == 'i' && str[idx + 5] == 'n' && str[idx + 6] == 'i' && str[idx + 7] == 't' && str[idx + 8] == 'y') {
                rval = _parse_constant(s, "-Infinity", idx, next_idx_ptr);
            }
            else
                fallthrough = 1;
            break;
        default:
            fallthrough = 1;
    }
    
    if (fallthrough)
        rval = _match_number_unicode(s, pystr, idx, next_idx_ptr);
    Py_LeaveRecursiveCall();
    return rval;
}

static PyObject *
scanner_call(PyObject *self, PyObject *args, PyObject *kwds)
{
    
    PyObject *pystr;
    PyObject *rval;
    Py_ssize_t idx;
    Py_ssize_t next_idx = -1;
    static char *kwlist[] = {"string", "idx", NULL};
    PyScannerObject *s;
    assert(PyScanner_Check(self));
    s = (PyScannerObject *)self;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "OO&:scan_once", kwlist, &pystr, _convertPyInt_AsSsize_t, &idx))
        return NULL;

    if (PyString_Check(pystr)) {
        rval = scan_once_str(s, pystr, idx, &next_idx);
    }
    else if (PyUnicode_Check(pystr)) {
        rval = scan_once_unicode(s, pystr, idx, &next_idx);
    }
    else {
        PyErr_Format(PyExc_TypeError,
                 "first argument must be a string, not %.80s",
                 Py_TYPE(pystr)->tp_name);
        return NULL;
    }
    PyDict_Clear(s->memo);
    return _build_rval_index_tuple(rval, next_idx);
}

static PyObject *
scanner_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyScannerObject *s;
    s = (PyScannerObject *)type->tp_alloc(type, 0);
    if (s != NULL) {
        s->encoding = NULL;
        s->strict = NULL;
        s->object_hook = NULL;
        s->pairs_hook = NULL;
        s->parse_float = NULL;
        s->parse_int = NULL;
        s->parse_constant = NULL;
    }
    return (PyObject *)s;
}

static int
scanner_init(PyObject *self, PyObject *args, PyObject *kwds)
{
    
    PyObject *ctx;
    static char *kwlist[] = {"context", NULL};
    PyScannerObject *s;

    assert(PyScanner_Check(self));
    s = (PyScannerObject *)self;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "O:make_scanner", kwlist, &ctx))
        return -1;

    if (s->memo == NULL) {
        s->memo = PyDict_New();
        if (s->memo == NULL)
            goto bail;
    }

    
    s->encoding = PyObject_GetAttrString(ctx, "encoding");
    if (s->encoding == NULL)
        goto bail;
    if (s->encoding == Py_None) {
        Py_DECREF(Py_None);
        s->encoding = PyString_InternFromString(DEFAULT_ENCODING);
    }
    else if (PyUnicode_Check(s->encoding)) {
        PyObject *tmp = PyUnicode_AsEncodedString(s->encoding, NULL, NULL);
        Py_DECREF(s->encoding);
        s->encoding = tmp;
    }
    if (s->encoding == NULL || !PyString_Check(s->encoding))
        goto bail;

    
    s->strict = PyObject_GetAttrString(ctx, "strict");
    if (s->strict == NULL)
        goto bail;
    s->object_hook = PyObject_GetAttrString(ctx, "object_hook");
    if (s->object_hook == NULL)
        goto bail;
    s->pairs_hook = PyObject_GetAttrString(ctx, "object_pairs_hook");
    if (s->pairs_hook == NULL)
        goto bail;
    s->parse_float = PyObject_GetAttrString(ctx, "parse_float");
    if (s->parse_float == NULL)
        goto bail;
    s->parse_int = PyObject_GetAttrString(ctx, "parse_int");
    if (s->parse_int == NULL)
        goto bail;
    s->parse_constant = PyObject_GetAttrString(ctx, "parse_constant");
    if (s->parse_constant == NULL)
        goto bail;

    return 0;

bail:
    Py_CLEAR(s->encoding);
    Py_CLEAR(s->strict);
    Py_CLEAR(s->object_hook);
    Py_CLEAR(s->pairs_hook);
    Py_CLEAR(s->parse_float);
    Py_CLEAR(s->parse_int);
    Py_CLEAR(s->parse_constant);
    return -1;
}

PyDoc_STRVAR(scanner_doc, "JSON scanner object");

static
PyTypeObject PyScannerType = {
    PyObject_HEAD_INIT(NULL)
    0,                    
    "simplejson._speedups.Scanner",       
    sizeof(PyScannerObject), 
    0,                    
    scanner_dealloc, 
    0,                    
    0,                    
    0,                    
    0,                    
    0,                    
    0,                    
    0,                    
    0,                    
    0,                    
    scanner_call,         
    0,                    
    0,                    
    0,                    
    0,                    
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC,   
    scanner_doc,          
    scanner_traverse,                    
    scanner_clear,                    
    0,                    
    0,                    
    0,                    
    0,                    
    0,                    
    scanner_members,                    
    0,                    
    0,                    
    0,                    
    0,                    
    0,                    
    0,                    
    scanner_init,                    
    0,        
    scanner_new,          
    0,              
};

static PyObject *
encoder_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyEncoderObject *s;
    s = (PyEncoderObject *)type->tp_alloc(type, 0);
    if (s != NULL) {
        s->markers = NULL;
        s->defaultfn = NULL;
        s->encoder = NULL;
        s->indent = NULL;
        s->key_separator = NULL;
        s->item_separator = NULL;
        s->sort_keys = NULL;
        s->skipkeys = NULL;
        s->key_memo = NULL;
        s->item_sort_key = NULL;
        s->Decimal = NULL;
    }
    return (PyObject *)s;
}

static int
encoder_init(PyObject *self, PyObject *args, PyObject *kwds)
{
    
    static char *kwlist[] = {"markers", "default", "encoder", "indent", "key_separator", "item_separator", "sort_keys", "skipkeys", "allow_nan", "key_memo", "use_decimal", "namedtuple_as_object", "tuple_as_array", "bigint_as_string", "item_sort_key", "Decimal", NULL};

    PyEncoderObject *s;
    PyObject *markers, *defaultfn, *encoder, *indent, *key_separator;
    PyObject *item_separator, *sort_keys, *skipkeys, *allow_nan, *key_memo;
    PyObject *use_decimal, *namedtuple_as_object, *tuple_as_array;
    PyObject *bigint_as_string, *item_sort_key, *Decimal;

    assert(PyEncoder_Check(self));
    s = (PyEncoderObject *)self;

    if (!PyArg_ParseTupleAndKeywords(args, kwds, "OOOOOOOOOOOOOOOO:make_encoder", kwlist,
        &markers, &defaultfn, &encoder, &indent, &key_separator, &item_separator,
        &sort_keys, &skipkeys, &allow_nan, &key_memo, &use_decimal,
        &namedtuple_as_object, &tuple_as_array, &bigint_as_string,
        &item_sort_key, &Decimal))
        return -1;

    s->markers = markers;
    s->defaultfn = defaultfn;
    s->encoder = encoder;
    s->indent = indent;
    s->key_separator = key_separator;
    s->item_separator = item_separator;
    s->sort_keys = sort_keys;
    s->skipkeys = skipkeys;
    s->key_memo = key_memo;
    s->fast_encode = (PyCFunction_Check(s->encoder) && PyCFunction_GetFunction(s->encoder) == (PyCFunction)py_encode_basestring_ascii);
    s->allow_nan = PyObject_IsTrue(allow_nan);
    s->use_decimal = PyObject_IsTrue(use_decimal);
    s->namedtuple_as_object = PyObject_IsTrue(namedtuple_as_object);
    s->tuple_as_array = PyObject_IsTrue(tuple_as_array);
    s->bigint_as_string = PyObject_IsTrue(bigint_as_string);
    s->item_sort_key = item_sort_key;
    s->Decimal = Decimal;

    Py_INCREF(s->markers);
    Py_INCREF(s->defaultfn);
    Py_INCREF(s->encoder);
    Py_INCREF(s->indent);
    Py_INCREF(s->key_separator);
    Py_INCREF(s->item_separator);
    Py_INCREF(s->sort_keys);
    Py_INCREF(s->skipkeys);
    Py_INCREF(s->key_memo);
    Py_INCREF(s->item_sort_key);
    Py_INCREF(s->Decimal);
    return 0;
}

static PyObject *
encoder_call(PyObject *self, PyObject *args, PyObject *kwds)
{
    
    static char *kwlist[] = {"obj", "_current_indent_level", NULL};
    PyObject *obj;
    PyObject *rval;
    Py_ssize_t indent_level;
    PyEncoderObject *s;
    assert(PyEncoder_Check(self));
    s = (PyEncoderObject *)self;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "OO&:_iterencode", kwlist,
        &obj, _convertPyInt_AsSsize_t, &indent_level))
        return NULL;
    rval = PyList_New(0);
    if (rval == NULL)
        return NULL;
    if (encoder_listencode_obj(s, rval, obj, indent_level)) {
        Py_DECREF(rval);
        return NULL;
    }
    return rval;
}

static PyObject *
_encoded_const(PyObject *obj)
{
    
    if (obj == Py_None) {
        static PyObject *s_null = NULL;
        if (s_null == NULL) {
            s_null = PyString_InternFromString("null");
        }
        Py_INCREF(s_null);
        return s_null;
    }
    else if (obj == Py_True) {
        static PyObject *s_true = NULL;
        if (s_true == NULL) {
            s_true = PyString_InternFromString("true");
        }
        Py_INCREF(s_true);
        return s_true;
    }
    else if (obj == Py_False) {
        static PyObject *s_false = NULL;
        if (s_false == NULL) {
            s_false = PyString_InternFromString("false");
        }
        Py_INCREF(s_false);
        return s_false;
    }
    else {
        PyErr_SetString(PyExc_ValueError, "not a const");
        return NULL;
    }
}

static PyObject *
encoder_encode_float(PyEncoderObject *s, PyObject *obj)
{
    
    double i = PyFloat_AS_DOUBLE(obj);
    if (!Py_IS_FINITE(i)) {
        if (!s->allow_nan) {
            PyErr_SetString(PyExc_ValueError, "Out of range float values are not JSON compliant");
            return NULL;
        }
        if (i > 0) {
            return PyString_FromString("Infinity");
        }
        else if (i < 0) {
            return PyString_FromString("-Infinity");
        }
        else {
            return PyString_FromString("NaN");
        }
    }
    
    return PyObject_Repr(obj);
}

static PyObject *
encoder_encode_string(PyEncoderObject *s, PyObject *obj)
{
    
    if (s->fast_encode)
        return py_encode_basestring_ascii(NULL, obj);
    else
        return PyObject_CallFunctionObjArgs(s->encoder, obj, NULL);
}

static int
_steal_list_append(PyObject *lst, PyObject *stolen)
{
    
    int rval = PyList_Append(lst, stolen);
    Py_DECREF(stolen);
    return rval;
}

static int
encoder_listencode_obj(PyEncoderObject *s, PyObject *rval, PyObject *obj, Py_ssize_t indent_level)
{
    
    int rv = -1;
    if (Py_EnterRecursiveCall(" while encoding a JSON document"))
        return rv;
    do {
        if (obj == Py_None || obj == Py_True || obj == Py_False) {
            PyObject *cstr = _encoded_const(obj);
            if (cstr != NULL)
                rv = _steal_list_append(rval, cstr);
        }
        else if (PyString_Check(obj) || PyUnicode_Check(obj))
        {
            PyObject *encoded = encoder_encode_string(s, obj);
            if (encoded != NULL)
                rv = _steal_list_append(rval, encoded);
        }
        else if (PyInt_Check(obj) || PyLong_Check(obj)) {
            PyObject *encoded = PyObject_Str(obj);
            if (encoded != NULL) {
                if (s->bigint_as_string) {
                    encoded = maybe_quote_bigint(encoded, obj);
                    if (encoded == NULL)
                        break;
                }
                rv = _steal_list_append(rval, encoded);
            }
        }
        else if (PyFloat_Check(obj)) {
            PyObject *encoded = encoder_encode_float(s, obj);
            if (encoded != NULL)
                rv = _steal_list_append(rval, encoded);
        }
        else if (s->namedtuple_as_object && _is_namedtuple(obj)) {
            PyObject *newobj = PyObject_CallMethod(obj, "_asdict", NULL);
            if (newobj != NULL) {
                rv = encoder_listencode_dict(s, rval, newobj, indent_level);
                Py_DECREF(newobj);
            }
        }
        else if (PyList_Check(obj) || (s->tuple_as_array && PyTuple_Check(obj))) {
            rv = encoder_listencode_list(s, rval, obj, indent_level);
        }
        else if (PyDict_Check(obj)) {
            rv = encoder_listencode_dict(s, rval, obj, indent_level);
        }
        else if (s->use_decimal && PyObject_TypeCheck(obj, s->Decimal)) {
            PyObject *encoded = PyObject_Str(obj);
            if (encoded != NULL)
                rv = _steal_list_append(rval, encoded);
        }
        else {
            PyObject *ident = NULL;
            PyObject *newobj;
            if (s->markers != Py_None) {
                int has_key;
                ident = PyLong_FromVoidPtr(obj);
                if (ident == NULL)
                    break;
                has_key = PyDict_Contains(s->markers, ident);
                if (has_key) {
                    if (has_key != -1)
                        PyErr_SetString(PyExc_ValueError, "Circular reference detected");
                    Py_DECREF(ident);
                    break;
                }
                if (PyDict_SetItem(s->markers, ident, obj)) {
                    Py_DECREF(ident);
                    break;
                }
            }
            newobj = PyObject_CallFunctionObjArgs(s->defaultfn, obj, NULL);
            if (newobj == NULL) {
                Py_XDECREF(ident);
                break;
            }
            rv = encoder_listencode_obj(s, rval, newobj, indent_level);
            Py_DECREF(newobj);
            if (rv) {
                Py_XDECREF(ident);
                rv = -1;
            }
            else if (ident != NULL) {
                if (PyDict_DelItem(s->markers, ident)) {
                    Py_XDECREF(ident);
                    rv = -1;
                }
                Py_XDECREF(ident);
            }
        }
    } while (0);
    Py_LeaveRecursiveCall();
    return rv;
}

static int
encoder_listencode_dict(PyEncoderObject *s, PyObject *rval, PyObject *dct, Py_ssize_t indent_level)
{
    
    static PyObject *open_dict = NULL;
    static PyObject *close_dict = NULL;
    static PyObject *empty_dict = NULL;
    static PyObject *iteritems = NULL;
    PyObject *kstr = NULL;
    PyObject *ident = NULL;
    PyObject *iter = NULL;
    PyObject *item = NULL;
    PyObject *items = NULL;
    PyObject *encoded = NULL;
    int skipkeys;
    Py_ssize_t idx;

    if (open_dict == NULL || close_dict == NULL || empty_dict == NULL || iteritems == NULL) {
        open_dict = PyString_InternFromString("{");
        close_dict = PyString_InternFromString("}");
        empty_dict = PyString_InternFromString("{}");
        iteritems = PyString_InternFromString("iteritems");
        if (open_dict == NULL || close_dict == NULL || empty_dict == NULL || iteritems == NULL)
            return -1;
    }
    if (PyDict_Size(dct) == 0)
        return PyList_Append(rval, empty_dict);

    if (s->markers != Py_None) {
        int has_key;
        ident = PyLong_FromVoidPtr(dct);
        if (ident == NULL)
            goto bail;
        has_key = PyDict_Contains(s->markers, ident);
        if (has_key) {
            if (has_key != -1)
                PyErr_SetString(PyExc_ValueError, "Circular reference detected");
            goto bail;
        }
        if (PyDict_SetItem(s->markers, ident, dct)) {
            goto bail;
        }
    }

    if (PyList_Append(rval, open_dict))
        goto bail;

    if (s->indent != Py_None) {
        
        indent_level += 1;
    }

    if (PyCallable_Check(s->item_sort_key)) {
        if (PyDict_CheckExact(dct))
            items = PyDict_Items(dct);
        else
            items = PyMapping_Items(dct);
        PyObject_CallMethod(items, "sort", "OO", Py_None, s->item_sort_key);
    }
    else if (PyObject_IsTrue(s->sort_keys)) {
        
        Py_ssize_t i, nitems;
        if (PyDict_CheckExact(dct))
            items = PyDict_Keys(dct);
        else
            items = PyMapping_Keys(dct);
        if (items == NULL)
            goto bail;
        if (!PyList_Check(items)) {
            PyErr_SetString(PyExc_ValueError, "keys must return list");
            goto bail;
        }
        if (PyList_Sort(items) < 0)
            goto bail;
        nitems = PyList_GET_SIZE(items);
        for (i = 0; i < nitems; i++) {
            PyObject *key, *value;
            key = PyList_GET_ITEM(items, i);
            value = PyDict_GetItem(dct, key);
            item = PyTuple_Pack(2, key, value);
            if (item == NULL)
                goto bail;
            PyList_SET_ITEM(items, i, item);
            Py_DECREF(key);
        }
    }
    else {
        if (PyDict_CheckExact(dct))
            items = PyDict_Items(dct);
        else
            items = PyMapping_Items(dct);
    }
    if (items == NULL)
        goto bail;
    iter = PyObject_GetIter(items);
    Py_DECREF(items);
    if (iter == NULL)
        goto bail;

    skipkeys = PyObject_IsTrue(s->skipkeys);
    idx = 0;
    while ((item = PyIter_Next(iter))) {
        PyObject *encoded, *key, *value;
        if (!PyTuple_Check(item) || Py_SIZE(item) != 2) {
            PyErr_SetString(PyExc_ValueError, "items must return 2-tuples");
            goto bail;
        }
        key = PyTuple_GET_ITEM(item, 0);
        if (key == NULL)
            goto bail;
        value = PyTuple_GET_ITEM(item, 1);
        if (value == NULL)
            goto bail;

        encoded = PyDict_GetItem(s->key_memo, key);
        if (encoded != NULL) {
            Py_INCREF(encoded);
        }
        else if (PyString_Check(key) || PyUnicode_Check(key)) {
            Py_INCREF(key);
            kstr = key;
        }
        else if (PyFloat_Check(key)) {
            kstr = encoder_encode_float(s, key);
            if (kstr == NULL)
                goto bail;
        }
        else if (key == Py_True || key == Py_False || key == Py_None) {
            kstr = _encoded_const(key);
            if (kstr == NULL)
                goto bail;
        }
        else if (PyInt_Check(key) || PyLong_Check(key)) {
            kstr = PyObject_Str(key);
            if (kstr == NULL)
                goto bail;
        }
        else if (skipkeys) {
            Py_DECREF(item);
            continue;
        }
        else {
            
            PyErr_SetString(PyExc_TypeError, "keys must be a string");
            goto bail;
        }

        if (idx) {
            if (PyList_Append(rval, s->item_separator))
                goto bail;
        }

        if (encoded == NULL) {
            encoded = encoder_encode_string(s, kstr);
            Py_CLEAR(kstr);
            if (encoded == NULL)
                goto bail;
            if (PyDict_SetItem(s->key_memo, key, encoded))
                goto bail;
        }
        if (PyList_Append(rval, encoded)) {
            goto bail;
        }
        Py_CLEAR(encoded);
        if (PyList_Append(rval, s->key_separator))
            goto bail;
        if (encoder_listencode_obj(s, rval, value, indent_level))
            goto bail;
        Py_CLEAR(item);
        idx += 1;
    }
    Py_CLEAR(iter);
    if (PyErr_Occurred())
        goto bail;
    if (ident != NULL) {
        if (PyDict_DelItem(s->markers, ident))
            goto bail;
        Py_CLEAR(ident);
    }
    if (s->indent != Py_None) {
        
        indent_level -= 1;
    }
    if (PyList_Append(rval, close_dict))
        goto bail;
    return 0;

bail:
    Py_XDECREF(encoded);
    Py_XDECREF(items);
    Py_XDECREF(iter);
    Py_XDECREF(kstr);
    Py_XDECREF(ident);
    return -1;
}


static int
encoder_listencode_list(PyEncoderObject *s, PyObject *rval, PyObject *seq, Py_ssize_t indent_level)
{
    
    static PyObject *open_array = NULL;
    static PyObject *close_array = NULL;
    static PyObject *empty_array = NULL;
    PyObject *ident = NULL;
    PyObject *iter = NULL;
    PyObject *obj = NULL;
    int is_true;
    int i = 0;

    if (open_array == NULL || close_array == NULL || empty_array == NULL) {
        open_array = PyString_InternFromString("[");
        close_array = PyString_InternFromString("]");
        empty_array = PyString_InternFromString("[]");
        if (open_array == NULL || close_array == NULL || empty_array == NULL)
            return -1;
    }
    ident = NULL;
    is_true = PyObject_IsTrue(seq);
    if (is_true == -1)
        return -1;
    else if (is_true == 0)
        return PyList_Append(rval, empty_array);

    if (s->markers != Py_None) {
        int has_key;
        ident = PyLong_FromVoidPtr(seq);
        if (ident == NULL)
            goto bail;
        has_key = PyDict_Contains(s->markers, ident);
        if (has_key) {
            if (has_key != -1)
                PyErr_SetString(PyExc_ValueError, "Circular reference detected");
            goto bail;
        }
        if (PyDict_SetItem(s->markers, ident, seq)) {
            goto bail;
        }
    }

    iter = PyObject_GetIter(seq);
    if (iter == NULL)
        goto bail;

    if (PyList_Append(rval, open_array))
        goto bail;
    if (s->indent != Py_None) {
        
        indent_level += 1;
    }
    while ((obj = PyIter_Next(iter))) {
        if (i) {
            if (PyList_Append(rval, s->item_separator))
                goto bail;
        }
        if (encoder_listencode_obj(s, rval, obj, indent_level))
            goto bail;
        i++;
        Py_CLEAR(obj);
    }
    Py_CLEAR(iter);
    if (PyErr_Occurred())
        goto bail;
    if (ident != NULL) {
        if (PyDict_DelItem(s->markers, ident))
            goto bail;
        Py_CLEAR(ident);
    }
    if (s->indent != Py_None) {
        
        indent_level -= 1;
    }
    if (PyList_Append(rval, close_array))
        goto bail;
    return 0;

bail:
    Py_XDECREF(obj);
    Py_XDECREF(iter);
    Py_XDECREF(ident);
    return -1;
}

static void
encoder_dealloc(PyObject *self)
{
    
    encoder_clear(self);
    Py_TYPE(self)->tp_free(self);
}

static int
encoder_traverse(PyObject *self, visitproc visit, void *arg)
{
    PyEncoderObject *s;
    assert(PyEncoder_Check(self));
    s = (PyEncoderObject *)self;
    Py_VISIT(s->markers);
    Py_VISIT(s->defaultfn);
    Py_VISIT(s->encoder);
    Py_VISIT(s->indent);
    Py_VISIT(s->key_separator);
    Py_VISIT(s->item_separator);
    Py_VISIT(s->sort_keys);
    Py_VISIT(s->skipkeys);
    Py_VISIT(s->key_memo);
    Py_VISIT(s->item_sort_key);
    return 0;
}

static int
encoder_clear(PyObject *self)
{
    
    PyEncoderObject *s;
    assert(PyEncoder_Check(self));
    s = (PyEncoderObject *)self;
    Py_CLEAR(s->markers);
    Py_CLEAR(s->defaultfn);
    Py_CLEAR(s->encoder);
    Py_CLEAR(s->indent);
    Py_CLEAR(s->key_separator);
    Py_CLEAR(s->item_separator);
    Py_CLEAR(s->sort_keys);
    Py_CLEAR(s->skipkeys);
    Py_CLEAR(s->key_memo);
    Py_CLEAR(s->item_sort_key);
    Py_CLEAR(s->Decimal);
    return 0;
}

PyDoc_STRVAR(encoder_doc, "_iterencode(obj, _current_indent_level) -> iterable");

static
PyTypeObject PyEncoderType = {
    PyObject_HEAD_INIT(NULL)
    0,                    
    "simplejson._speedups.Encoder",       
    sizeof(PyEncoderObject), 
    0,                    
    encoder_dealloc, 
    0,                    
    0,                    
    0,                    
    0,                    
    0,                    
    0,                    
    0,                    
    0,                    
    0,                    
    encoder_call,         
    0,                    
    0,                    
    0,                    
    0,                    
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_GC,   
    encoder_doc,          
    encoder_traverse,     
    encoder_clear,        
    0,                    
    0,                    
    0,                    
    0,                    
    0,                    
    encoder_members,      
    0,                    
    0,                    
    0,                    
    0,                    
    0,                    
    0,                    
    encoder_init,         
    0,                    
    encoder_new,          
    0,                    
};

static PyMethodDef speedups_methods[] = {
    {"encode_basestring_ascii",
        (PyCFunction)py_encode_basestring_ascii,
        METH_O,
        pydoc_encode_basestring_ascii},
    {"scanstring",
        (PyCFunction)py_scanstring,
        METH_VARARGS,
        pydoc_scanstring},
    {NULL, NULL, 0, NULL}
};

PyDoc_STRVAR(module_doc,
"simplejson speedups\n");

void
init_speedups(void)
{
    PyObject *m;
    PyScannerType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&PyScannerType) < 0)
        return;
    PyEncoderType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&PyEncoderType) < 0)
        return;


    m = Py_InitModule3("_speedups", speedups_methods, module_doc);
    Py_INCREF((PyObject*)&PyScannerType);
    PyModule_AddObject(m, "make_scanner", (PyObject*)&PyScannerType);
    Py_INCREF((PyObject*)&PyEncoderType);
    PyModule_AddObject(m, "make_encoder", (PyObject*)&PyEncoderType);
}