/**
 * @file test_helpers.c
 * @brief Implementation of helper functions for testing
 *
 * @author Claude
 * @date March 25, 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "test_helpers.h"

/**
 * @brief Captures stdout during function execution
 *
 * Redirects stdout to a pipe, executes the provided function,
 * and captures the output in the provided buffer.
 *
 * @param func Function to execute with captured stdout
 * @param buffer Buffer to store captured output
 * @param buffer_size Size of the buffer
 * @return bool true on success, false on error
 */
bool capture_stdout(void (*func)(void), char *buffer, size_t buffer_size)
{
    int stdout_backup;
    int pipe_fd[2];

    /* Back up the original stdout file descriptor */
    stdout_backup = dup(STDOUT_FILENO);
    if (stdout_backup == -1) {
        return false;
    }

    /* Create a pipe */
    if (pipe(pipe_fd) == -1) {
        close(stdout_backup);
        return false;
    }

    /* Redirect stdout to the pipe */
    if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        close(stdout_backup);
        return false;
    }

    /* Execute the function with redirected stdout */
    func();

    /* Flush stdout to make sure all output is in the pipe */
    fflush(stdout);

    /* Restore the original stdout */
    if (dup2(stdout_backup, STDOUT_FILENO) == -1) {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        close(stdout_backup);
        return false;
    }

    /* Close the write end of the pipe */
    close(pipe_fd[1]);

    /* Read the captured output from the pipe */
    memset(buffer, 0, buffer_size);
    ssize_t bytes_read = read(pipe_fd[0], buffer, buffer_size - 1);
    if (bytes_read < 0) {
        close(pipe_fd[0]);
        close(stdout_backup);
        return false;
    }

    /* Ensure null termination */
    buffer[bytes_read] = '\0';

    /* Clean up */
    close(pipe_fd[0]);
    close(stdout_backup);

    return true;
}