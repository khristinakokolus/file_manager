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

int copy_data(struct archive *ar, struct archive *aw);
int extract(const char *dirname);
std::vector<std::string>list_entries(const char *filepath);

#endif //FILEMANAGER_ARCHIVING_H
