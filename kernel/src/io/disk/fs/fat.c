// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       io/disk/fs/fat.c
// Author:     vodozhaba
// Created on: Jan 03, 2017
// Purpose:    Provides file functions for FAT12/16/32 partitions.                                   

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "io/disk/disk.h"
#include "io/disk/fs/fat.h"
#include "io/disk/volume.h"

typedef struct {
    uint8_t hours : 5;
    uint8_t minutes : 6;
    uint8_t double_seconds : 5;
} __attribute__((packed)) FatTime;

typedef struct {
    uint8_t year : 7;
    uint8_t month : 4;
    uint8_t day : 5;
} __attribute__((packed)) FatDate;

typedef struct {
    uint8_t jump[3];
    char oem_id[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t fats;
    uint16_t directory_entries;
    uint16_t total_sectors;
    uint8_t media_descriptor_type;
    uint16_t sectors_per_fat;
    uint16_t sectors_per_track;
    uint16_t total_heads;
    uint32_t hidden_sectors;
    uint32_t total_sectors_large;
    union {
        struct {
            uint8_t drive_number;
            uint8_t reserved;
            uint8_t fat_signature;
            uint32_t volume_id;
            char label[11];
            char system_id[8];
            uint8_t boot_code[448];
        } __attribute__((packed)) fat12_16;
        struct {
            uint32_t sectors_per_fat;
            uint16_t flags;
            struct {
                uint8_t major;
                uint8_t minor;
            } __attribute__((packed)) version;
            uint32_t root_cluster_number;
            uint16_t fsinfo_sector_number;
            uint16_t backup_boot_sector;
            uint8_t reserved_1[12];
            uint8_t drive_number;
            uint8_t reserved_2;
            uint8_t fat_signature;
            uint32_t volume_id;
            char label[11];
            char system_id[8];
            uint8_t boot_code[420];
        } __attribute__((packed)) fat32;
    } ebr;
    uint16_t boot_signature;
} __attribute__((packed)) FatBootRecord;

#define FAT_ATTR_READ_ONLY 0x01
#define FAT_ATTR_HIDDEN    0x02
#define FAT_ATTR_SYSTEM    0x04
#define FAT_ATTR_VOLUME_ID 0x08
#define FAT_ATTR_DIRECTORY 0x10
#define FAT_ATTR_ARCHIVE   0x20
#define FAT_ATTR_LFN       0x0F

typedef union {
    struct {
        char name[8];
        char extension[3];
        uint8_t attr;
        uint8_t reserved;
        uint8_t creation_tenths_sec;
        FatTime creation_time;
        FatDate creation_date;
        FatDate accessed_date;
        uint16_t cluster_number_high;
        FatTime modification_time;
        FatDate modification_date;
        uint16_t cluster_number_low;
        uint32_t size;
    } __attribute__((packed)) sfn;
    struct {
        uint8_t number;
        uint16_t name_1[5];
        uint8_t attr;
        uint8_t type;
        uint8_t checksum;
        uint16_t name_2[6];
        uint16_t zero;
        uint16_t name_3[2];
    } __attribute__((packed)) lfn;
} __attribute__((packed)) FatDirectoryEntry;

typedef enum {
    FAT12,
    FAT16,
    FAT32
} FatType;

typedef struct {
    FatBootRecord br;
    FatType type;
    uint32_t first_data_sector;
    uint32_t bytes_per_cluster;
} FatVolumeInternalData;

typedef struct {
    uint32_t first_cluster;
} FatFileInternalData;

#define INVALID_CLUSTER 0
#define FAT_ENTRY_BAD_SECTOR 0x0FFFFFF8

static void* Fat32ReadCluster(Volume* volume, uint32_t cluster, void* buf) {
    FatVolumeInternalData* data = volume->data;
    FatBootRecord* br = &data->br;
    Lba48 first_sector = (cluster - 2) * br->sectors_per_cluster + data->first_data_sector;
    for(uint32_t i = 0; i < br->sectors_per_cluster; i++) {
        if(!PartitionReadSector(volume->disk, volume->partition, first_sector + i, buf + i * br->bytes_per_sector)) {
            return NULL;
        }
    }
    return buf;
}

static FileDescriptor* GenFileDescriptor(Volume* volume, FatDirectoryEntry* entry) {
    FileDescriptor* ret = malloc(sizeof(*ret));
    ret->seek = 0;
    ret->traverse_op = Fat32TraverseOp;
    ret->read_op = Fat32ReadOp;
    ret->volume = volume;
    if(entry->sfn.attr & FAT_ATTR_DIRECTORY) {
        ret->size = 0;
        ret->type = FD_TYPE_DIRECTORY;
    } else {
        ret->size = entry->sfn.size;
        ret->type = FD_TYPE_FILE;
    }
    FatFileInternalData* fiid = malloc(sizeof(*fiid));
    fiid->first_cluster = (uint32_t) entry->sfn.cluster_number_high << 16 | entry->sfn.cluster_number_low;
    ret->data = fiid;
    return ret;
}

static uint32_t NextCluster(Volume* volume, uint32_t cluster) {
    FatVolumeInternalData* fvid = volume->data;
    uint32_t* fat = malloc(fvid->br.bytes_per_sector);
    uint32_t entries_per_sector = fvid->br.bytes_per_sector / 4; // 32 bits
    PartitionReadSector(volume->disk, volume->partition, fvid->br.reserved_sectors + cluster / entries_per_sector, fat);
    uint32_t value = fat[cluster % entries_per_sector] & 0x0FFFFFFF;
    return value < FAT_ENTRY_BAD_SECTOR ? value : INVALID_CLUSTER;
}

size_t Fat32ReadOp(FileDescriptor* file, size_t size, void* buf) {
    FatVolumeInternalData* fvid = file->volume->data;
    FatFileInternalData* ffid = file->data;
    void* cluster_buf = malloc(fvid->bytes_per_cluster);
    size_t ret = 0;
    size_t leading = (fvid->bytes_per_cluster - file->seek % fvid->bytes_per_cluster) % fvid->bytes_per_cluster;
    uint32_t first_cluster = file->seek / fvid->bytes_per_cluster, cluster = ffid->first_cluster;
    for(size_t i = 0; i < first_cluster; i++) {
        cluster = NextCluster(file->volume, cluster);
        if(cluster == INVALID_CLUSTER) {
            free(cluster_buf);
            return 0;
        }
    }
    if(leading != 0) {
        if(!Fat32ReadCluster(file->volume, cluster, cluster_buf)) {
            free(cluster_buf);
            return 0;
        }
        memcpy(buf, cluster_buf + fvid->bytes_per_cluster - leading, leading);
        ret += leading;
        buf += leading;
        size -= leading;
        cluster = NextCluster(file->volume, cluster);
    }
    uint32_t entire_clusters = size / fvid->bytes_per_cluster;
    for(size_t i = 0; i < entire_clusters; i++) {
        ret += fvid->bytes_per_cluster;
        buf += fvid->bytes_per_cluster;
        size -= fvid->bytes_per_cluster;
        cluster = NextCluster(file->volume, cluster);
        if(cluster == INVALID_CLUSTER || !Fat32ReadCluster(file->volume, cluster, buf)) {
            free(cluster_buf);
            file->seek += ret;
            return ret;
        }
    }
    if(size != 0) {
        if(cluster == INVALID_CLUSTER || !Fat32ReadCluster(file->volume, cluster, cluster_buf)) {
            free(cluster_buf);
            file->seek += ret;
            return ret;
        }
        memcpy(buf, cluster_buf, size);
        ret += size;
    }
    free(cluster_buf);
    file->seek += ret;
    return ret;
}

FileDescriptor* Fat32TraverseOp(FileDescriptor* parent, char* name) {
    if(parent->type != FD_TYPE_DIRECTORY) {
        return NULL;
    }
    FatVolumeInternalData* fvid = parent->volume->data;
    FatFileInternalData* fiid = parent->data;
    FatDirectoryEntry* cluster = malloc(fvid->bytes_per_cluster);
    char* buf = malloc(256);
    char* seek = buf;
    for(uint32_t cluster_number = fiid->first_cluster; cluster_number != INVALID_CLUSTER; cluster_number = NextCluster(parent->volume, cluster_number)) {
        Fat32ReadCluster(parent->volume, cluster_number, cluster);
        // Yup, this is spaghetti. But this is FAT.
        for(size_t i = 0; i < fvid->bytes_per_cluster / sizeof(FatDirectoryEntry); i++) {
            FatDirectoryEntry* entry = &cluster[i];
            if((uint8_t) entry->sfn.name[0] == 0x00) {
                seek = buf;
                free(buf);
                free(cluster);
                return NULL;
            }
            if((uint8_t) entry->sfn.name[0] == 0xE5) {
                seek = buf;
                continue;
            }
            if(entry->sfn.attr == FAT_ATTR_LFN) {
                bool end = false;
                for(size_t i = 0; i < 5 && !end; i++) {
                    uint16_t c = entry->lfn.name_1[i];
                    *seek++ = (char) c;
                    if(c == 0x0000) {
                        end = true;
                    }
                }
                for(size_t i = 0; i < 6 && !end; i++) {
                    uint16_t c = entry->lfn.name_2[i];
                    *seek++ = (char) c;
                    if(c == 0x0000) {
                        end = true;
                    }
                }
                for(size_t i = 0; i < 2 && !end; i++) {
                    uint16_t c = entry->lfn.name_3[i];
                    *seek++ = (char) c;
                    if(c == 0x0000) {
                        end = true;
                    }
                }
            } else {
                if(seek == buf) {
                    int i;
                    for(i = 7; entry->sfn.name[i] == ' ' && i >= 0; i--);
                    size_t len = i + 1;
                    memcpy(seek, entry->sfn.name, len);
                    seek += len;
                    *seek++ = '.';
                    for(i = 2; entry->sfn.name[i] == ' ' && i >= 0; i--);
                    len = i + 1;
                    *seek = 0x00;
                    seek = buf;
                    if(strcmp(buf, name) == 0) {
                        FileDescriptor* ret = GenFileDescriptor(parent->volume, entry);
                        free(buf);
                        free(cluster);
                        return ret;
                    }
                } else {
                    seek = buf;
                    if(strcmp(buf, name) == 0) {
                        FileDescriptor* ret = GenFileDescriptor(parent->volume, entry);
                        free(buf);
                        free(cluster);
                        return ret;
                    }
                }
            }
        }
    }
    free(buf);
    free(cluster);
    return NULL;
}

static Volume* Fat32MountVolume(Volume* volume) {
    FatVolumeInternalData* volume_data = volume->data;
    FatBootRecord* br = &volume_data->br;
    volume_data->first_data_sector = br->reserved_sectors + br->fats * br->ebr.fat32.sectors_per_fat;
    volume_data->bytes_per_cluster = br->bytes_per_sector * br->sectors_per_cluster;
    FileDescriptor* file = malloc(sizeof(*file));
    file->volume = volume;
    file->traverse_op = Fat32TraverseOp;
    file->type = FD_TYPE_DIRECTORY;
    FatFileInternalData* file_data = malloc(sizeof(*file_data));
    file_data->first_cluster = volume_data->br.ebr.fat32.root_cluster_number;
    file->data = file_data;
    volume->root = file;
    return volume;
}

Volume* FatMountVolume(DiskDescriptor* disk, uint32_t partition) {
    FatVolumeInternalData* data = malloc(sizeof(*data));
    PartitionReadSector(disk, partition, 0, &data->br);
    if(data->br.boot_signature != 0xAA55) {
        free(data);
        return NULL;
    }
    Volume* ret = malloc(sizeof(*ret));
    ret->disk = disk;
    ret->data = data;
    ret->partition = partition;
    if(data->br.ebr.fat12_16.fat_signature == 0x28 || data->br.ebr.fat12_16.fat_signature == 0x29) {
        // Stop using outdated file systems!
        // TODO: actually, implement a FAT12/16 driver
        free(data);
        free(ret);
        return NULL;
    } else if(data->br.ebr.fat32.fat_signature == 0x28 || data->br.ebr.fat32.fat_signature == 0x29) {
        data->type = FAT32;
        return Fat32MountVolume(ret);
    } else {
        free(data);
        free(ret);
        return NULL;
    }
}
