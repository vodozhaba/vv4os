/******************************************************************************
 * File:       /vv4os/mem/alloc.c                                             *
 * Author:     vodozhaba                                                      *
 * Created on: Apr 20, 2017                                                   *
 * Purpose:    Provides malloc group of functions.                            *
 ******************************************************************************/

#include <stdbool.h>
#include <stddef.h>
#include "mem/virt_mem_mgr.h"
#include "stdlib/stdlib.h"

#define BIG_SIGNATURE ((size_t) 0xB0A013F4)
#define SMALL_SIGNATURE ((size_t) 0xAFCBF74C)

// Here, I use the term 'block'.
// Every block has a metadata frame with block entries.
// There are as much block entries as it is possible to fit into the metadata frame.
// Every block entry describes a data frame.
// In addition, there are two words (size_t, not 16-bit words) before every allocated item.

#define WORD_SIZE (sizeof(size_t) * 8)
#define WORDS_PER_BITMAP (FRAME_SIZE / WORD_SIZE)

typedef struct BlockEntry {
	struct BlockEntry* next;
	void* data_frame;
	// Let's make a little assumption that a byte is 8 bits. I'm tired of this portability.
	size_t bitmap[WORDS_PER_BITMAP];
} BlockEntry;

#define ENTRIES_PER_BLOCK (FRAME_SIZE / sizeof(BlockEntry))

static BlockEntry* bitmap_list = NULL;

static void AllocateBlock() {
	BlockEntry* metadata_frame = AllocateContiguousVirtualFrames(1);
	memset(metadata_frame, 0, FRAME_SIZE);
	for(size_t i = 0; i < ENTRIES_PER_BLOCK - 1; i++) {
		metadata_frame[i].next = &metadata_frame[i + 1];
	}
	metadata_frame[ENTRIES_PER_BLOCK - 1].next = bitmap_list;
	for(size_t i = 0; i < ENTRIES_PER_BLOCK; i++) {
		metadata_frame[i].data_frame = AllocateContiguousVirtualFrames(1);
	}
	bitmap_list = metadata_frame;
}

static void MarkInBitmap(BlockEntry* entry, size_t offset, size_t size, bool value) {
	for(size_t i = offset; i < offset + size; i++) {
		if(value)
			entry->bitmap[i / WORD_SIZE] |= 1 << (i % WORD_SIZE);
		else
			entry->bitmap[i / WORD_SIZE] &= ~(1 << (i % WORD_SIZE));
	}
}

static void* SmallMalloc(size_t size) {
	for(BlockEntry* entry = bitmap_list; entry != NULL; entry = entry->next) {
		size_t found = 0;
		for(size_t i = 0; i < FRAME_SIZE; i++) {
			bool allocated = (entry->bitmap[i / WORD_SIZE] >> (i % WORD_SIZE)) & 1;
			if(allocated)
				found = 0;
			else {
				found++;
				if(found >= size) {
					size_t offset = i - found + 1;
					MarkInBitmap(entry, offset, size, true);
					size_t* addr = entry->data_frame + offset;
					addr[0] = SMALL_SIGNATURE;
					addr[1] = size;
					return &addr[2];
				}
			}
		}
	}
	AllocateBlock();
	return SmallMalloc(size);
}

// For (approximately) more than one frame
static void* BigMalloc(size_t size) {
	size_t frames = size / FRAME_SIZE + (size % FRAME_SIZE ? 1 : 0);
	size_t* addr = AllocateContiguousVirtualFrames(frames);
	if(addr == NULL)
		return NULL;
	addr[0] = BIG_SIGNATURE;
	addr[1] = frames;
	return &addr[2];
}

void* malloc(size_t size) {
	size += 2; // including metadata
	if(size >= FRAME_SIZE) {
		return BigMalloc(size);
	} else
		return SmallMalloc(size);
}

BlockEntry* FindEntryByFrame(void* data_frame) {
	for(BlockEntry* entry = bitmap_list; entry != NULL; entry = entry->next) {
		if(entry->data_frame == data_frame) return entry;
	}
	return NULL;
}

void SmallFree(void* addr) {
	size_t addr_as_word = (size_t) addr;
	void* data_frame = (void*)(addr_as_word / FRAME_SIZE * FRAME_SIZE);
	BlockEntry* entry = FindEntryByFrame(data_frame);
	size_t offset = addr_as_word % FRAME_SIZE - 2 * sizeof(size_t);
	size_t size = ((size_t*) addr)[-1];
	MarkInBitmap(entry, offset, size, false);
}

void BigFree(void* addr) {
	size_t frames = ((size_t*) addr)[-1];
	FreeContiguousVirtualFrames(addr - 2, frames);
}

void free(void* addr) {
	size_t signature = ((size_t*) addr)[-2];
	if(signature == BIG_SIGNATURE)
		BigFree(addr);
	else if(signature == SMALL_SIGNATURE)
		SmallFree(addr);
}
