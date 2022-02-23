# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or , and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             native-lib.cpp

             therm/cJSON.c
             therm/cJSON.h
             therm/ClientThread.cpp
             therm/ClientThread.h
             therm/ClientThread.cpp
             therm/EasyTime.h
             therm/EasyTime.cpp
             therm/FileEx.h
             therm/FileEx.cpp
             therm/IR_APP_Define.h

             therm/IR_APP_Private.h
             therm/IR_APP_Private.cpp

            therm/IR_APP_SDK.h
            therm/IR_APP_SDK.cpp

            therm/KTime.h
            therm/KTime.cpp

            therm/Socket.h
            therm/Socket.cpp
            therm/Thread.h
            therm/Thread.cpp

            therm/wifi_decode_packet.h
            therm/wifi_decode_packet.cpp

            therm/wifi_define.h
            therm/wifi_define.cpp

            therm/wifipacketinfo.h
            therm/wifipacketinfo.cpp

            therm/zs_CmdList.h
            therm/zs_CmdProcess.cpp
            therm/zs_CmdProcess.h
            therm/zs_Types.h

            therm/DeviceConnect.cpp
#            therm/DeviceConnect.h

             )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

# 设置头文件搜索路径
include_directories(include)

# 通过设置编译选项, 设置函数库的搜索路径
# 此处的 ANDROID_ABI 是在
# build.gradle android->defaultConfig->externalNativeBuild->cmake
# 下的 abiFilters 中设置
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}")

target_link_libraries( # Specifies the target library.
                       native-lib

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )