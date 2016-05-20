// OfficeOdfFileW.cpp : Implementation of COfficeOdfFileW
#include "stdafx.h"

#include "OfficeOdfFileW.h"
#include "../../ASCOfficeUtils/ASCOfficeUtilsLib/OfficeUtils.h"

#include <string>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp> 

#include "../../Common/DocxFormat/Source/Base/Base.h"
#include "../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"

#include "../source/Oox2OdfConverter/Oox2OdfConverter.h"


#ifndef STANDALONE_USE
	#define STANDALONE_USE 0// ��� �� �����: ���� (1) ��� ����� (0)
#endif 


// ��� ���������� - uuid

std::wstring bstr2wstring(BSTR str)
{
    return str ? std::wstring(&str[0], &str[::SysStringLen(str)]) : L"";
}

STDMETHODIMP COfficeOdfFileW::LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
{
   return S_FALSE;
}
STDMETHODIMP COfficeOdfFileW::SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
{
    HRESULT hr;

    if (!sSrcPath)
    {
        return E_FAIL;
    }

#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
	std::wstring inputDir = FileSystem::Directory::GetFolderPath(std::wstring(sSrcPath));
#else
	std::wstring inputDir = sSrcPath;
#endif

	std::wstring outputDir = FileSystem::Directory::GetFolderPath(std::wstring(sDstFileName));

    // ������� � � ���������� ���� ������� ���������

	std::wstring dstTempPath = FileSystem::Directory::CreateDirectoryWithUniqueName(outputDir);
    

#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
    std::wstring srcTempPath = FileSystem::Directory::CreateDirectoryWithUniqueName(outputDir);
#else
    std::wstring srcTempPath = inputDir;
#endif

    try
    {
		FileSystem::Directory::CreateDirectory(dstTempPath);

#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
        
        FileSystem::Directory::CreateDirectory(srcTempPath); // ������� ��������� ���������� ��� �������������� ������
#endif
        hr = SaveToFileImpl(bstr2wstring(sSrcPath),srcTempPath, dstTempPath, bstr2wstring(sDstFileName));
        
    }
    catch(...)
    {
        hr = E_FAIL;
    }

    // ������� ��������� ���������� � ������������� ����������
    try
    {
		FileSystem::Directory::DeleteDirectory(dstTempPath);
    }
    catch(...)
    {
    }

#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
    // � ������ ���� �� ������ ���� � ������� ��������� ���������� (�� ���� �� �������)
    try 
    {
		FileSystem::Directory::DeleteDirectory(srcTempPath);
    }
    catch(...) 
    {
    }
#endif
    
    return hr;
}

HRESULT COfficeOdfFileW::SaveToFileImpl(const std::wstring & srcPath,
                                        const std::wstring & srcTempPath,
                                        const std::wstring & dstTempPath,
                                        const std::wstring & dstFileName)
{
    HRESULT hr = E_FAIL;
	COfficeUtils oCOfficeUtils(NULL);  

    // ������������� �������� (���� �� ����) �� ��������� ����������
#if defined(STANDALONE_USE) && (STANDALONE_USE == 1)
        if (S_OK != oCOfficeUtils.ExtractToDirectory(srcPath.c_str(), srcTempPath.c_str(), NULL, 0))
            return hr;
#endif

	try
	{
		std::wstring type = DetectTypeDocument(srcTempPath);

		ProgressCallback ffCallBack;

		ffCallBack.OnProgress	=	OnProgressFunc;
		ffCallBack.OnProgressEx	=	OnProgressExFunc;
		ffCallBack.caller		=	this;
		
		Oox2Odf::Converter converter(srcTempPath, type, &ffCallBack);
		

		converter.convert();
		converter.write(dstTempPath);
	} 
	catch(...)
	{
	}

	if (S_OK != oCOfficeUtils.CompressFileOrDirectory(dstTempPath.c_str(), dstFileName.c_str(), -1))
        return hr;


    return S_OK;
}

//FileChecker � ������ ����� !!
std::wstring COfficeOdfFileW::DetectTypeDocument(const std::wstring & Path)
{
	return  L"text";
	//return  L"spreadsheet";
}

void COfficeOdfFileW::OnProgressFunc (LPVOID lpParam, long nID, long nPercent)
{
	COfficeOdfFileW* pOdfFile = reinterpret_cast<COfficeOdfFileW*>(lpParam);
	if (pOdfFile != NULL)
	{
		pOdfFile->OnProgress(nID, nPercent);
	}
}

void COfficeOdfFileW::OnProgressExFunc (LPVOID lpParam, long nID, long nPercent, short* pStop)
{
	COfficeOdfFileW* pOdfFile = reinterpret_cast<COfficeOdfFileW*>(lpParam);
	if (pOdfFile != NULL)
	{
		pOdfFile->OnProgressEx(nID, nPercent, pStop);
	}

}