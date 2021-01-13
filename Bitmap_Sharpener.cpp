#include <iostream>
#include <fstream>

using namespace std;

int MAX_A = 3;

//data structures for bitmap metadata
struct BMPFILEHEADER {
	int bfType;
	int bfSize;
	int bfReserved1;
	int bfReserved2;
	int bfOffBits;
};

struct BMPINFOHEADER {
	int biSize;
	int biWidth;
	int biHeight;
	int biPlanes;
	int biBitCount;
	int biCompression;
	int biSizeImage;
	int biXpelsPerMeter;
	int biYpelsPerMeter;
	int biCrlUses;
	int biCrlImportant;
};

//read bitmap header and write to own data structure
void odczytajiprzepiszBFH(ifstream& ifs, BMPFILEHEADER& bfh, ofstream& ofs)
{
	ifs.read(reinterpret_cast<char*>(&bfh.bfType), 2);
	ifs.read(reinterpret_cast<char*>(&bfh.bfSize), 4);
	ifs.read(reinterpret_cast<char*>(&bfh.bfReserved1), 2);
	ifs.read(reinterpret_cast<char*>(&bfh.bfReserved2), 2);
	ifs.read(reinterpret_cast<char*>(&bfh.bfOffBits), 4);

	ofs.write(reinterpret_cast<char*>(&bfh.bfType), 2);
	ofs.write(reinterpret_cast<char*>(&bfh.bfSize), 4);
	ofs.write(reinterpret_cast<char*>(&bfh.bfReserved1), 2);
	ofs.write(reinterpret_cast<char*>(&bfh.bfReserved2), 2);
	ofs.write(reinterpret_cast<char*>(&bfh.bfOffBits), 4);
}

//read bitmap header and write to own data structure
void odczytajiprzepiszBIH(ifstream& ifs, BMPINFOHEADER& bih, ofstream& ofs)
{
	ifs.read(reinterpret_cast<char*>(&bih.biSize), 4);
	ifs.read(reinterpret_cast<char*>(&bih.biWidth), 4);
	ifs.read(reinterpret_cast<char*>(&bih.biHeight), 4);
	ifs.read(reinterpret_cast<char*>(&bih.biPlanes), 2);
	ifs.read(reinterpret_cast<char*>(&bih.biBitCount), 2);
	ifs.read(reinterpret_cast<char*>(&bih.biCompression), 4);
	ifs.read(reinterpret_cast<char*>(&bih.biSizeImage), 4);
	ifs.read(reinterpret_cast<char*>(&bih.biXpelsPerMeter), 4);
	ifs.read(reinterpret_cast<char*>(&bih.biYpelsPerMeter), 4);
	ifs.read(reinterpret_cast<char*>(&bih.biCrlUses), 4);
	ifs.read(reinterpret_cast<char*>(&bih.biCrlImportant), 4);

	ofs.write(reinterpret_cast<char*>(&bih.biSize), 4);
	ofs.write(reinterpret_cast<char*>(&bih.biWidth), 4);
	ofs.write(reinterpret_cast<char*>(&bih.biHeight), 4);
	ofs.write(reinterpret_cast<char*>(&bih.biPlanes), 2);
	ofs.write(reinterpret_cast<char*>(&bih.biBitCount), 2);
	ofs.write(reinterpret_cast<char*>(&bih.biCompression), 4);
	ofs.write(reinterpret_cast<char*>(&bih.biSizeImage), 4);
	ofs.write(reinterpret_cast<char*>(&bih.biXpelsPerMeter), 4);
	ofs.write(reinterpret_cast<char*>(&bih.biYpelsPerMeter), 4);
	ofs.write(reinterpret_cast<char*>(&bih.biCrlUses), 4);
	ofs.write(reinterpret_cast<char*>(&bih.biCrlImportant), 4);
}

//funkcja wczytuj¹ca 3 pierwsze wiersze bitmapy do bufora
//read first 3 lines of the bitmap to a buffer
void wczytajBufor(ifstream& ifs, char* B[], int szerokoscwiersza)
{
	for (int h = 0; h < 3; h++)
		for (int i = 0; i < szerokoscwiersza; i++)
		{
			ifs.read(reinterpret_cast<char *>(&B[h][i]), 1);
		}

}
//funkcja która przeskakuje buforem na kolejny wiersz bitmapy
//load next line to the buffor
void zmienBufor(ifstream& ifs, char* B[], int szerokoscwiersza)
{
	for (int pisz = 0; pisz < szerokoscwiersza; pisz++)
	{
		B[0][pisz] = B[1][pisz];
		B[1][pisz] = B[2][pisz];
		ifs.read(reinterpret_cast<char *>(&B[2][pisz]), 1);
	}
}

