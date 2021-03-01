#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
#include <ctype.h>
#include <fstream>
#include <ctime>
#include <sstream>
using namespace std;

struct tree 
{
	int son; //Kac kelimenin burada bittigi sayisi
	char harf; //Agacin tuttugu harf
	string yer; //kelimelerin konumu
	tree *kardes; //Harfin kardesi
	tree *cocuk; //Harften sonra gelen ilk harf
};

template <typename T>
string to_string(const T& value) {
    stringstream ss;
    ss << value;
    return ss.str();
}

tree *ilkcocuk(tree *kok, char gharf, int kelimesonu, int kelimeyeri);
tree *kardesekle(tree *kok, char gharf, int kelimesonu, int kelimeyeri);
void yazdir(tree *kok, int uz);
pair <int, string> ara(tree *kok, string ara);
string yakinsa(tree *kok, string ara, int uzaklik);
tree *olustur(tree *kok);
tree *oku(tree *kok, int dosyano);

int main()
{
	cout << "Dosyalar olusturuluyor...\n";
	clock_t begin_time = clock();
	tree *koktxt = NULL;
	koktxt = olustur(koktxt);
	tree *kokrtf = NULL;
	kokrtf = olustur(kokrtf);
	tree *kokhtm = NULL;
	kokhtm = olustur(kokhtm);
	tree *kokxml = NULL;
	kokxml = olustur(kokxml);
	cout << "Dosyalar olusturuldu. Gecen sure: ";
	cout << float(clock() - begin_time) / CLOCKS_PER_SEC;
	cout << endl;

	cout << "Dosyalar okunuyor...\n";
	begin_time = clock();
	koktxt = oku(koktxt, 1);
	kokrtf = oku(kokrtf, 2);
	kokhtm = oku(kokhtm, 3);
	kokxml = oku(kokxml, 4);
	cout << "Dosyalar okundu. Gecen sure: ";
	cout << float(clock() - begin_time) / CLOCKS_PER_SEC;
	cout << endl;

	string arakelime;
	cout << "Aramak istediginiz kelimeyi giriniz: ";
	cin >> arakelime;
	begin_time = clock();
	pair <int, string> bulunanlar1 = ara(koktxt, arakelime);
	pair <int, string> bulunanlar2 = ara(kokrtf, arakelime);
	pair <int, string> bulunanlar3 = ara(kokhtm, arakelime);
	pair <int, string> bulunanlar4 = ara(kokxml, arakelime);
	int bulunan = bulunanlar1.first;
	int topbul = bulunan;
	if (bulunan > 0)
	{
		cout << "\"" << arakelime << "\" kelimesi birinci dosyada " << bulunan << " kez bulundu.\n";
		cout << "Bulunan kelimenin yerleri: " << bulunanlar1.second << endl;
	}
	bulunan = bulunanlar2.first;
	topbul += bulunan;
	if (bulunan > 0)
	{
		cout << "\"" << arakelime << "\" kelimesi ikinci dosyada " << bulunan << " kez bulundu.\n";
		cout << "Bulunan kelimenin yerleri: " << bulunanlar2.second << endl;
	}
	bulunan = bulunanlar3.first;
	topbul += bulunan;
	if (bulunan > 0)
	{
		cout << "\"" << arakelime << "\" kelimesi ucuncu dosyada " << bulunan << " kez bulundu.\n";
		cout << "Bulunan kelimenin yerleri: " << bulunanlar3.second << endl;
	}
	bulunan = bulunanlar4.first;
	topbul += bulunan;
	if (bulunan > 0)
	{
		cout << "\"" << arakelime << "\" kelimesi dorduncu dosyada " << bulunan << " kez bulundu.\n";
		cout << "Bulunan kelimenin yerleri: " << bulunanlar4.second << endl;
	}
	
	if (topbul == 0)
	{
		cout << "\"" << arakelime << "\" kelimesi bulunamadi.\n";
		string yakinsanan = "";
		yakinsanan = yakinsa(koktxt, arakelime, arakelime.length());
		if (yakinsanan == "")
		{
			yakinsanan = yakinsa(kokrtf, arakelime, arakelime.length());
			if (yakinsanan == "")
			{
				yakinsanan = yakinsa(kokhtm, arakelime, arakelime.length());
				if (yakinsanan == "")
				{
					yakinsanan = yakinsa(kokxml, arakelime, arakelime.length());
				}
			}
		}
		cout << "Sunu mu demek istediniz: " << yakinsanan;
	}
	cout << "Arama yapildi. Gecen sure: ";
	cout << float(clock() - begin_time) / CLOCKS_PER_SEC;
	cout << endl;
	
	return 0;
}

tree *olustur(tree *kok)
{
	kok = new tree();
	kok->kardes = NULL;
	kok->cocuk = NULL;
	kok->harf = NULL;
	kok->yer = "";
	kok->son = 0;

	//Alfabetik sýralama için alfabeyi giriyoruz
	tree *alfsira = kok;
	kok = ilkcocuk(alfsira, 'a', 0, 0);
	alfsira = alfsira->cocuk;
	for (char i = 'b'; i < 'z'; i++)
	{
		alfsira = kardesekle(alfsira, i, 0, 0);
		alfsira = alfsira->kardes;
	}
	for (char i = '0'; i < '9'; i++)
	{
		alfsira = kardesekle(alfsira, i, 0, 0);
		alfsira = alfsira->kardes;
	}
	return kok;
}

