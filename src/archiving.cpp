#include <sys/types.h>

#include <sys/stat.h>

#include <archive.h>
#include <archive_entry.h>
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <string>
#include <boost/filesystem.hpp>


std::vector<std::string> list_entries(const char *filepath){

    struct archive *a;
    struct archive_entry *entry;
    int r;
    std::vector<std::string> entries;

    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);
    r = archive_read_open_filename(a, filepath, 10240); // Note 1
    if (r != ARCHIVE_OK)
        exit(1);
    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
        entries.emplace_back(archive_entry_pathname(entry));
        archive_read_data_skip(a);  // Note 2
    }
    r = archive_read_free(a);  // Note 3
    if (r != ARCHIVE_OK)
        exit(1);
    return entries;
}


void write_archive(const char *filename)
{
    struct archive *a;
    struct archive_entry *entry;
    struct stat st;
    char buff[8192];
    int len;
    int fd;

    std::string new_path = filename;
    std::string ext = ".zip"; // .tar or anything else

    if (boost::filesystem::exists(new_path+ext)) {
        int num = 1;
        while (true) {
            std::string s = std::to_string(num);
            new_path += s;
            if (!boost::filesystem::exists(new_path+ext)) {
                break;
            }
            new_path.pop_back();
            num++;
        }
    }

    new_path += ext;
    const char *outname = new_path.c_str();


    a = archive_write_new();
    archive_write_add_filter_gzip(a);   // can be changed to other format
    archive_write_set_format_pax_restricted(a); // Note 1
    archive_write_open_filename(a, outname);
    while (*filename) {
        stat(filename, &st);
        entry = archive_entry_new(); // Note 2
        archive_entry_set_pathname(entry, filename);
        archive_entry_set_size(entry, st.st_size); // Note 3
        archive_entry_set_filetype(entry, AE_IFREG);
        archive_entry_set_perm(entry, 0644);
        archive_write_header(a, entry);
        fd = open(filename, O_RDONLY);
        len = read(fd, buff, sizeof(buff));
        while ( len > 0 ) {
            archive_write_data(a, buff, len);
            len = read(fd, buff, sizeof(buff));
        }
        close(fd);
        archive_entry_free(entry);
        filename++;
    }
    archive_write_close(a); // Note 4
    archive_write_free(a); // Note 5
}

