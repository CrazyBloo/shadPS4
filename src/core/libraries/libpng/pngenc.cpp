// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "common/logging/log.h"
#include "core/libraries/error_codes.h"
#include "core/libraries/libs.h"
#include "core/libraries/libpng/pngenc.h"

namespace Libraries::PngEnc {

s32 PS4_SYSV_ABI scePngEncCreate(const OrbisPngEncCreateParam* param, void* memoryAddress,
                                 uint32_t memorySize, OrbisPngEncHandle* handle) {
    LOG_ERROR(Lib_Png, "(STUBBED) called");
    return ORBIS_OK;
}

s32 PS4_SYSV_ABI scePngEncDelete(OrbisPngEncHandle handle) {
    LOG_ERROR(Lib_Png, "(STUBBED) called");
    return ORBIS_OK;
}

s32 PS4_SYSV_ABI scePngEncEncode(OrbisPngEncHandle handle, const OrbisPngEncEncodeParam* param,
                                 OrbisPngEncOutputInfo* outputInfo) {
    LOG_ERROR(Lib_Png, "(STUBBED) called");
    return ORBIS_OK;
}

s32 PS4_SYSV_ABI scePngEncQueryMemorySize(const OrbisPngEncCreateParam* param) {
    LOG_ERROR(Lib_Png, "(STUBBED) called");
    return ORBIS_OK;
}

void RegisterlibScePngEnc(Core::Loader::SymbolsResolver* sym) {
    LIB_FUNCTION("7aGTPfrqT9s", "libScePngEnc", 1, "libScePngEnc", 1, 1, scePngEncCreate);
    LIB_FUNCTION("RUrWdwTWZy8", "libScePngEnc", 1, "libScePngEnc", 1, 1, scePngEncDelete);
    LIB_FUNCTION("xgDjJKpcyHo", "libScePngEnc", 1, "libScePngEnc", 1, 1, scePngEncEncode);
    LIB_FUNCTION("9030RnBDoh4", "libScePngEnc", 1, "libScePngEnc", 1, 1, scePngEncQueryMemorySize);
};

} // namespace Libraries::PngEnc