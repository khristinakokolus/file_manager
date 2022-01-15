# Сross-platform orthodox GUI file manager

Welcome to the Сross-platform orthodox GUI file manager project!

## Contributors
[Khrystyna Kokolus](https://github.com/khristinakokolus)

[Daryna Hnatenko](https://github.com/monberin)

[Zoryana Herman](https://github.com/zoriankaH)


## Description

The main idea of this project is to create cross-platform orthodox GUI file manager using Qt. 

At the moment such functionality is implemented:

- txt files opening
- deletion of files and folders
- files and folders copying
- files and folders renaming
- error handling when copying and deleting files, folders
- files search
- files and folders creation
- files opening in hex view
- running exe programs
- folders arhivation and unarchivation, possibility to see what files are in archive

## Prerequisites

If you want to work with this project you should firstly write in the command prompt:


```
git clone https://github.com/khristinakokolus/file_manager.git
```

Also you need:

- **C++ compiler** - needs to support **C++17** standard
- **CMake** 3.16+
- **LibArchive**
- **LibZip**


## Usage

Running from command prompt:

```
cd file_manager
mkdir build && cd build && cmake .. && make
./filemanager
```

After that a file manager window will appear.

## GUI

Initial view of filesystem:

![Screenshot 2022-01-15 090528](https://user-images.githubusercontent.com/60686300/149612936-0d530ccf-eac3-4ee9-8779-a04b12979668.png)


Commands that are now supported:

![func](https://user-images.githubusercontent.com/60686300/149612968-88fe8716-0cef-4bea-8fff-89c1b3ba03f6.png)


## First presentation

[Final presentation slides](https://docs.google.com/presentation/d/1lUsVkebr7rmr7RD618irmHtaPKhTO3-rOvxqew0hpXY/edit#slide=id.p1)
