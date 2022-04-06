#pragma once
#include "TemelTurler.hpp"
#include "Pencere.hpp"
#include "AracPaneli.hpp"
#include "Yilan.hpp"
#include "Daire.hpp"
class Oyun
{
public:
	Oyun();
	~Oyun();
	void setFps(int fps);
	void oyunuBaslat(unsigned int genislik=600,unsigned int yukseklik= 600);
	void cizimFonksiyonu();
	void kaplamaEkle(sf::String dosyaIsmi, bool arkaPlanOlsunmu = true);
	void oyunuAyarla();
	void arayuzleriYukle();
	void olaylariBagla();
	void fareHareket(int x, int y);
	void fareBasildi(sf::Event::MouseButtonEvent olay);
	void fareBirakma(sf::Event::MouseButtonEvent olay);
	void klavyeBasildi(sf::Keyboard::Key tus);
	void oyunuKapat();
	void daireUret();
	void silinenlerinKonumunuDegistir();
	void carpisanIdlereKaydet(DairePtr);
	bool dusmeKontrol(int);
	void dusur(int);
	void oyunBittiKontrol();
	void tumDaireleriSil();
	void carpisanIdlerdenSil(float id);

private:

	void saatiYenidenBaslat();

	DairePtr			m_daire;
	DaireListesi		m_daireList;
	sf::Time			m_gecenSure;
	sf::Time			m_cerceveSuresi;
	
	sf::RectangleShape  ustSinir;
	sf::RectangleShape  sagSinir;
	sf::RectangleShape  solSinir;
	sf::RectangleShape  dikdortgen;
	float				dikdortgenAngle;
	float				daireIdler;
	float				carpisanIdler[100][100];
	bool				yeniDaireAteslendimi;

	sf::Clock			m_saat;
	Pencere				m_pencere;
	KaplamaHaritasi		m_aracKaplamaListesi;
	int					m_fps;
	AracPaneliListesi	m_panelListesi;
	FontPtr				m_font;
	int					m_genislik;
	int					m_yukseklik;
	bool				m_devamEdiyormu;
};