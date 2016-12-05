#ifndef ERR_H_
#define ERR_H_

typedef enum {
	ERR_SUCCESS = 0,
	ERR_FUNC_PARAMS = -1,
	ERR_FOPENCLOSE = -2,
	ERR_FREAD = -3,
	ERR_FWRITE = -4,
	ERR_ALLOC = -5,
	ERR_INPUT = -6,
} err_codes_t;

struct err_info {
	err_codes_t num;
	char *msg;
};

extern const struct err_info errors[];

#endif
