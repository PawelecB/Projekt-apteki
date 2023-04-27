// Bartlomiej Pawelec
// WCY21KY2S1
// Projekt Apteka
#include<iostream>
#include<cstdlib>
#include<time.h>
#include<vector>
#include<fstream>

using namespace std;

typedef int liczba;		// Deklaracja typu

template <typename T>
void Incr(T dane)
{
	dane++;
}

class Lek
{
	protected:
		string nazwa;
		bool naRecepte;
		int ilosc;
	public:
		virtual void PrintLek()
		{
			cout<<"Nazwa: "<<nazwa<<" Na Recepte: "<<(naRecepte? " Tak " : " Nie ")<<" Ilosc: "<<ilosc<<endl;
		}
		// Konstruktor domyslny
		Lek()
		{
			
		}
		Lek(string nazwa,bool recepta,int ile)
		{
			this->nazwa=nazwa;
			this->naRecepte=recepta;
			this->ilosc=ile;
		}
		string getNazwa()
		{
			return nazwa;
		}
		bool getRecepta()
		{
			return naRecepte;
		}
		int getIlosc()
		{
			return ilosc;
		}
		void operator+(int i)
		{
			ilosc+=i;
		}	
};

class Lek_detal : public Lek 		// Klasa dziedziczna po klasie Lek
{
	friend void zmienIlosc(Lek_detal &o,int i);
	private:
		int cena; 	// Dodatkowe pole ilosc
	public:	
	// Konstruktor
	Lek_detal(string nazwa,bool recepta,int ile,int cena)
		{
			this->nazwa=nazwa;
			this->naRecepte=recepta;
			this->ilosc=ile;
			this->cena=cena;
		}
		
	void PrintLek()
	{
		cout<<"Nazwa: "<<nazwa<<" Na Recepte: "<<(naRecepte? " Tak " : " Nie ")<<" Ilosc: "<<ilosc<<" Cena:  "<<cena<<endl;
	}
	void saveStan(string sciezka)
	{
		fstream plik;
		plik.open(sciezka.c_str(), fstream::app);
		plik<<"Nazwa: "<<nazwa<<" Na Recepte: "<<(naRecepte? " Tak " : " Nie ")<<" Ilosc: "<<ilosc<<" Cena: "<<cena<<endl;
		plik.close();
	}
	// Preciazenie operatora
	virtual void operator+(int i)
	{
		ilosc+=i;
	}
	int getCena()
	{
		return cena;
	}
};

class Recepta 
{
	friend class Lek;		// Zaprzyjaznienie z klasa Lek
	private:
		int kod;
		int peselPacjenta;
		string nazwiskoLekarza;
		vector<Lek> przepisaneLeki;		// Zapisane leki na danego pacjenta (pesel) i dany kod (kod) przez lekarza (nazwiskoLekarza)
	public:
		// Konstruktor ( wystawiona recepte archiwizuje do pliku )
		Recepta(int kod,int pesel,string nazwisko)
		{
			fstream plik;
			plik.open("recepta_zapis.txt", fstream::app);
			plik<<"Kod recepty: "<<kod<<" Pesel pacjenta: "<<pesel<<" Nazwisko lekarza: "<<nazwisko<<endl;
			this->kod=kod;
			this->peselPacjenta=pesel;
			this->nazwiskoLekarza=nazwisko;
			int a;
			while(1)
			{
				cout<<" Dodac lek do recepty? "<<endl<<"0. Nie"<<endl<<"1. Tak"<<endl;
				cin>>a;
				if(a==1)
				{
					string n;
					int ile;
					bool r;
					cout<<"Podaj nazwe: "<<endl;
					cin>>n;
					cout<<"Czy jest on na recepte? "<<endl<<"0. Nie"<<endl<<"1. Tak"<<endl;
					cin>>r;
					cout<<"Podaj ilosc: "<<endl;
					cin>>ile;
					Lek* lek=new Lek(n,r,ile);
					przepisaneLeki.push_back(*lek);
					plik<<"	Nazwa leku: "<<n<<" Na recepte? "<<r<<" Ilosc "<<ile<<endl;
				}
				else
				{
					break;
				}
			}
			plik<<"------------------------------------------"<<endl;
			plik.close();
		}
		void PrintRecepta()
		{	
			cout<<"PESEL pacjenta "<<peselPacjenta<<" Kod Recepty "<<kod<<" Nazwisko wystawiajacego "<<nazwiskoLekarza<<endl;
			cout<<" Wystawione Leki: "<<endl;
			for(int i=0; i<przepisaneLeki.size();i++)
			{
				cout<<"	"<<i+1<<".";
				przepisaneLeki[i].PrintLek();
			}
		}
		int getPESEL()
		{
			return peselPacjenta;
		}
		int getKod()
		{
			return kod;
		}
		int ileLekow()
		{
			return przepisaneLeki.size();
		}
		string returnNazwa(int i)
		{
			return przepisaneLeki[i].getNazwa();
		}
		bool returnRecepta(int i)
		{
			return przepisaneLeki[i].getRecepta();
		}
		int returnIlosc(int i)
		{
			return przepisaneLeki[i].getIlosc();
		}
		void usunLek(int i)
		{
			przepisaneLeki.erase(przepisaneLeki.begin()+i);
		}
		void zmienIlosc(int i,int y)
		{
			przepisaneLeki[i]+y;
		}
};

