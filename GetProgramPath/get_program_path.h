#ifndef __GET_PROGRAM_PATH_H
#define __GET_PROGRAM_PATH_H

// Returns a string of the absolute path of the running executable using argv[0]. Return must be freed by user.
char *get_program_path(const char * const argv0);

#endif // __GET_PROGRAM_PATH_H
