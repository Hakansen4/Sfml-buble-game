#include "Oyun.hpp"
#include <iostream>
Oyun::Oyun()
{
	m_fps = 60;
	m_cerceveSuresi = sf::seconds(1.0f / m_fps);
	m_devamEdiyormu = false;
	dikdortgen.setSize(sf::Vector2f(8, 60));
	dikdortgen.setPosition(sf::Vector2f(300, 620));
	dikdortgen.setOrigin(sf::Vector2f(5, 80));
	ustSinir.setSize(sf::Vector2f(600, 10));
	ustSinir.setFillColor(sf::Color::Red);
	sagSinir.setSize(sf::Vector2f(10, 600));
	sagSinir.setFillColor(sf::Color::Red);
	sagSinir.setPosition(sf::Vector2f(590, 0));
	solSinir.setSize(sf::Vector2f(10, 600));
	solSinir.setFillColor(sf::Color::Red);
	daireIdler = 0;
	yeniDaireAteslendimi = false;
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j< 100; j++)
		{
			carpisanIdler[i][j] = 20000;
		}
	}
}

Oyun::~Oyun()
{
}

void Oyun::setFps(int fps)
{
}
void Oyun::kaplamaEkle(sf::String dosyaIsmi, bool arkaPlanOlsunmu)
{
	if (m_aracKaplamaListesi.count(dosyaIsmi) == 0)
	{
		TexturePtr yeniKaplama = std::make_shared<sf::Texture>();
		sf::String yol = "./resimler/" + dosyaIsmi;
		std::filesystem::path tamyol(yol);
		
		if (arkaPlanOlsunmu)
		{
			if (yeniKaplama->loadFromFile(tamyol))
			{
				m_aracKaplamaListesi[dosyaIsmi] = yeniKaplama;
			}
		}
		else
		{
			sf::Image image;
			if (image.loadFromFile(tamyol))
			{
				auto maskeRengi = image.getPixel(0, 0);
				image.createMaskFromColor(maskeRengi);
				yeniKaplama->loadFromImage(image);
				m_aracKaplamaListesi[dosyaIsmi] = yeniKaplama;
			}
		}

	}

}
void Oyun::oyunuAyarla()
{	
	arayuzleriYukle();
	olaylariBagla();
}
void Oyun::arayuzleriYukle()
{
	m_font = std::make_shared<sf::Font>();
	if (!m_font->loadFromFile("./fontlar/alev.ttf"))
	{
		std::cout << "font bulunamadi" << std::endl;
	}
	kaplamaEkle("Button.png");
	kaplamaEkle("Windows.png");
	AracPaneliPtr yeniPanel = std::make_shared<AracPaneli>();
	yeniPanel->fontAta(m_font);
	yeniPanel->aracKaplamasi(m_aracKaplamaListesi["Button.png"]);
	yeniPanel->arkaPlanAta(m_aracKaplamaListesi["Windows.png"], sf::IntRect(sf::Vector2i(799, 933), sf::Vector2i(550, 742)));
	

	auto btnYeniOyun = yeniPanel->butonEkle("Yeni Oyun");
	auto btnListe = yeniPanel->butonEkle("Liste");
	auto btnCikis = yeniPanel->butonEkle("Cikis");

	btnCikis->tiklaFonksiyonuEkle([&] {
		this->oyunuKapat();
		});

	btnYeniOyun->tiklaFonksiyonuEkle([&] {
		m_panelListesi[0]->gorunurluk(false);
		m_devamEdiyormu = true;
		});

	float x = (m_genislik - yeniPanel->boyutGetir().x) / 2;
	float y = (m_yukseklik - yeniPanel->boyutGetir().y) / 2;
	yeniPanel->konumAta(sf::Vector2f(x, y));
	
	m_panelListesi.push_back(yeniPanel);
}
void Oyun::olaylariBagla()
{
	m_pencere.hareketFonksiyonuEkle(
		std::bind(&Oyun::fareHareket, this, std::placeholders::_1, std::placeholders::_2));

	m_pencere.fareBasilmaFonksiyonuEkle(
		std::bind(&Oyun::fareBasildi, this,std::placeholders::_1));
	
	m_pencere.fareBirakilmaFonksiyonuEkle(
		std::bind(&Oyun::fareBirakma, this, std::placeholders::_1));
	m_pencere.tusBasilmaFonksiyonuEkle(
		std::bind(&Oyun::klavyeBasildi, this, std::placeholders::_1));

}
void Oyun::fareHareket(int x, int y)
{
	for (auto siradaki : m_panelListesi)
	{
		if(siradaki->icerdemi(x,y))
			siradaki->fareHareket(x, y);

	}
		
	
}
void Oyun::fareBasildi(sf::Event::MouseButtonEvent olay)
{
	int x = olay.x;
	int y = olay.y;
	for (auto siradaki : m_panelListesi)
	{
		if (siradaki->icerdemi(x, y))
			siradaki->fareButonBasildi(olay);

	}
}
void Oyun::fareBirakma(sf::Event::MouseButtonEvent olay)
{
	int x = olay.x;
	int y = olay.y;
	for (auto siradaki : m_panelListesi)
	{

		siradaki->fareButonBirakildi(olay);

	}
}
void Oyun::klavyeBasildi(sf::Keyboard::Key tus)
{
	if (tus == sf::Keyboard::Left)
	{
		dikdortgenAngle -= 5;
		std::cout << dikdortgenAngle << std::endl;
	}
	if (tus == sf::Keyboard::Right)
	{
		dikdortgenAngle += 5;
		std::cout << dikdortgenAngle << std::endl;
	}

	if (tus == sf::Keyboard::Space)
	{
		if (!yeniDaireAteslendimi)
		{
			m_daire = Daire::daireUret(20.0f);
			m_daire->id = daireIdler;
			daireIdler++;
			daireUret();
			m_daire->yonBelirle(dikdortgenAngle);
			m_daireList.push_back(m_daire);
			yeniDaireAteslendimi = true;
			if (m_daire->id == 0)
				carpisanIdler[0][0] = m_daire->id;
		}
	}
}
void Oyun::oyunuKapat()
{
	m_pencere.kapat();
}
void Oyun::daireUret()
{
	m_daire->m_konum.x = 280;
	m_daire->m_konum.y = 580;
}
void Oyun::silinenlerinKonumunuDegistir()
{
	for (auto next : m_daireList)
	{
		if (next->silinsinmi)
		{
			next->m_konum = sf::Vector2f(1000, 1000);
		}
	}
}
void Oyun::carpisanIdlereKaydet(DairePtr daire)
{
	for (int x = 0; x < 100; x++)
	{
		for (int y = 0; y < 100; y++)
		{
			if (carpisanIdler[x][y] == daire->id)
			{
				for (int i = 0; i < 100; i++)
				{
					if (carpisanIdler[x][i] == 20000)
					{
						carpisanIdler[x][i] = m_daire->id;
						return;
					}
				}
			}
		}
	}
	for (int x = 0; x < 100; x++)
	{
		for (int y = 0; y < 1; y++)
		{
			if (carpisanIdler[x][y] == 20000)
			{
				carpisanIdler[x][y] = m_daire->id;
				return;
			}
		}
	}
}

