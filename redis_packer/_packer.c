#include "Python.h"

/* Calculate the number of bytes needed to represent an integer as string. */
static int intlen(int i) {
    int len = 0;
    if (i < 0) {
        len++;
        i = -i;
    }
    do {
        len++;
        i /= 10;
    } while(i);
    return len;
}

/* Helper that calculates the bulk length given a certain string length. */
static size_t bulklen(size_t len) {
    return 1+intlen(len)+2+len+2;
}

/* Format a command according to the Redis protocol. This function takes the
 * number of arguments, an array with arguments and an array with their
 * lengths. If the latter is set to NULL, strlen will be used to compute the
 * argument lengths.
 */
int redisFormatCommandArgv(char **target, int argc, const char **argv, const size_t *argvlen) {
    char *cmd = NULL; /* final command */
    int pos; /* position in final command */
    size_t len;
    int totlen, j;

    /* Calculate number of bytes needed for the command */
    totlen = 1+intlen(argc)+2;
    for (j = 0; j < argc; j++) {
        len = argvlen ? argvlen[j] : strlen(argv[j]);
        totlen += bulklen(len);
    }

    /* Build the command at protocol level */
    cmd = malloc(totlen+1);
    if (cmd == NULL)
        return -1;

    pos = sprintf(cmd,"*%d\r\n",argc);
    for (j = 0; j < argc; j++) {
        len = argvlen ? argvlen[j] : strlen(argv[j]);
        pos += sprintf(cmd+pos,"$%zu\r\n",len);
        memcpy(cmd+pos,argv[j],len);
        pos += len;
        cmd[pos++] = '\r';
        cmd[pos++] = '\n';
    }
    assert(pos == totlen);
    cmd[pos] = '\0';

    *target = cmd;
    return totlen;
}

int ParseArguments(char **argv, size_t *argvlen, Py_ssize_t size, PyObject *args) {
    Py_ssize_t i;
    PyObject *temp_p, *temp_p2;

    for (i=0;i<size;i++) {
        temp_p = PyTuple_GetItem(args,i);

        if (temp_p == NULL) {
            return -1;
        }

        /* Check if temp_p is a string */
        if (PyString_Check(temp_p) == 1) {
             /* Convert string to char * c string */
            argv[i] = PyString_AsString(temp_p);
            argvlen[i] = PyString_Size(temp_p);
        } else if (PyFloat_Check(temp_p) == 1) {
            temp_p2 = PyObject_Repr(temp_p);
            argv[i] = PyString_AsString(temp_p2);
            argvlen[i] = PyString_Size(temp_p2);
            Py_DECREF(temp_p2);
        } else if (PyUnicode_Check(temp_p) == 1) {
            temp_p2 = PyUnicode_AsEncodedString(temp_p, "UTF-8", "strict");
            argv[i] = PyString_AsString(temp_p2);
            argvlen[i] = PyString_Size(temp_p2);
            Py_DECREF(temp_p2);
        } else {
            temp_p2 = PyObject_Str(temp_p);
            argv[i] = PyString_AsString(temp_p2);
            argvlen[i] = PyString_Size(temp_p2);
            Py_DECREF(temp_p2);
        }
    }

    return 1;
}

static PyObject *packer_pack_command(PyObject *self, PyObject *args) {
    Py_ssize_t tuple_size = PyTuple_Size(args);
    char **argv = malloc(tuple_size * sizeof(char *));
    size_t *argvlen = malloc(tuple_size * sizeof(size_t));
    char *cmd; 
    int len, i;
    PyObject *str_out;

    if (!tuple_size) {
        if (!PyErr_Occurred()) {
            PyErr_SetString(PyExc_TypeError, "You must supply at least one argument.");
        }
        return NULL;
    }

    //len = pythonFormatCommandArgv(&cmd, (size_t) tuple_size, args);
    if (!(ParseArguments(argv, argvlen, tuple_size, args))) { 
        free(argv);
        return NULL;
    }

    // TODO: check size
    len = redisFormatCommandArgv(&cmd, (size_t) tuple_size, (const char **)argv, argvlen);
    str_out = PyString_FromStringAndSize((const char *)cmd, (Py_ssize_t) len);

    free(cmd);
    free(argv);
    free(argvlen);
    return (PyObject *)str_out;
}

static PyMethodDef module_methods[] = {
    {"pack_command", (PyCFunction)packer_pack_command, METH_VARARGS, "pack redis command."},
    {NULL, NULL, 0, NULL}
};

//void initpacker(void) {
//    PyImport_AddModule("redis_packer._packer");
//    Py_InitModule("redis_packer._packer", module_methods);
//}
//
//int main(int argc, char *argv[]) {
//    Py_SetProgramName(argv[0]);
//    Py_Initialize();
//    initpacker();
//    return 0;
//}

PyMODINIT_FUNC
init_packer(void) {
    //PyImport_AddModule("redis_packer");
    //PyImport_AddModule("redis_packer._packer");
    
    Py_InitModule3("redis_packer._packer", module_methods, "");
}
