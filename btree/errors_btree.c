#include "errors_btree.h"

const struct err_info errors[] = {
	{ERR_SUCCESS, "Success"},
	{ERR_FUNC_PARAMS, "Error in function parameters"},
	{ERR_FOPENCLOSE, "Error in file open/close"},
	{ERR_FREAD, "Error in fread"},
	{ERR_FWRITE, "Error in fwrite"},
	{ERR_ALLOC, "Error in memory allocation"},
	{ERR_INPUT, "Error in akinator's input"}
};