bool Oyun::dusmeKontrol(int index)
{
	for (int i = 0; i < 100; i++)
	{
		if (carpisanIdler[index][i] != 20000)
		{
			for (auto daire : m_daireList)
			{
				if (daire->id == carpisanIdler[index][i])
				{
					if (daire->isHoldingLine)
						return true;
				}
			}
		}
	}
	return false;
}

void Oyun::dusur(int index)
{
	for (int i = 0; i < 100; i++)
	{
		for (auto daire : m_daireList)
		{
			if (daire->id == carpisanIdler[index][i])
			{
				daire->silinsinmi = true;
				carpisanIdler[index][i] = 20000;
			}
		}
	}
}

void Oyun::oyunBittiKontrol()
{
	for (auto daire : m_daireList)
	{
		if (daire->m_konum.y + 2 * daire->m_boyut > m_yukseklik	&&	!daire->silinsinmi)
		{
			//Oyun bitti
			m_devamEdiyormu = false;
			tumDaireleriSil();
			arayuzleriYukle();
			return;
		}
	}
}

void Oyun::tumDaireleriSil()
{
	for (auto daire : m_daireList)
	{
		daire->silinsinmi = true;
		daire->isHoldingLine = false;
		daire->m_konum = sf::Vector2f(1000, 1000);
		carpisanIdlerdenSil(daire->id);
	}
}

void Oyun::carpisanIdlerdenSil(float id)
{
	for (int x=0;x<100;x++)
	{
		for (int y=0;y<100;y++)
		{
			if (id == carpisanIdler[x][y])
			{
				carpisanIdler[x][y] = 20000;
				return;
			}
		}
	}
}

