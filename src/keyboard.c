#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include "keyboard.h"

#define TERM_BUF_SIZE 4096 //max buffer size for the terminal
#define TERM_MAX_COLS 20 //max amount of colons the term can display
// #define TERM_MAX_LINES ?? //max amount of lines the term can display (unused yet)

char termBuffer[TERM_BUF_SIZE] = "#~ ";
char cmdOut[TERM_BUF_SIZE] = ""; //output for command
int termLen = 3;
int skipSeq = 0; //flag to ignore ANSI sequences

void executeCommand(const char *cmd) {
    FILE *fp;
    char result[256];
    char cmdLower[256];

    //convert command to lowercase so the shell can use it 
    for (int i = 0; cmd[i] != '\0' && i < sizeof(cmdLower) - 1; i++) {
        cmdLower[i] = tolower((unsigned char)cmd[i]);
        cmdLower[i + 1] = '\0';
    }

    fp = popen(cmdLower, "r");
    if (fp == NULL) {
        strcat(termBuffer, "\nERROR: FAILED TO RUN COMMAND\n#~ ");
        return;
    }

    cmdOut[0] = '\0';

    //read output from stdin
    while (fgets(result, sizeof(result), fp) != NULL) {
        //convert each char into uppercase for displaying with drawText
        for (int i = 0; result[i] != '\0'; i++) {
            result[i] = toupper((unsigned char)result[i]);
            if (result[i] == 0x1B) {skipSeq = 1;continue;}
        }
        strcat(cmdOut, result);
    }

    pclose(fp);

    //disp. cmd + result
    snprintf(termBuffer, TERM_BUF_SIZE, "#~ %.2000s\n%.2000s#~ ", cmd, cmdOut); //compiler warning here (idek?) ((Update: fixed with "%.2000s"))
    termBuffer[TERM_BUF_SIZE] = '\0';
}

void initKeyboard() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

void handleKeyboard() {
    char c;
    while (read(STDIN_FILENO, &c, 1) > 0) {

        if (skipSeq) {if ((c >= 'A' && c <= 'D') || c == '~') {skipSeq = 0;}continue;}
        if (c == 0x1B) {skipSeq = 1;continue;}
        
        c = toupper((unsigned char)c);
        int len = strlen(termBuffer);

        if (c == '\n' || c == '\r') {
            //extract last command after last "#~ "
            char *lastPrompt = strrchr(termBuffer, '~');
            if (lastPrompt) {
                char cmd[256];
                strncpy(cmd, lastPrompt + 2, sizeof(cmd) - 1);
                cmd[sizeof(cmd) - 1] = '\0';

                //trim newline/spaces
                for (int i = strlen(cmd) - 1; i >= 0 && (cmd[i] == '\n' || cmd[i] == ' '); i--)
                    cmd[i] = '\0';

                if (strlen(cmd) > 0)
                    executeCommand(cmd);
                else
                    strcat(termBuffer, "\n#~ ");
            } else {
                strcat(termBuffer, "\n#~ ");
            }
        }
        else if (c == 127 || c == 8) { // backspace
            if (len > 3 && termBuffer[len - 1] != '\n') {
                termBuffer[len - 1] = '\0';
            }
        }
        else if (isprint(c)) {
            int charsInLine = 0;
            for (int i = len - 1; i >= 0 && termBuffer[i] != '\n'; i--)
                charsInLine++;

            if (charsInLine >= TERM_MAX_COLS + 3)
                strcat(termBuffer, "\n#~ ");

            if (len < TERM_BUF_SIZE - 1) {
                termBuffer[len] = c;
                termBuffer[len + 1] = '\0';
            }
        }
    }
}

// void handleKeyboard() {
//     char c;
//     while (read(STDIN_FILENO, &c, 1) > 0) {
//         c = toupper((unsigned char)c);
//         if (c == 27) return; // ESC key to quit
//         if (c == 127 || c == 8) { // backspace
//             if (termLen < TERM_BUF_SIZE - 1) termBuffer[termLen--] = ' ';
//         } else if (c == '\n' || c == '\r') {
//             if (termLen < TERM_BUF_SIZE - 4) {
//                 strcat(termBuffer, "\n#~ ");
//             }
//             if (termLen < TERM_BUF_SIZE - 1) termBuffer[termLen++] = '\n';
//         } else if (c >= 32 && c <= 126) {
//             if (termLen < TERM_BUF_SIZE - 1) termBuffer[termLen++] = c;
//         }

//         // cut line if longer than TERM_MAX_COLS
//         int count = 0;
//         for (int i = 0; i < termLen; i++) {
//             if (termBuffer[i] == '\n') count = 0;
//             else if (++count > TERM_MAX_COLS) {
//                 // insert newline
//                 if (termLen < TERM_BUF_SIZE - 1) {
//                     for (int j = termLen; j > i; j--) termBuffer[j] = termBuffer[j - 1];
//                     termBuffer[i] = '\n';
//                     termLen++;
//                 }
//                 count = 0;
//             }
//         }
//     }
// }
