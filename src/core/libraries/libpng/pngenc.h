// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "common/types.h"

namespace Core::Loader {
class SymbolsResolver;
}

namespace Libraries::PngEnc {

struct OrbisPngEncCreateParam {
    uint32_t this_size;
    uint32_t attribute;
    uint32_t max_image_width;
    uint32_t max_filter_number;
};

struct OrbisPngEncEncodeParam {
    const void* image_mem_addr;
    void* png_mem_addr;
    uint32_t image_mem_size;
    uint32_t png_mem_size;
    uint32_t image_width;
    uint32_t image_height;
    uint32_t image_pitch;
    uint16_t pixel_format;
    uint16_t color_space;
    uint16_t bit_depth;
    uint16_t clut_number;
    uint16_t filter_type;
    uint16_t compression_level;
};

struct OrbisPngEncOutputInfo {
    uint32_t data_size;
    uint32_t processed_height;
};

using OrbisPngEncHandle = void*;

s32 PS4_SYSV_ABI scePngEncCreate(const OrbisPngEncCreateParam* param, void* memoryAddress,
                                 uint32_t memorySize, OrbisPngEncHandle* handle);
s32 PS4_SYSV_ABI scePngEncDelete(OrbisPngEncHandle handle);
s32 PS4_SYSV_ABI scePngEncEncode(OrbisPngEncHandle handle, const OrbisPngEncEncodeParam* param,
                                 OrbisPngEncOutputInfo* outputInfo);
s32 PS4_SYSV_ABI scePngEncQueryMemorySize(const OrbisPngEncCreateParam* param);

void RegisterlibScePngEnc(Core::Loader::SymbolsResolver* sym);
} // namespace Libraries::PngEnc