//clean memory
void usunpamiec(char* B[], char * W)
{
	delete[] W;
	W = NULL;
	
	for(int i = 0; i<3; i++)
	{
		delete [] B[i];
		B[i] = NULL;
	 } 
	delete[] B;
	B = NULL; 
}

//do the convolution on the data in the buffer
void konwolucja(ifstream& ifs, ofstream& ofs, BMPFILEHEADER bfh, BMPINFOHEADER bih, int szerokoscwiersza)
{
	
	int szerokoscznaczaca = 3 * bih.biWidth;								//szerokosc wiersza bez wype³nienia zerami
	char** B = new char* [MAX_A];											//stworzenie bufora
	for (int i = 0; i < MAX_A; i++)
	{
		B[i] = new char[szerokoscwiersza];
	}

	char* W = new char[szerokoscwiersza];										//tablica w której zapisany zostaje wynik konwolucji 

	wczytajBufor(ifs, B, szerokoscwiersza);

	for (int j = 0; j < szerokoscwiersza; j++)
	{
		ofs.write(reinterpret_cast<char *>(&B[0][j]), 1);						//przepisanie pierwszego wiersza
	}
	
		for (int wysokosc = 0; wysokosc < (bih.biHeight - 2); wysokosc++)		//liczba konwolucji
		{
			int rgb = 0;
			for (int i = 0; i < 3; i++)											//zmienna rgb wykorzystywana do wykonania konwolucji na trzech warstawach koloru
			{
				W[rgb] = B[1][rgb];												//przepisanie skrajnego piksela
				for (int sz = rgb + 3; sz < (szerokoscznaczaca - (5-rgb)); sz += 3)		//zmienna sz przechodzi po wszytkich pikselach danego koloru w wierszu z pominiêciem pierwszego i ostatniego piksela
				{
					W[sz] = (0 * B[0][sz - 3]) + ((-1) * B[0][sz]) + (0 * B[0][sz + 3]) + ((-1) * B[1][sz - 3]) + (5 * B[1][sz]) + ((-1) * B[1][sz + 3]) + (0 * B[2][sz - 3]) + ((-1) * B[2][sz]) + (0 * B[2][sz + 3]);
				}
				int n = 3;
				W[szerokoscznaczaca - n] = B[1][szerokoscznaczaca - n];			//przepisanie skrajnego piksela
				--n;
				++rgb;
			}
			for (int dopiszzero = szerokoscznaczaca; dopiszzero < szerokoscwiersza; dopiszzero++)		//pêtla która dope³nia wiersz zerami tak aby jego szerokoœc by³a podzielna przez 4
			{
				W[dopiszzero] = 0;
			}
			for (int pisz = 0; pisz < szerokoscwiersza; pisz++)											//pêtla zapisuj¹ca przetworzony wiersz do wyjœciowego pliku
			{
				ofs.write(reinterpret_cast<char*>(&W[pisz]), 1);
			}
			zmienBufor(ifs, B, szerokoscwiersza);
		}

		for(int pisz = 0; pisz < szerokoscwiersza; pisz++)												//pêtla która przepisuje ostatni wiersz bitmapy
		{
			ofs.write(reinterpret_cast<char*>(&B[2][pisz]), 1);
		}
		
		usunpamiec(B, W);
}

int main()
{

	BMPFILEHEADER bfh;
	BMPINFOHEADER bih;

	ifstream ifs;
	ofstream ofs;

	ifs.open("sonar_aktywny2.bmp", ios::binary);
	if (!ifs)
	{
		cout << "B³¹d otwarcia pliku";
		return 0;
	}
	ofs.open("sonar_aktywny2przetworzony.bmp", ios::binary);
	if (!ofs)
	{
		cout << "B³¹d otwarcia pliku";
		return 0;
	}

	odczytajiprzepiszBFH(ifs, bfh, ofs);
	odczytajiprzepiszBIH(ifs, bih, ofs);

	int szerokoscwiersza = (bfh.bfSize - bfh.bfOffBits) / bih.biHeight;
	
	konwolucja(ifs, ofs, bfh, bih, szerokoscwiersza);
	
	ifs.close();
	ofs.close();
}
