/* SPDX-License-Identifier: Apache-2.0 */
/*
 * Copyright (C) 2009-2015 Intel Corporation
 */
/*++

@file: StringManager.h

--*/

//-------------------------------------------------------------------------------------------
//
//  Contents:   defenition to the string manager - used to load strings of correct language
//              from embedded rc file 
//
//--------------------------------------------------------------------------------------------
#ifndef _STRING_MANAGER_H_
#define _STRING_MANAGER_H_

#include "CommonDllExport.h"
#include <string>
#include <map>
#include <vector>
#include <memory>

//define it for different uses
#ifdef UNICODE
#define STRING_TYPE std::wstring
#define CHAR_TYPE wchar_t
#define EMPTY_STR STRING_TYPE(L"")
#else
#define STRING_TYPE std::string
#define CHAR_TYPE char
#define EMPTY_STR STRING_TYPE("")
#endif

#define MAX_STRING_LEN 500

class COMMON_EXPORT StringLoader
{
public:
	//load the strings with IDs from given vector into the map,throws exception on failure
	virtual void loadStrings(std::vector<unsigned int>& ids,std::map<unsigned int,STRING_TYPE>& stringMap) = 0;
	virtual ~StringLoader() {}
};

class COMMON_EXPORT CStringManager
{
public:
	//get string by it's ID
	//returns : the string if was loaded, empty string otherwise
	const STRING_TYPE getString(unsigned int id) const; 
	//load the strings with the IDs in the given vector from resource file using given StringLoader, throws exception on failure
	void loadStrings(StringLoader& loader,std::vector<unsigned int>& ids) {loader.loadStrings(ids,m_stringMap);}
	virtual ~CStringManager() {}
private:
	std::map<unsigned int,STRING_TYPE> m_stringMap;
};

//Singleton for CStringManager
class COMMON_EXPORT StringManager
{
public:
	static CStringManager* instance();

private:
	static std::shared_ptr<CStringManager> m_instance;
	StringManager() {} //private constructor
};



#endif //_STRING_MANAGER_H_