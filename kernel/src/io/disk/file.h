// File:       io/disk/file.h
// Author:     vodozhaba
// Created on: Jan 03, 2017
// Purpose:    Describes the file descriptor structure.

#pragma once

struct FileDescriptor;
typedef struct FileDescriptor FileDescriptor;

typedef FileDescriptor* (*FileTraverseOp)(FileDescriptor* parent, char* name);

struct Volume;
typedef struct Volume Volume;

struct FileDescriptor {
    Volume* volume;
    void* data;
    FileTraverseOp traverse_op;
};
