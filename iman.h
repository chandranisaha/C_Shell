#ifndef IMAN_H
#define IMAN_H
#include <stddef.h>
#define BUFFER_SIZE 4096
void fetch_man_page(char *command_name) ;
void separate_header_from_body(char *response, char **body);
void strip_html_tags(char *str);
// void remove_http_headers(char *response);
// void remove_initial_lines_until_content_type(char *response);
#endif // IMAN_H