// Moving files across filesystems on Linux

#include "stdio.h"
#include "unistd.h"
#include "errno.h"
#include "sys/sendfile.h"
#include "stdlib.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "stdbool.h"

int lc_rename(const char *const old, const char *const new, const bool fail_if_exists)
{
    LC_RENAME_START:

    if (!rename(old, new))
    {
        return 0;
    }

    switch (errno)
    {
        break; case EEXIST:
        {
            if (fail_if_exists)
            {
                return 1;
            }

            if (remove(new))
            {
                return 1;
            }

            goto LC_RENAME_START;
        }
        break; case EXDEV:
        {
            if (0 == access(new, F_OK))
            {
                if (fail_if_exists)
                {
                    errno = EEXIST;
                    return 1;
                }
                else if (remove(new))
                {
                    return 1;
                }
            }

            const int fd_old = open(old, O_RDONLY);
            if (-1 == fd_old)
            {
                return 1;
            }

            const int fd_new = open(new, O_WRONLY | O_CREAT | O_EXCL);
            if (-1 == fd_new)
            {
                close(fd_old);
                return 1;
            }

            struct stat old_stat;
            if (fstat(fd_old, &old_stat))
            {
                return 1;
            }

            if (old_stat.st_size != sendfile(fd_new, fd_old, NULL, old_stat.st_size))
            {
                return 1;
            }

            if (close(fd_old))
            {
                return 1;
            }

            if (close(fd_new))
            {
                return 1;
            }

            if (chmod(new, old_stat.st_mode))
            {
                perror("wow");
                remove(new);

                return 1;
            }

            return 0;
        }
    }

    return 1;
}

int main(const int argc, const char *const *const argv)
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s [SOURCE] [DEST] [OVERWRITE:Y/N]\n", argv[0]);
        return 1;
    }

    if (lc_rename(argv[1], argv[2], argv[3][0] == 'N'))
    {
        perror("Rename error");
        return 1;
    }
}
