# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2010-2024 Intel Corporation
cmake_minimum_required (VERSION 3.15)

if (UNIX)
  find_package (PkgConfig)
  set (EVENTLOG_SRC LinuxEventLog.cpp)
  set (REGISTRY_SRC LinuxRegistryStorage.cpp)
  pkg_check_modules (IDN2 REQUIRED libidn2)
else (UNIX)
  set (EVENTLOG_SRC WindowsEventLog.cpp)
  set (VERIFYFILE_SRC VerifyFile.cpp)
  set (SECLIBS
    crypt32.lib
    WinTrust.lib
    version.lib
  )
  set (REGISTRY_SRC WindowsRegistryStorage.cpp)
  set (IS64BIT_SRC Is64BitOs.cpp)
  set (STRINGMANAGER_SRC StringManager.cpp)
  set (STRINGLOADER_SRC WindowsStringLoader.cpp)
endif (UNIX)

add_library (GMS_COMMON STATIC
  EventHandler.cpp
  FuncEntryExit.cpp
  GMSExternalLogger.cpp
  GmsSubService.cpp
  ${EVENTLOG_SRC}
  RegistryGenerator.cpp
  GmsService.cpp
  ${VERIFYFILE_SRC}
  Tools.cpp
  UNSRegistry.cpp
  DataStorageWrapper.cpp
  RegistryStorage.cpp
  ${REGISTRY_SRC}
  ${IS64BIT_SRC}
  ${STRINGMANAGER_SRC}
  ${STRINGLOADER_SRC}
)

target_link_libraries (GMS_COMMON
  ACE::ACE
  ${SECLIBS}
  UNSMessageFileLib
  ${IDN2_LIBRARIES}
)

target_compile_definitions (GMS_COMMON PRIVATE
  NO_STDOUT_DEBUG_LOG
  $<$<BOOL:${IGNORE_DLL_SIGNATURES}>:IGNORE_DLL_SIGNATURES>
)

target_include_directories (GMS_COMMON PRIVATE
  ${IDN2_INCLUDE_DIRS}
)

target_include_directories (GMS_COMMON PUBLIC .)
