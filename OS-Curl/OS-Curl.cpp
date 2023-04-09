#include "OS-Curl.h"
#define ERROR(f) if(f) return false

OSCurl::OSCurl():
	_pCurl(nullptr),
	_lfFalg(Lf_None),
	_Code(CURLE_OK),
	_CurlList(nullptr)
{
	_pCurl = curl_easy_init();
	curl_easy_setopt(_pCurl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(_pCurl, CURLOPT_WRITEDATA, this);
	// if want to support gzip
	curl_easy_setopt(_pCurl, CURLOPT_ENCODING, "gzip");
}

OSCurl::~OSCurl()
{
	ClearHeaderList();
	curl_easy_cleanup(_pCurl);
}

size_t OSCurl::WriteCallback(void* pBuffer, size_t nSize, size_t nMemByte, void* pParam)
{
	OSCurl* pThis = (OSCurl*)pParam;
	DWORD dwWritten = 0;
	switch (pThis->_lfFalg)
	{
	case Lf_Post:
		//...
	case Lf_Get:
	{
		pThis->_Respons.append((const char*)pBuffer, nSize * nMemByte);
		dwWritten = nSize * nMemByte;
	}
	break;
	case Lf_None:
		break;
	}
	return dwWritten;
}

void OSCurl::ClearHeaderList()
{
	if (_CurlList)
	{
		curl_slist_free_all(_CurlList);
		_CurlList = nullptr;
	}
}

bool OSCurl::SetTimeOut(int nSecond)
{
	ERROR(nSecond < 0);
	_Code = curl_easy_setopt(_pCurl, CURLOPT_TIMEOUT, nSecond);
	return CURLE_OK == _Code;
}

bool OSCurl::SetConnectTimeOut(int nSecond)
{
	ERROR(nSecond < 0);
	_Code = curl_easy_setopt(_pCurl, CURLOPT_CONNECTTIMEOUT, nSecond);
	return CURLE_OK == _Code;
}

bool OSCurl::SetUserAgent(std::string Agent)
{
	ERROR(Agent.empty() == 0);
	_Code = curl_easy_setopt(_pCurl, CURLOPT_USERAGENT, Agent.c_str());
	return CURLE_OK == _Code;
}

bool OSCurl::AddHeader(std::string Key, std::string Value)
{	
	ERROR(Key.empty() || Value.empty());
	std::string _Header(Key);
	_Header.append(": ").append(Value);
	_CurlList = curl_slist_append(_CurlList, _Header.c_str());
	_Code = curl_easy_setopt(_pCurl, CURLOPT_HTTPHEADER, _CurlList);
	return CURLE_OK == _Code;

}

bool OSCurl::SetCookie(std::string Cookie)
{
	ERROR(Cookie.empty());
	_Code = curl_easy_setopt(_pCurl, CURLOPT_COOKIE, Cookie.c_str());
	return CURLE_OK == _Code;
}

bool OSCurl::RequestCookie()
{
	_Code = curl_easy_setopt(_pCurl, CURLOPT_COOKIEFILE, "");
	return CURLE_OK == _Code;
}

bool OSCurl::GetCookie(std::string& OUT cookie)
{
	curl_slist* _CookieList = nullptr;
	std::string	_Cookie;
	_Code = curl_easy_getinfo(_pCurl, CURLINFO_COOKIELIST, &_CookieList);
	ERROR(_Code != CURLE_OK);
	while (_CookieList)
	{
		_Cookie.append(_CookieList->data);
		int _index = _Cookie.rfind("\t", _Cookie.rfind("\t") - 1);
		_Cookie = _Cookie.erase(0, _index + 1);
		_index = _Cookie.find("\t");
		if (_index != std::string::npos)
			_Cookie.replace(_index, 1, "=");
		cookie.append(_Cookie + ";");
		_CookieList = _CookieList->next;
	}
	return true;
}

std::string OSCurl::GetLastCurlError() const
{
	return curl_easy_strerror(_Code);
}

bool OSCurl::Post(std::string Url, std::string Data)
{
	ERROR(Url.empty() || Data.empty());
	curl_easy_setopt(_pCurl, CURLOPT_POST, 1);
	curl_easy_setopt(_pCurl, CURLOPT_POSTFIELDS, Data.c_str());
	curl_easy_setopt(_pCurl, CURLOPT_URL, Url.c_str());
	_lfFalg = Lf_Post;
	_Respons.clear();
	_Code = curl_easy_perform(_pCurl);
	return CURLE_OK == _Code;
}

bool OSCurl::Get(std::string Url)
{
	ERROR(Url.empty());
	curl_easy_setopt(_pCurl, CURLOPT_HTTPGET, 1);
	curl_easy_setopt(_pCurl, CURLOPT_URL, Url.c_str());
	curl_easy_setopt(_pCurl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(_pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(_pCurl, CURLOPT_SSL_VERIFYHOST, 0L);
	_lfFalg = Lf_Get;
	_Respons.clear();
	_Code = curl_easy_perform(_pCurl);
	return CURLE_OK == _Code;
}

const std::string OSCurl::GetRespons()
{
	return _Respons;
}