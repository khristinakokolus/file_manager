//
// Created by monberin on 14.01.22.
//

#ifndef FILEMANAGER_ARCHIVING_H
#define FILEMANAGER_ARCHIVING_H

#include <sys/types.h>

#include <sys/stat.h>

#include <archive.h>
#include <archive_entry.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <boost/filesystem.hpp>

void write_archive(const char *filename);

#endif //FILEMANAGER_ARCHIVING_H