// 	Funkcja zaprzyjazniona z klasa Lista ( ktora dziedziczy po klasie lek ) do zmiany ilosci 
void zmienIlosc(Lek_detal &o,int i)
{
	o.ilosc+=i;
}

int main()
{
	vector<Lek_detal*> Magazyn;
	vector<Recepta*> Recepty;
	
	// Definicja dwoch przykladowych lekow 
	Magazyn.push_back(new Lek_detal("LekBezRecepty",0,10,20));
	Magazyn.push_back(new Lek_detal("LekNaRecepte",1,5,10));
	
	liczba c=1;
	
	while(1)
	{
		cout<<" Logowanie: "<<endl
		<< "1. Farmaceuta "<<endl
		<< "2. Lekarz "<<endl
		<< "3. Klient "<<endl
		<< "4. Opusc program"<<endl;
		cin>>c;
		switch(c)
		{
			case 1:
				{
					liczba p;
					bool s=1;
					menu_farmaceuta:
					while(s)
					{
						cout<<" Menu: "<<endl
						<< "1. Dodaj lek "<<endl
						<< "2. Sprawdz magazyn "<<endl
						<< "3. Wyloguj sie "<<endl;
						cin>>p;
						switch(p)
						{
							case 1:
								{
									string n;
									int ile,p;
									bool r;
									cout<<"Podaj nazwe: "<<endl;
									cin>>n;
									cout<<"Ile paczek nalezy dodac: "<<endl;
									cin>>ile;
									// Jezeli lek wystepuje w magazynie, to zmienia tylko jego ilosc 
									for(int i=0;i<Magazyn.size();i++)
									{
										if(Magazyn[i]->getNazwa()==n)
										{
											// Wykonanie przeciazenia operatora
											*Magazyn[i]+ile;
											goto menu_farmaceuta;	// Przeskok do menu farmaceuty
										}
									}
									cout<<"Czy jest on na recepte? "<<endl<<"0. Nie"<<endl<<"1. Tak"<<endl;
									cin>>r;
									cout<<"Cena: "<<endl;
									cin>>p;
									Magazyn.push_back(new Lek_detal(n,r,ile,p));
									Magazyn[Magazyn.size()-1]->saveStan("zapisz_magazyn.txt");
									break;
								}
							case 2:
								{
									for(int i=0;i<Magazyn.size();i++)
									{
										Magazyn[i]->PrintLek();
									}
									break;
								}
							case 3:
								{
									cout<<"---Wylogowywanie---"<<endl<<endl;
									s=0;
									break;	
								}
						}
					}
					break;					
				}
			case 2:
				{
					string nazwLekarz;
					cout<<" Podaj swoje nazwisko: "<<endl;
					cin>>nazwLekarz;
					liczba p,k=100;
					bool s=1;
					while(s)
					{
						cout<<" Menu: "<<endl
						<<"1. Wystaw recepte "<<endl
						<<"2. Wystawione recepty"<<endl
						<<"3. Wyloguj sie "<<endl;
						cin>>p;
						switch(p)
						{
							case 1:
								{
									int p;
									cout<<"Podaj pesel pacjenta"<<endl;
									cin>>p;	
									Recepty.push_back(new Recepta(k,p,nazwLekarz));
									cout<<"Kod wystawionej recepty: "<<k<<endl;
									Incr(k);			// Wykorzystanie zdefiniowanej funkcji
									break;
								}
							case 2:
								{
									for(int i=0;i<Recepty.size();i++)
									{
										Recepty[i]->PrintRecepta();
									}
									break;
								}
							case 3:
								{
									cout<<"---Wylogowywanie---"<<endl<<endl;
									s=0;
									break;
								}
						}
					}
					break;	
				}
			case 3:
				{
					vector<Lek_detal*> Koszyk;
					liczba p;
					bool s=1;
					while(s)
					{ 
						cout<<" Menu: "<<endl
						<<"1. Zrealizuj recepte"<<endl
						<<"2. Dodaj lek do koszyka"<<endl
						<<"3. Wyswietl oferte"<<endl
						<<"4. Wyswietl koszyk"<<endl
						<<"5. Wyloguj sie (skasowanie koszyka!)"<<endl;
						cin>>p;
						switch(p)
						{
							case 1:
								{
									int ps,k;
									cout<<"Podaj PESEL: "<<endl;
									cin>>ps;
									cout<<"Podaj kod: "<<endl;
									cin>>k;
									for(int i=0;i<Recepty.size();i++)
									{
										if((Recepty[i]->getKod())==k && (Recepty[i]->getPESEL())==ps)	// Odszukiwanie recepty dla danego nr pesel i kodu
										{	
											int temp=Recepty[i]->ileLekow();
											for(int j=0;j<temp;j++)	// Dodawanie lekow do koszyka
											{
												// Zapisanie informacji o leku na recepcie do zmiennych
												string nazwaR=Recepty[i]->returnNazwa(j);
												bool receptaR=Recepty[i]->returnRecepta(j);
												int iloscR=Recepty[i]->returnIlosc(j);
												for(int k=0;k<Magazyn.size();k++)	// Sprawdzanie leku na magazynie
												{
													// Poszukiwanie leku o danej nazwie i sprawdzanie czy znajduje sie na magazynie
													if(Magazyn[k]->getNazwa()==nazwaR && Magazyn[k]->getIlosc()>0)
													{
														if(Magazyn[k]->getIlosc()-iloscR<0)
														{
															iloscR=Magazyn[k]->getIlosc();
															cout<<" W magazynie jest niewystarczajaca ilosc lekow, do koszyka dodano "
															<<iloscR<<" sztuk leku "<<nazwaR<<endl;
														}
														Koszyk.push_back(new Lek_detal(nazwaR,receptaR,iloscR,iloscR*Magazyn[k]->getCena()));
														zmienIlosc(*Magazyn[k],-iloscR);	// Przeciazenie operatora za pomoca funkcji zaprzyjaznionej 
														if(iloscR==Recepty[i]->returnIlosc(j))
														{
															Recepty[i]->usunLek(j);					// Usuniecie pojedynczego leku na recepcie po CALKOWITEJ realizacji
															cout<<"---Dodano "<<nazwaR<<" do koszyka---"<<endl;
														}
														else
														{
															Recepty[i]->zmienIlosc(j,-iloscR);		// Jezeli w aptece nie bylo tylu lekow, ilosc lekow jest odejmowana
																									// z recepty
														}
														if(Recepty[i]->ileLekow()==0)
														{
															Recepty.erase(Recepty.begin()+i);	//Usuniecie recepty po jej CALKOWITEJ realizacji
														}
														break;	// Dodano lek == zakonczenie poszukiwan w magazynie
													}
													if(k==Magazyn.size()-1)	// Ostatnia iteracja i nie znaleziono leku
													{
														cout<<"---Nie znaleziono leku "<<nazwaR<<"---"<<endl;
													}
												}
											}
											break;	 
										}
										if(i==Recepty.size()-1)	// Ostatnia iteracja i nie znaleziono recepty
										{
											cout<<"---Nie znaleziono recepty o okreslonym numerze pesel lub kodzie---"<<endl;
										}
									}
									break;
								}
							case 2:
								{
									string n;
									int ile;
									cout<<"Podaj nazwe leku"<<endl;
									cin>>n;
									cout<<"Podaj ilosc do zakupu"<<endl;
									cin>>ile;
									for(int i=0;i<Magazyn.size();i++)
									{
										if(Magazyn[i]->getNazwa()==n)
										{
											if(Magazyn[i]->getRecepta()==1)
											{
												cout<<"Ten lek jest na recepte - nie mozna go zakupic"<<endl;
												i=Magazyn.size(); // Zakonczenie dzialania petli
											}
											else
											{
												if(Magazyn[i]->getIlosc()-ile<0)
												{
													ile=Magazyn[i]->getIlosc();
													cout<<" W magazynie jest niewystarczajaca ilosc lekow, do koszyka dodano "
													<<ile<<" sztuk leku "<<n<<endl;
												}
												Koszyk.push_back(new Lek_detal(n,0,ile,ile*Magazyn[i]->getCena()));
												zmienIlosc(*Magazyn[i],-ile);	// Przeciazenie operatora za pomoca funkcji zaprzyjaznionej
												i=Magazyn.size(); // Zakonczenie dzialania petli 	
											}
										}
										else if(i==Magazyn.size()-1)	// Gdy nie znaleziono leku a jestesmy na ostatniej iteracji petli
										{
											cout<<"---Nie znaleziono leku "<<n<<"---"<<endl;
										}
									}
									break;
								}
							case 3:
								{
									for(int i=0;i<Magazyn.size();i++)
									{
										Magazyn[i]->PrintLek();
									}
									break;	
								}
							case 4:
								{
									for(int i=0;i<Koszyk.size();i++)
									{
										Koszyk[i]->PrintLek();
									}
									break;
								}
							case 5:
								{
									// Przed wylogowaniem archiwizowany jest stan koszyka 
									for(int i=0;i<Koszyk.size();i++)
									{
										Koszyk[i]->saveStan("zapis_koszyk.txt");
									}
									s=0;
									break;
								}
						}
					}
					break;
				}
			case 4:
				{
					return 0; 
				}
		}
	}
}
