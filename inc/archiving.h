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
#include <zip.h>

int copy_data(struct archive *ar, struct archive *aw);
int extract(const char *dirname);
std::vector<std::string>list_entries(const char *filepath);
void find_files(const std::string& archive_dir, const std::string& search_dir, zip_t *zip_creator);
void make_archive(const std::string& dir_to_archive, const std::string& archive_name);

#endif //FILEMANAGER_ARCHIVING_H
