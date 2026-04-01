// Allows the user to edit some text in their editor of choice and commit it to RAM for program use, vis-a-vis git commit

#include "stdlib.h"
#include "stdio.h"

#define TEST_FILE "test_file_IGN.txt"

#define RAMBLINGS "Lorem ipsum dolor sit amet consectetur adipiscing elit. Quisque faucibus ex sapien vitae pellentesque sem placerat. In id cursus mi pretium tellus duis convallis. Tempus leo eu aenean sed diam urna tempor. Pulvinar vivamus fringilla lacus nec metus bibendum egestas. Iaculis massa nisl malesuada lacinia integer nunc posuere. Ut hendrerit semper vel class aptent taciti sociosqu. Ad litora torquent per conubia nostra inceptos himenaeos.\n"

int main()
{
    // Get env var $EDITOR, else use vi
    char *editor = getenv("EDITOR"), backup_editor[] = "vi";
    if (!editor)
    {
        editor = backup_editor;
    }

    FILE *write = fopen(TEST_FILE, "w");
    if (!write)
    {
        return 1;
    }

    fputs(RAMBLINGS, write);

    if (fclose(write))
    {
        return 1;
    }

    char cmd_buff[128];
    sprintf(cmd_buff, "%s " TEST_FILE, editor);
    system(cmd_buff);

    FILE *read = fopen(TEST_FILE, "r");
    if (!read)
    {
        return 1;
    }

    char file_buff[sizeof(RAMBLINGS) + 512];
    fgets(file_buff, sizeof(RAMBLINGS) + 512, read);

    if (fclose(read) || file_buff[0] == '\n')
    {
        return 1;
    }

    printf("Finished:\n%s\n", file_buff);
}
