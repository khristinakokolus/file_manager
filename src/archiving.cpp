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

//int main(int argc, const char **argv)
//{
//    write_archive("/home/zoria/Documents/arch/archive_test");
//    return 0;
//}