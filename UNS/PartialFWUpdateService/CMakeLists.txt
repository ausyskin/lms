# SPDX-License-Identifier: Apache-2.0
# Copyright (C) 2010-2024 Intel Corporation
cmake_minimum_required (VERSION 3.15)

add_library (PartialFWUpdateService STATIC
  PartialFWUpdateService.cpp
  PFWUpdateDllWrapperFactory.cpp
  PFWUpdateDllWrapperLegacy.cpp
  PFWUpdateDllWrapperME11.cpp
  PFWUpdateDllWrapperME12.cpp
  PFWUpdateDllWrapperME16.cpp
  PFWUpdateDllWrapperME18.cpp
  errorlist.c
)

target_link_libraries (PartialFWUpdateService
  GMS_COMMON
  ACE::ACE
  WsmanClient
)

target_compile_definitions (PartialFWUpdateService PRIVATE
  PARTIALFWUPDATESERVICE_HAS_DLL=0
)
