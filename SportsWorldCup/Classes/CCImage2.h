//
//  CCImage2.h
//  SportsWorldCup
//
//  Created by ksoo k on 13. 4. 12..
//
//

#ifndef __SportsWorldCup__CCImage2__
#define __SportsWorldCup__CCImage2__
#include "cocos2d.h"

USING_NS_CC;
class CCImage2 : public CCImage
{
public:
	
	bool initWithImageData(void * pData,
						   int nDataLen,
						   EImageFormat eFmt = CCImage::kFmtUnKnown/* = eSrcFmtPng*/,
						   int nWidth = 0/* = 0*/,
						   int nHeight = 0 /* = 0*/,
						   int nBitsPerComponent = 8/* = 8*/)
	{
		bool bRet = false;
		do
		{
			CC_BREAK_IF(! pData || nDataLen <= 0);
			
			if (kFmtPng == eFmt)
			{
				bRet = _initWithPngData(pData, nDataLen);
				break;
			}
			else if (kFmtJpg == eFmt)
			{
				bRet = _initWithJpgData(pData, nDataLen);
				break;
			}
			else if (kFmtRawData == eFmt)
			{
				bRet = _initWithRawData(pData, nDataLen, nWidth, nHeight, nBitsPerComponent);
				break;
			}else
			{
				// if it is a png file buffer.
				if (nDataLen > 8)
				{
					unsigned char* pHead = (unsigned char*)pData;
					if (   pHead[0] == 0x89
						&& pHead[1] == 0x50
						&& pHead[2] == 0x4E
						&& pHead[3] == 0x47
						&& pHead[4] == 0x0D
						&& pHead[5] == 0x0A
						&& pHead[6] == 0x1A
						&& pHead[7] == 0x0A)
					{
						bRet = _initWithPngData(pData, nDataLen);
						break;
					}
				}
				// if it is a jpeg file buffer.
				if (nDataLen > 2)
				{
					unsigned char* pHead = (unsigned char*)pData;
					if (   pHead[0] == 0xff
						&& pHead[1] == 0xd8)
					{
						bRet = _initWithJpgData(pData, nDataLen);
						break;
					}
				}
			}
		} while (0);
		return bRet;
	}
	
	
};


#endif /* defined(__SportsWorldCup__CCImage2__) */
