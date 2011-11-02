//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	TGAImg.h - This file handles the classes constructors for
//	the TGAImg class
//	
//////////////////////////////////////////////////////////////

class TGAImg
{
	public:
		TGAImg::TGAImg();
		TGAImg::~TGAImg();
		int TGAImg::Load(char* szFilename);
		void TGAImg::FlipImg();
		int TGAImg::GetBPP();
		int TGAImg::GetWidth();
		int TGAImg::GetHeight();
		unsigned char* TGAImg::GetImg();
		unsigned char* TGAImg::GetPalette();

	//	Assign variables only the class can use
	private:
		short int iWidth, iHeight, iBPP;
		unsigned long lImageSize;
		char bEnc;
		unsigned char *pImage, *pPalette, *pData;

		//	Internal workers
		int TGAImg::ReadHeader();
		int TGAImg::LoadRawData();
		int TGAImg::LoadTgaRLEData();
		int TGAImg::LoadTgaPalette();
		void TGAImg::BGRtoRGB();
};
