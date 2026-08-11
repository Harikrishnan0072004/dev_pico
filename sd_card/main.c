#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hw_config.h"
#include "ff.h"

// Helper function to read a full line typed in the terminal
void get_cmd_line(char *buffer, int max_len) {
    int index = 0;
    while (index < max_len - 1) {
        int c = getchar();
        if (c == EOF || c == 255) {
            continue; // Wait for actual character input
        }
        if (c == '\r' || c == '\n') {
            buffer[index] = '\0';
            printf("\r\n"); // Move terminal cursor down
            break;
        }
        if (c == '\b' || c == 127) { // Handle backspace safely
            if (index > 0) {
                index--;
                printf("\b \b");
            }
            continue;
        }
        putchar(c); // Echo characters back to your screen
        buffer[index++] = c;
    }
}

// Command: List all files
void cmd_ls() {
    DIR dir;
    FILINFO fno;
    
    if (f_opendir(&dir, "0:") == FR_OK) {
        printf("\n?? Root Directory Contents:\n");
        
        // ? FIXED: Correctly check fno.fname[0] to stop when the directory ends
        while (f_readdir(&dir, &fno) == FR_OK && fno.fname[0] != '\0') {
            
            // ? FIXED: Check the first character to skip "." and ".." navigation paths
            if (fno.fname[0] == '.') continue;
            
            if (fno.fattrib & AM_DIR) {
                printf("  [DIR]  %s/\n", fno.fname);
            } else {
                printf("  [FILE] %-20s (%llu bytes)\n", fno.fname, (unsigned long long)fno.fsize);
            }
        }
        f_closedir(&dir);
        printf("\n");
    } else {
        printf("? Failed to read directory.\n");
    }
}

// Command: Read and display a file (cat)
void cmd_read(const char *filename) {
    FIL fil;
    char line[256]; // ? FIXED: Changed to an explicit 256-byte array buffer
    
    if (f_open(&fil, filename, FA_READ) == FR_OK) {
        printf("\n?? Contents of %s:\n-------------------\n", filename);
        while (f_gets(line, sizeof(line), &fil)) {
            printf("%s", line);
        }
        printf("\n-------------------\n\n");
        f_close(&fil);
    } else {
        printf("? Could not open file: %s\n", filename);
    }
}

// Command: Write new text to a file (overwrite/create)
void cmd_write(const char *filename, const char *text) {
    FIL fil;
    UINT bw;
    if (f_open(&fil, filename, FA_WRITE | FA_CREATE_ALWAYS) == FR_OK) {
        f_write(&fil, text, strlen(text), &bw);
        f_write(&fil, "\r\n", 2, &bw); // Add a clean newline
        f_close(&fil);
        printf("? Successfully wrote %s (%u bytes physical sync)\n", filename, bw);
    } else {
        printf("? Failed to edit/create file: %s\n", filename);
    }
}

// Command: Append text to an existing file
void cmd_append(const char *filename, const char *text) {
    FIL fil;
    UINT bw;
    if (f_open(&fil, filename, FA_WRITE | FA_OPEN_APPEND) == FR_OK) {
        f_write(&fil, text, strlen(text), &bw);
        f_write(&fil, "\r\n", 2, &bw);
        f_close(&fil);
        printf("? Appended data to %s\n", filename);
    } else {
        printf("? Failed to append to file: %s\n", filename);
    }
}

int main() {
    stdio_init_all();
    sleep_ms(3000); // Buffer window to open terminal tool

    printf("\n=======================================\n");
    printf("?? Pico SDK Interactive SD Shell Ready \n");
    printf("Available: ls, read <file>, write <file> <text>, append <file> <text>\n");
    printf("=======================================\n");

    FATFS fs;
    if (f_mount(&fs, "0:", 1) != FR_OK) {
        printf("? SD Card initialization completely failed!\n");
        while (true) { tight_loop_contents(); }
    }

    char cmd_buffer[256]; // ? FIXED: Changed to an explicit 256-byte array buffer

    while (true) {
        printf("pico-sd> ");
        get_cmd_line(cmd_buffer, sizeof(cmd_buffer));

        // Tokenize command name out of string buffer
        char *cmd = strtok(cmd_buffer, " ");
        if (!cmd) continue;

        if (strcmp(cmd, "ls") == 0) {
            cmd_ls();
        } 
        else if (strcmp(cmd, "read") == 0) {
            char *filename = strtok(NULL, " ");
            if (filename) cmd_read(filename);
            else printf("Usage: read <filename>\n");
        } 
        else if (strcmp(cmd, "write") == 0) {
            char *filename = strtok(NULL, " ");
            char *text = strtok(NULL, ""); // Grab all remaining characters as the text body
            if (filename && text) cmd_write(filename, text);
            else printf("Usage: write <filename> <text context string>\n");
        } 
        else if (strcmp(cmd, "append") == 0) {
            char *filename = strtok(NULL, " ");
            char *text = strtok(NULL, "");
            if (filename && text) cmd_append(filename, text);
            else printf("Usage: append <filename> <text string>\n");
        } 
        else {
            printf("? Unknown command! Use: ls, read, write, append\n");
        }
    }
    return 0;
}

