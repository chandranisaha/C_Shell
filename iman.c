#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "iman.h"

// Function to strip HTML tags from a given string
void strip_html_tags(char *str)
{
    
    char *src = str;
    char *dst = str;
    int inside_tag = 0;

    while (*src)
    {
        if (*src == '<')
        {
            inside_tag = 1;
        }
        else if (*src == '>')
        {
            inside_tag = 0;
            src++;
            continue;
        }

        if (!inside_tag)
        {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}

// Function to separate the HTTP header from the body
// Function to separate the HTTP header from the body
void separate_header_from_body(char *response, char **body)
{
    char *header_end = strstr(response, "\r\n\r\n"); // Find the end of the header
    if (header_end != NULL)
    {
        // Move past the header and extra newlines or spaces
        *body = header_end + 4;

        // Skip additional newlines or spaces if any
        while (**body == '\r' || **body == '\n' || **body == ' ')
        {
            (*body)++;
        }
    }
    else
    {
        *body = response; // If no header found, assume entire response is body
    }
}


// Function to fetch the man page for a given command
void fetch_man_page(char *command_name)
{
    command_name += 5;  // Skip the 'iman ' part
    // printf("%s\n",command_name);
    // return;
    char * cn = command_name;
    while (*command_name == ' ') command_name++; // Skip any leading spaces
    while(*cn!=' ' && *cn!='\0')
    {
        cn++;
    }
    if(*cn ==' ')
    *cn = '\0';

    int sockfd;
    struct sockaddr_in server_addr;
    struct hostent *server;
    
    char request[1024];
    char buffer[BUFFER_SIZE];
    int bytes_received;
    
    char *body_start;
    char content_buffer[BUFFER_SIZE * 10] = {0}; // Buffer to accumulate content
    size_t content_len = 0;

    // Step 1: Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        exit(1);
    }

    // Step 2: Get the server's address
    server = gethostbyname("man.he.net");
    if (server == NULL)
    {
        fprintf(stderr, "Error: No such host\n");
        exit(1);
    }

    // Step 3: Set up the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80); // HTTP uses port 80
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    // Step 4: Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error connecting to server");
        exit(1);
    }

    // Step 5: Create the HTTP GET request
    snprintf(request, sizeof(request),
             "GET /?topic=%s&section=1 HTTP/1.1\r\n"
             "Host: man.he.net\r\n"
             "Connection: close\r\n\r\n",
             command_name);

    // Step 6: Send the request to the server
    if (send(sockfd, request, strlen(request), 0) < 0)
    {
        perror("Error sending request");
        exit(1);
    }

    // Step 7: Read and accumulate the response from the server
    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0)
    {
        buffer[bytes_received] = '\0'; // Null-terminate the received data
        strcat(content_buffer, buffer); // Accumulate content
        content_len = strlen(content_buffer);
    }

    if (bytes_received < 0)
    {
        perror("Error receiving data");
    }

    // Step 8: Close the socket
    close(sockfd);

    // Separate header from body
    separate_header_from_body(content_buffer, &body_start);

    // Strip HTML tags from the accumulated content (body)
    strip_html_tags(body_start);
    while (!(*body_start == 'h' && *(body_start+1) == 't' &&*(body_start+2) == 'm' &&*(body_start+3) == 'l')  )
        {
            (body_start)++;
        }
        (body_start)+=4;

    // Print the cleaned content
    printf("%s\n", body_start);
}
