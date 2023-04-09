#pragma once
#include "curl-base/curl/curl.h"
#include "zlib-base/zlib.h"
#include "zlib-base/zconf.h"
#pragma comment(lib,"openssl.lib")
#pragma comment(lib,"libssl.lib")
#pragma comment(lib,"libcrypto.lib")
#pragma comment(lib,"zlib-base/zlibwapi.lib")
#pragma comment(lib,"curl-base/libcurl.lib")
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Wldap32.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"Crypt32.lib")
#pragma comment(lib,"Normaliz.lib")

#include <iostream>
#include <string>
#include <Windows.h>

enum LibcurlFlag
{
	Lf_None = 0,
	Lf_Download,
	Lf_Post,
	Lf_Get,
};

class OSCurl
{
public:
	OSCurl();
	~OSCurl();
	// ���ó�ʱʱ��
	bool SetTimeOut(int nSecond);
	// �������ӳ�ʱʱ��
	bool SetConnectTimeOut(int nSecond);
	// �����û�����
	bool SetUserAgent(std::string Agent);
	// ����Զ���ͷ
	bool AddHeader(std::string Key, std::string Value);
	// ��������ͷ�б�
	void ClearHeaderList();
	// ����cookie
	bool SetCookie(std::string Cookie);
	// ����cookie
	bool RequestCookie();
	// ��ȡcookie
	bool GetCookie(std::string&  OUT cookie);
	// ��ȡ���������Ϣ
	std::string GetLastCurlError() const;
	// Post
	bool Post(std::string Url, std::string Data);
	// Get
	bool Get(std::string Url);
	// ��ȡRespons
	const std::string GetRespons();
protected:
	static size_t WriteCallback(void* pBuffer, size_t nSize, size_t nMemByte, void* pParam);
private:
	CURLcode _Code;
	std::string _Respons;
	LibcurlFlag	_lfFalg;
	curl_slist* _CurlList;
	CURL* _pCurl;
};