// File:       io/disk/file.h
// Author:     vodozhaba
// Created on: Jan 03, 2017
// Purpose:    Describes the file descriptor structure.

#pragma once

#include <stddef.h>
#include <stdint.h>

struct FileDescriptor;
typedef struct FileDescriptor FileDescriptor;

typedef FileDescriptor* (*FileTraverseOp)(FileDescriptor* parent, char* name);
typedef size_t (*FileReadOp)(FileDescriptor* file, size_t size, void* buf);
typedef size_t (*FileWriteOp)(FileDescriptor* file, size_t size, const void* buf);

struct Volume;
typedef struct Volume Volume;

typedef enum {
    FD_TYPE_FILE,
    FD_TYPE_DIRECTORY
} FileDescriptorType;

struct FileDescriptor {
    uint64_t seek;
    uint64_t size;
    Volume* volume;
    void* data;
    FileTraverseOp traverse_op;
    FileReadOp read_op;
    FileWriteOp write_op;
    uint32_t local_id;
    FileDescriptorType type;
    struct FileDescriptor* next;
};

size_t FileAccessOpStub();
FileDescriptor* FileTraverseOpStub();