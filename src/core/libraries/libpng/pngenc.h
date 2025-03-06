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
    void* image_mem_addr;
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

struct OrbisPngEncoderInfo {
    uintptr_t self_address;
    u64 this_size;
    u32 image_width;
    u32 max_filters;
    u32 image_size;
};

using OrbisPngEncHandle = void*;

enum OrbisPngEncAttribute {
    ORBIS_PNG_ENC_ATTRIBUTE_NONE = 0
};

enum OrbisPngEncColorSpace {
    ORBIS_PNG_ENC_COLOR_SPACE_RGB = 3,
    ORBIS_PNG_ENC_COLOR_SPACE_RGBA = 19
};

enum OrbisPngEncPixelFormat {
    ORBIS_PNG_ENC_PIXEL_FORMAT_R8G8B8A8 = 0,
    ORBIS_PNG_ENC_PIXEL_FORMAT_B8G8R8A8 = 1
};

enum OrbisPngEncFilterType {
    ORBIS_PNG_ENC_FILTER_TYPE_NONE = 0,
    ORBIS_PNG_ENC_FILTER_TYPE_SUB = 1,
    ORBIS_PNG_ENC_FILTER_TYPE_UP = 2,
    ORBIS_PNG_ENC_FILTER_TYPE_AVG = 4,
    ORBIS_PNG_ENC_FILTER_TYPE_PAETH = 8,
    ORBIS_PNG_ENC_FILTER_TYPE_ALL = 15
};

s32 PS4_SYSV_ABI scePngEncCreate(const OrbisPngEncCreateParam* param, void* memoryAddress,
                                 uint32_t memorySize, OrbisPngEncHandle* handle);
s32 PS4_SYSV_ABI scePngEncDelete(OrbisPngEncHandle handle);
s32 PS4_SYSV_ABI scePngEncEncode(OrbisPngEncHandle handle, const OrbisPngEncEncodeParam* param,
                                 OrbisPngEncOutputInfo* outputInfo);
s32 PS4_SYSV_ABI scePngEncQueryMemorySize(const OrbisPngEncCreateParam* param);

void RegisterlibScePngEnc(Core::Loader::SymbolsResolver* sym);
} // namespace Libraries::PngEnc