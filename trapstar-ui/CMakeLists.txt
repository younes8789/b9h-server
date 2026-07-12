cmake_minimum_required(VERSION 3.19)
project(TrapstarUI VERSION 1.0.0 LANGUAGES CXX)

# ============================================================
#  إعدادات C++ الأساسية
# ============================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================
#  تحميل open.mp SDK تلقائياً عبر FetchContent
# ============================================================
include(FetchContent)

FetchContent_Declare(
    omp-sdk
    GIT_REPOSITORY https://github.com/openmultiplayer/open.mp-sdk.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(omp-sdk)

# ============================================================
#  بناء الـ Component كـ Shared Library (.so / .dll)
# ============================================================
add_library(TrapstarUI SHARED
    src/main.cpp
)

# ربط SDK بالـ Component
target_link_libraries(TrapstarUI PRIVATE OMP-SDK)

# إعداد اسم الملف الناتج (بدون prefix "lib" على لينكس)
set_target_properties(TrapstarUI PROPERTIES
    PREFIX ""
    OUTPUT_NAME "trapstar"
)

# ============================================================
#  الإعدادات المختلفة لكل نظام تشغيل
# ============================================================
if (UNIX)
    # لينكس: خاص يكون 32-bit باش يتوافق مع open.mp
    target_compile_options(TrapstarUI PRIVATE -m32)
    target_link_options(TrapstarUI PRIVATE -m32)
endif()

if (WIN32)
    # ويندوز: نوقف الـ MSVC warnings الزيادة
    target_compile_definitions(TrapstarUI PRIVATE
        _CRT_SECURE_NO_WARNINGS
        WIN32_LEAN_AND_MEAN
    )
endif()

message(STATUS "[TrapstarUI] Component configured successfully!")
message(STATUS "[TrapstarUI] Output: TrapstarUI.so (Linux) / TrapstarUI.dll (Windows)")
