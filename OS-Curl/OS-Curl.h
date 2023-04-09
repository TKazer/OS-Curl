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
	// 设置超时时间
	bool SetTimeOut(int nSecond);
	// 设置连接超时时间
	bool SetConnectTimeOut(int nSecond);
	// 设置用户代理
	bool SetUserAgent(std::string Agent);
	// 添加自定义头
	bool AddHeader(std::string Key, std::string Value);
	// 清理请求头列表
	void ClearHeaderList();
	// 设置cookie
	bool SetCookie(std::string Cookie);
	// 请求cookie
	bool RequestCookie();
	// 获取cookie
	bool GetCookie(std::string&  OUT cookie);
	// 获取最近错误消息
	std::string GetLastCurlError() const;
	// Post
	bool Post(std::string Url, std::string Data);
	// Get
	bool Get(std::string Url);
	// 获取Respons
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