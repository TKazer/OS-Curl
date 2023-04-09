#include "OS-Curl.h"

int main()
{
	OSCurl curl;
	if (curl.Get("https://comment.bilibili.com/1086918206.xml"))
	{
		std::cout << curl.GetRespons() << std::endl;
	}
	return 0;
}