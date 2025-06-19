#include "cline.h"
#include "io.h"
#include "timer.h"
#include "terminal.h"

#define CMDBUFSIZ (8192)
#define MAXARGS (1024)
#define PROMPT "> "

static char** read_command(char* buf, size_t bufsiz);
__attribute__((used))static void process_command(char** command);

void launch_command_line(){
    char buf[CMDBUFSIZ];
    while(1){
        printf(PROMPT);
        char** args = read_command(buf, CMDBUFSIZ);
        /*
        printf("Arguments debug:\n");
        for(size_t i = 0; args[i] != NULL; i++){
            printf("%d) %s\n", i+1, args[i]);
        }
        */
        if(args != NULL && args[0] != NULL)
            process_command(args);
    }
}

static char** read_command(char* buf, size_t bufsiz){
    static char* args_out[MAXARGS];

    size_t sz = getline(buf, bufsiz-1);
    if(sz > 0 && buf[sz-1] == '\n')
        buf[sz-1] = '\0';
    else
        buf[sz] = '\0';

    //skip trailing spaces
    for (;isspace(*buf);++buf);

    size_t args_count = 0;
    size_t i = 0;
    while (args_count < MAXARGS-1 && buf[i] != '\0') {
        //push new argument
        args_out[args_count++] = buf+i;
        //skip argument's characters
        for(;buf[i] != '\0' && !isspace(buf[i]); ++i);
        if(buf[i] == '\0')
            break;
        buf[i++] = '\0';
        //skip spaces
        for(;buf[i] != '\0' && isspace(buf[i]); ++i);
    }
    args_out[args_count] = NULL;
    return args_out;
}

__attribute__((used))static void process_command(char** command){
    if(strcmp("help", command[0]) == 0){
        printf("\"time\" - print time since boot\n"
               "\"echo\" - print given arguments\n"
               "\"clear\" - clear the screen\n");
    }else if(strcmp("time", command[0]) == 0){
        printf("%s\n", gettime());
    }else if(strcmp("echo", command[0]) == 0){
        for(size_t i = 1; command[i] != NULL; ++i)
            printf("%s ", command[i]);
        putchar('\n');
    }else if(strcmp("clear", command[0]) == 0){
        clear_screen();
        move_cursor(0, 0);
    }else{
        printf("Undefined command: \"%s\",\n"
            "type \"help\" for command list\n", command[0]);
    }
}