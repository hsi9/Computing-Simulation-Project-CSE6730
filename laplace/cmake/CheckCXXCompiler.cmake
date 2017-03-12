include(CheckCXXCompilerFlag)

set( MINIMUM_GCC_VERSION_REQUIRED 5.2 )
set( MINIMUM_APPLECLANG_VERSION_REQUIRED 6.0 )
set( MINIMUM_CLANG_VERSION_REQUIRED 3.4 )
set( MINIMUM_ICC_VERSION_REQUIRED 15.0 )
set( MINIMUM_MSVC_VERSION_REQUIRED 19.0 )

macro(CHECK_COMPILER_VERSION compiler_name minimum_version)
    message( "-- Checking for C++ compiler version" )
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS ${minimum_version})
        message( FATAL_ERROR "${BoldRed}${compiler_name} version must be >= ${MINIMUM_GCC_VERSION_REQUIRED}!${ColourReset}" )
    endif()
    message( "-- Checking for C++ compiler version -> ${CMAKE_CXX_COMPILER_VERSION} (Success)" )
endmacro(CHECK_COMPILER_VERSION)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# On Linux machines, clang uses gcc's libstdc++ library by default
# External libraries built using libstdc++ will need to be rebuilt with libc++ though -
# see http://stackoverflow.com/questions/8454329/why-cant-clang-with-libc-in-c0x-mode-link-this-boostprogram-options-examp
CHECK_CXX_COMPILER_FLAG("-stdlib=libc++" COMPILER_SUPPORTS_LIBCXX)
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND COMPILER_SUPPORTS_LIBCXX)
    message( STATUS "libc++ found; explicitly compiling with -stdlib=libc++..." )
    message( WARNING "${UnderlinedRed}External dependencies may need to be rebuilt using libc++ to avoid linker errors during project compilation!${ColourReset}" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

# Older versions of compilers may support C++11, but not include C++11 standards-compliant libraries
if (CMAKE_COMPILER_IS_GNUCC)
    CHECK_COMPILER_VERSION(GCC MINIMUM_GCC_VERSION_REQUIRED)
ELSEif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    CHECK_COMPILER_VERSION(ICC MINIMUM_ICC_VERSION_REQUIRED)
ELSEif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    CHECK_COMPILER_VERSION(Clang MINIMUM_CLANG_VERSION_REQUIRED)
ELSEif (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    CHECK_COMPILER_VERSION(AppleClang MINIMUM_APPLECLANG_VERSION_REQUIRED)
ELSEif (CMAKE_COMPILER_IS_MSVC)
    CHECK_COMPILER_VERSION(MSVC MINIMUM_MSVC_VERSION_REQUIRED)
endif()

# Enable compiler warnings
if (MSVC)
    if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]") # Force to always compile with W4
        STRING(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
ELSEif (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -fno-rtti")
ELSEif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()
