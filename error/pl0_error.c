#include "pl0_error.h"

void PL0_print_error(char * err_msg, char* cur_line_content, int cur_line_num, int offset)
{
	fprintf(stderr,"[ERR] Error found in line %d, Type:%s\n%s\n", cur_line_num, err_msg, cur_line_content);
        //printf("[ERR] Error found in line %d, Type:%s \n %s\n", cur_line_num, err_msg, cur_line_content);
	int i=0;
	for (i = 0; i < offset; i++)
	{
		if (cur_line_content[i] == '\t')
		{
			fprintf(stderr, "\t");
			//printf("\t");
		}
		else
		{
			fprintf(stderr, " ");
			//fprintf(" ");
		}
	}
	fprintf(stderr, "^\n\n");
//fprintf("^\n\n");
}

