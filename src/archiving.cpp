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
#include <string>
#include <dirent.h>
#include <zip.h>

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


// Used code from https://stackoverflow.com/questions/40159892/using-asprintf-on-windows
#ifdef _WIN32
int vasprintf(char **strp, const char *fmt, va_list ap) {
    // _vscprintf tells you how big the buffer needs to be
    int len = _vscprintf(fmt, ap);
    if (len == -1) {
        return -1;
    }
    size_t size = (size_t)len + 1;
    char *str = static_cast<char *>(malloc(size));
    if (!str) {
        return -1;
    }
    // _vsprintf_s is the "secure" version of vsprintf
    int r = vsprintf_s(str, len + 1, fmt, ap);
    if (r == -1) {
        free(str);
        return -1;
    }
    *strp = str;
    return r;
}

// Used code from https://stackoverflow.com/questions/40159892/using-asprintf-on-windows
int asprintf_win(char **strp, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int r = vasprintf(strp, fmt, ap);
    va_end(ap);
    return r;
}
#endif

int copy_data(struct archive *ar, struct archive *aw)
{
    int r;
    const void *buff;
    size_t size;
    off_t offset;

    for (;;) {
        r = archive_read_data_block(ar, &buff, &size, reinterpret_cast<la_int64_t *>(&offset));
        if (r == ARCHIVE_EOF)
            return (ARCHIVE_OK);
        if (r < ARCHIVE_OK)
            return (r);
        r = archive_write_data_block(aw, buff, size, offset);
        if (r < ARCHIVE_OK) {
            fprintf(stderr, "%s\n", archive_error_string(aw));
            return (r);
        }
    }
}

int extract(const char *dirname){
    struct archive *a;
    struct archive *ext;
    struct archive_entry *entry;
    int flags;
    int r;

    flags = ARCHIVE_EXTRACT_TIME;
    flags |= ARCHIVE_EXTRACT_PERM;
    flags |= ARCHIVE_EXTRACT_ACL;
    flags |= ARCHIVE_EXTRACT_FFLAGS;

    a = archive_read_new();
    archive_read_support_format_all(a);
    ext = archive_write_disk_new();
    archive_write_disk_set_options(ext, flags);
    archive_write_disk_set_standard_lookup(ext);
    if ((r = archive_read_open_filename(a, dirname, 10240)))
        return 1;

    boost::filesystem::path dir(dirname);

    std::string dest_file_str = dir.stem().string();
    char * dest_file = const_cast<char *>(dest_file_str.c_str());
    std::string temp_dir_str = dir.remove_filename().string();
    char * temp_dir = const_cast<char *>(temp_dir_str.c_str());

    for (;;) {
        r = archive_read_next_header(a, &entry);
        if (r == ARCHIVE_EOF)
            break;
        if (r < ARCHIVE_OK)
            fprintf(stderr, "%s\n", archive_error_string(a));
        if (r < ARCHIVE_WARN)
            return -1;

#ifdef _WIN32
        asprintf_win(&dest_file, "%s/%s", temp_dir, archive_entry_pathname(entry));
#endif

#ifdef linux
        asprintf(&dest_file, "%s/%s", temp_dir, archive_entry_pathname(entry));
#endif

        archive_entry_set_pathname(entry, dest_file);

        r = archive_write_header(ext, entry);
        if (r < ARCHIVE_OK)
            fprintf(stderr, "%s\n", archive_error_string(ext));
        else if (archive_entry_size(entry) > 0) {
            r = copy_data(a, ext);
            if (r < ARCHIVE_OK)
                fprintf(stderr, "%s\n", archive_error_string(ext));
            if (r < ARCHIVE_WARN)
                exit(1);
        }
        r = archive_write_finish_entry(ext);
        if (r < ARCHIVE_OK)
            fprintf(stderr, "%s\n", archive_error_string(ext));
        if (r < ARCHIVE_WARN) {
            return -1;
        }

        free(dest_file);
    }
    archive_read_close(a);
    archive_read_free(a);
    archive_write_close(ext);
    archive_write_free(ext);
    return 0;
}


void find_files(const std::string& archive_dir, const std::string& search_dir, zip_t *zip_creator)
{
    struct dirent *dir;
    std::string dir_name;
    struct stat st;

    DIR *dir_pointer = opendir(search_dir.c_str());

    while ((dir = readdir(dir_pointer)) != NULL) {
        dir_name = dir->d_name;
        if (dir_name != "." && dir_name != "..") {
            std::string path = search_dir + "/" + dir_name;
            stat(path.c_str(), &st);

            std::string path_substr = path.substr(archive_dir.length() + 1);
            if (S_ISDIR(st.st_mode)) {
                zip_dir_add(zip_creator, path_substr.c_str(), ZIP_FL_ENC_UTF_8);
                find_files(archive_dir, path, zip_creator);
            } else {
                zip_file_add(zip_creator, path_substr.c_str(),
                             zip_source_file(zip_creator, path.c_str(), 0, 0), ZIP_FL_ENC_UTF_8);
            }
        }
    }
    closedir(dir_pointer);
}

void make_archive(const std::string& dir_to_archive, const std::string& archive_name)
{
    int error;
    zip_t *zip_creator = zip_open(archive_name.c_str(), ZIP_CREATE | ZIP_EXCL, &error);

    find_files(dir_to_archive, dir_to_archive, zip_creator);
    zip_close(zip_creator);
}