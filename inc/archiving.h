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

static int copy_data(struct archive *ar, struct archive *aw);
static int extract(const char *filename);
void write_archive(const char *filename);
std::vector<std::string>list_entries(const char *filepath);

#endif //FILEMANAGER_ARCHIVING_H