void Oyun::oyunuBaslat(unsigned int genislik, unsigned int yukseklik)
{
	m_genislik = genislik;
	m_yukseklik = yukseklik;
	m_pencere.olustur(genislik, yukseklik, "SFML");

	oyunuAyarla();

	saatiYenidenBaslat();
	while (m_pencere.acikmi())
	{
		m_pencere.olayKontrol();
		
		if (m_saat.getElapsedTime() >= m_cerceveSuresi)
		{
			cizimFonksiyonu();
			saatiYenidenBaslat();
		}
		else
		{
			sf::sleep(m_cerceveSuresi - m_saat.getElapsedTime());
		}

	}
}

void Oyun::cizimFonksiyonu()
{
	m_pencere.cizimeBasla();
	for (auto siradaki : m_panelListesi)
	{
		if (!m_devamEdiyormu)
			siradaki->ciz(m_pencere);
	}
	if (m_devamEdiyormu)
	{
		if (m_daireList.size() > 1)
		{
			for (auto siradaki : m_daireList)
			{
				if (m_daire->isCollidedwCircle(siradaki->m_konum)	&&	m_daire->m_konum != siradaki->m_konum	&&	yeniDaireAteslendimi)
				{
					oyunBittiKontrol();
					m_daire->m_hiz = 0;
					m_daire->m_ileriHiz = 0;
					m_daire->carpistigiDaireIdler.push_back(siradaki->id);
					siradaki->carpistigiDaireIdler.push_back(m_daire->id);
					carpisanIdlereKaydet(siradaki);
					//bundan once ayni renk sorgula
					if (siradaki->colorName == m_daire->colorName)
					{
						m_daire->m_carpistigiDaireSayisi++;
						siradaki->m_carpistigiDaireSayisi++;
					}
					//---------------
					if (m_daire->m_carpistigiDaireSayisi >= 2 || siradaki->m_carpistigiDaireSayisi >= 2)
					{
						m_daire->silinsinmi = true;
						siradaki->silinsinmi = true;
						m_daire->isHoldingLine = false;
						siradaki->isHoldingLine = false;
						//carpisanIdlerdenSil(m_daire->id);
						//carpisanIdlerdenSil(siradaki->id);
						for (auto next : m_daireList)
						{
							for (auto next2 : m_daire->carpistigiDaireIdler)
							{
								if (next->id == next2)
								{
									if (m_daire->colorName == next->colorName)
									{
										next->silinsinmi = true;
										next->isHoldingLine = false;
									}
								}
							}
							for (auto next3 : siradaki->carpistigiDaireIdler)
							{
								if (next->id == next3)
								{
									if (siradaki->colorName == next->colorName)
									{
										next->silinsinmi = true;
										next->isHoldingLine = false;
									}
								}
							}
						}
						int index;
						for (int i = 0; i < 100; i++)
						{
							for (int j = 0; j < 100; j++)
							{
								if (siradaki->id == carpisanIdler[i][j])
								{
									index = i;
									break;
								}
							}
						}
						if (!dusmeKontrol(index))
						{
							dusur(index);
						}
					}
					yeniDaireAteslendimi = false;
				}
			}
		}
		silinenlerinKonumunuDegistir();
		if (m_daireList.size() > 0)
		{
			if (m_daire->isCollidedwSquareR(sagSinir.getPosition().x))
			{
				m_daire->m_hiz *= -1;
			}
		}
		if (m_daireList.size() > 0)
		{
			if (m_daire->isCollidedwSquareL(solSinir.getPosition().x,10))
			{
				m_daire->m_hiz *= -1;
			}
		}
		if (m_daireList.size() > 0)
		{
			if (m_daire->isCollidedwSquareF(ustSinir.getPosition().y, 10))
			{
				m_daire->m_hiz = 0;
				m_daire->m_ileriHiz = 0;
				m_daire->isHoldingLine = true;
				yeniDaireAteslendimi = false;
				if (m_daire->id != 0)
					carpisanIdlereKaydet(m_daire);
			}
		}
		if (m_daireList.size() != 0)
			m_daireList.back()->hareket();
		m_pencere.ciz(dikdortgen);
		dikdortgen.setRotation(sf::degrees(dikdortgenAngle));
		m_pencere.ciz(ustSinir);
		m_pencere.ciz(sagSinir);
		m_pencere.ciz(solSinir);	
		if (m_daireList.size() != 0)
		{
			for (auto siradaki : m_daireList)
			{
				if (!siradaki->silinsinmi)
					siradaki->ciz(m_pencere);
			}
		}
	}
	m_pencere.cizimiBitir();
}

void Oyun::saatiYenidenBaslat()
{
	m_saat.restart();
}