tree *ilkcocuk(tree *kok, char gharf, int kelimesonu, int kelimeyeri)
{//suanki, harf, 1, sayac
	kok->cocuk = new tree();
	kok->cocuk->harf = gharf;
	kok->cocuk->cocuk = NULL;
	kok->cocuk->kardes = NULL;
	kok->cocuk->son = kelimesonu;
	if (kelimesonu > 0)
		kok->cocuk->yer = to_string(kelimeyeri);
	return kok;
}
tree *kardesekle(tree *kok, char gharf, int kelimesonu, int kelimeyeri)
{
	kok->kardes = new tree();
	kok->kardes->harf = gharf;
	kok->kardes->cocuk = NULL;
	kok->kardes->kardes = NULL;
	kok->kardes->son = kelimesonu;
	if (kelimesonu > 0)
		kok->kardes->yer = to_string(kelimeyeri);
	return kok;
}

char bkelime[42] = "@";
void yazdir(tree *kok, int uz)
{
	if (kok == NULL)
		return;
	else
	{
		bkelime[uz] = kok->harf;
		bkelime[uz + 1] = '@';
		if (kok->son != 0)
		{
			for (int i = 1; i <= uz; i++)
			{
				cout << bkelime[i];
			}
			cout << " x" << kok->son << endl;
		}
		yazdir(kok->cocuk, uz + 1);

		bkelime[uz] = '@';
		bkelime[uz + 1] = ' ';

		yazdir(kok->kardes, uz);
	}
}

pair <int, string> ara(tree *kok, string ara)
{
	if (kok == NULL)
		return make_pair(0, "");

	tree *suanki = kok->cocuk;
	for (int i = 0; i < ara.length(); i++)
	{
		char harf = ara[i];
		if (suanki != NULL)
		{
			while (suanki->kardes != NULL)
			{
				if (suanki->harf == harf)
					break;
				suanki = suanki->kardes;
			}
			if (suanki->harf == harf)
				if (i == ara.length() - 1)
					return make_pair(suanki->son, suanki->yer);
				else
					suanki = suanki->cocuk;
			else
				return make_pair(0, "");
		}
	}
	return make_pair(0, "");
}

int limit = 2;
string yakinsa(tree *kok, string ara, int uzaklik)
{
	if (kok == NULL)
		return "";
	else
	{
		string gelen = "";
		tree *suanki = kok->cocuk;
		char harf = ara[0];
		while (suanki != NULL)
		{
			if (suanki->harf == harf)
				if (ara.length() > 0)
					gelen = yakinsa(suanki, ara.substr(1, ara.length()), uzaklik - 1);
				else
					return "";
			else
				if (ara.length() > 0)
					gelen = yakinsa(suanki, ara.substr(1, ara.length()), uzaklik);
				else
					return "";
			if (gelen != "")
			{
				string yeniharf = string(1, suanki->harf);
				return yeniharf + gelen;
			}
			suanki = suanki->kardes;
		}
		if (uzaklik < limit && kok != NULL)
			if (kok->son > 0)
				return " ";
		return "";
	}
}


tree *oku(tree *kok, int dosyano)
{
	string kelime;
	ifstream dosya;
	if (dosyano == 1)
		dosya.open("Parca1.txt");
	if (dosyano == 2)
		dosya.open("Parca2.rtf");
	if (dosyano == 3)
		dosya.open("Parca3.html");
	if (dosyano == 4)
		dosya.open("Parca4.xml");
	
	if (!dosya.is_open()) {
		cout << dosyano << " adli dosya bulunamadi.\n";
		return 0;
	}
	int sayac = 0;
	do
	{
		sayac++;
		dosya >> kelime;
		char sonharf = tolower(kelime[kelime.length() - 1]);
		if (!(sonharf > 'a' && sonharf < 'z') && !(sonharf > '0' && sonharf < '9'))
		{
			kelime = kelime.substr(0, kelime.length() - 1);
		}
		tree *suanki = kok;
		for (int j = 0; j < kelime.length(); j++)
		{
			char harf = tolower(kelime[j]);
			if (suanki->cocuk == NULL)
			{
				if (j == kelime.length() - 1)
					suanki = ilkcocuk(suanki, harf, 1, sayac);
				else
					suanki = ilkcocuk(suanki, harf, 0, 0);
				suanki = suanki->cocuk;
			}
			else
			{
				suanki = suanki->cocuk;
				while (suanki->kardes != NULL)
				{
					if (suanki->harf == harf)
						break;
					suanki = suanki->kardes;
				}
				if (suanki->harf == harf)
				{
					if (j == kelime.length() - 1)
					{
						suanki->son++;
						suanki->yer += ";" + to_string(sayac);
					}
				}
				else
				{
					if (j == kelime.length() - 1)
						kardesekle(suanki, harf, 1, sayac);
					else
						kardesekle(suanki, harf, 0, 0);
					suanki = suanki->kardes;
				}
			}
		}
	} while (!dosya.eof());
	dosya.close();
	return kok;
}
