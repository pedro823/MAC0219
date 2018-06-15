#pragma once

/*--
    Error checker and handler for CUDA functions.
  --*/

#define errorCheck() cudaErrorHandler(__FILE__, __LINE__)

void cudaErrorHandler(const char *file, int line);
