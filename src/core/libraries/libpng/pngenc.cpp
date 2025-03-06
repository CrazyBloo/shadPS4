// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include <png.h>
#include "common/logging/log.h"
#include "core/libraries/error_codes.h"
#include "core/libraries/libs.h"
#include "core/libraries/libpng/pngenc.h"
#include "core/libraries/libpng/pngenc_error.h"

namespace Libraries::PngEnc {

s32 PS4_SYSV_ABI scePngEncCreate(const OrbisPngEncCreateParam* param, void* memoryAddress,
                                 uint32_t memorySize, OrbisPngEncHandle* handle) {
    if (param == nullptr || memoryAddress == nullptr || handle == nullptr) {
        return ORBIS_PNG_ENC_ERROR_INVALID_ADDR;
    }

    LOG_ERROR(Lib_Png, "scePngEncCreate | params: size {}, attribute {}, width {}, filter {}, memorySize {}", param->this_size, param->attribute, param->max_image_width, param->max_filter_number, memorySize);

    if (param->this_size != 0x10) {
        return ORBIS_PNG_ENC_ERROR_INVALID_SIZE;
    }

    if (param->attribute != ORBIS_PNG_ENC_ATTRIBUTE_NONE
        || param->max_image_width - 1 >= 1000000
        || param->max_filter_number >= 5) {
        return ORBIS_PNG_ENC_ERROR_INVALID_PARAM;
    }

    if ((param->max_image_width * 4 + 0x1c & 0xffffffe0) * (param->max_filter_number + 2) + 0x45c70 > memorySize) {
        return ORBIS_PNG_ENC_ERROR_INVALID_SIZE;
    }

    OrbisPngEncoderInfo* encoder_info = reinterpret_cast<OrbisPngEncoderInfo*>(
        (uintptr_t)memoryAddress + 0xfU & 0xfffffffffffffff0);

    encoder_info->self_address = reinterpret_cast<uintptr_t>(encoder_info);
    encoder_info->this_size = 0x10;
    encoder_info->image_width = param->max_image_width + 7 & 0xfffffff8;
    encoder_info->max_filters = param->max_filter_number;
    encoder_info->image_size =
        (param->max_filter_number + 2) * (param->max_image_width * 4 + 0x1c & 0xffffffe0) + 0x45c00;

    *handle = encoder_info;

    LOG_ERROR(Lib_Png, "scePngEncCreate returned OK");

    return ORBIS_OK;
}

s32 PS4_SYSV_ABI scePngEncDelete(OrbisPngEncHandle handle) {
    if (handle == nullptr) {
        return ORBIS_PNG_ENC_ERROR_INVALID_HANDLE;
    }

    uintptr_t* handle_ptr = reinterpret_cast<uintptr_t*>(handle);
    *handle_ptr = 0;

    LOG_ERROR(Lib_Png, "scePngEncDelete returned OK");

    return ORBIS_OK;
}

struct EncoderCallbackData {
    std::vector<uint8_t> png_staging_buffer;
    void* output_location;
};

static void PngWriteCallback(png_structp png_ptr, png_bytep data, png_size_t length) {
    LOG_ERROR(Lib_Png, "Got png bytes");
    EncoderCallbackData* callbackData = reinterpret_cast<EncoderCallbackData*>(png_get_io_ptr(png_ptr));

    callbackData->png_staging_buffer.insert(callbackData->png_staging_buffer.end(), data,
                                            data + length);

    //uintptr_t png_location = (uintptr_t)(callbackData->outputLocation) + callbackData->offset;
    //memcpy((void*)png_location, data, length);
    //callbackData->offset += length;
}

s32 PS4_SYSV_ABI scePngEncEncode(OrbisPngEncHandle handle, const OrbisPngEncEncodeParam* param,
                                 OrbisPngEncOutputInfo* outputInfo) {
    LOG_ERROR(Lib_Png, "(STUBBED) called");

    png_structp png_ptr = NULL;
    png_infop png_info = NULL;

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        LOG_ERROR(Lib_Png, "Couldn't create png write struct");
        return ORBIS_FAIL;
    }

    png_info = png_create_info_struct(png_ptr);
    if (!png_info) {
        LOG_ERROR(Lib_Png, "Couldn't create png info struct");
        return ORBIS_FAIL;
    }

    png_set_IHDR(png_ptr, png_info, param->image_width, param->image_height, param->bit_depth,
                 PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);

    EncoderCallbackData callbackData{};
    callbackData.output_location = param->png_mem_addr;

    png_set_write_fn(png_ptr, &callbackData, PngWriteCallback, NULL);

    png_write_info(png_ptr, png_info);
    png_set_filter(png_ptr, PNG_FILTER_TYPE_BASE, PNG_FILTER_NONE);

    // Write rows one by one
    for (int y = 0; y < param->image_height; y++) {
        png_bytep row = (png_bytep)param->image_mem_addr + (y * param->image_width * 3); // Each row starts at y * row_size
        png_write_row(png_ptr, row);
    }

    png_write_end(png_ptr, png_info);
    png_destroy_write_struct(&png_ptr, &png_info);

    memcpy(param->png_mem_addr, callbackData.png_staging_buffer.data(), param->png_mem_size);

    FILE* file = fopen("minecraft.png", "w"); // Open file for writing (overwrite if exists)
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    fwrite(callbackData.png_staging_buffer.data(), param->png_mem_size, 1,
           file); // Write text to the file

    fclose(file); // Close the file

    return ORBIS_OK;
}

s32 PS4_SYSV_ABI scePngEncQueryMemorySize(const OrbisPngEncCreateParam* param) {
    if (param == nullptr) {
        LOG_ERROR(Lib_Png, "Invalid Param");
        return ORBIS_PNG_ENC_ERROR_INVALID_ADDR;
    }

    LOG_INFO(Lib_Png,
             "scePngEncQueryMemorySize | this_size = {}, attribute = {}, max_image_width = {}, "
             "max_filter_number = {}",
             param->this_size, param->attribute, param->max_image_width, param->max_filter_number);

    if (param->this_size != 0x10) {
        
        return ORBIS_PNG_ENC_ERROR_INVALID_SIZE;
    }

    if (param->attribute == 0 && param->max_image_width - 1 < 1000000 &&
        param->max_filter_number < 5) {
        s32 retVal =
            (param->max_filter_number + 2) * (param->max_image_width * 4 + 28 & 0xffffffe0) +
            0x45c70;
        LOG_ERROR(Lib_Png, "PngQuery returned {}", retVal);
        return retVal;
    }

    return ORBIS_PNG_ENC_ERROR_INVALID_PARAM;
}

void RegisterlibScePngEnc(Core::Loader::SymbolsResolver* sym) {
    LIB_FUNCTION("7aGTPfrqT9s", "libScePngEnc", 1, "libScePngEnc", 1, 1, scePngEncCreate);
    LIB_FUNCTION("RUrWdwTWZy8", "libScePngEnc", 1, "libScePngEnc", 1, 1, scePngEncDelete);
    LIB_FUNCTION("xgDjJKpcyHo", "libScePngEnc", 1, "libScePngEnc", 1, 1, scePngEncEncode);
    LIB_FUNCTION("9030RnBDoh4", "libScePngEnc", 1, "libScePngEnc", 1, 1, scePngEncQueryMemorySize);
};

} // namespace Libraries::PngEnc