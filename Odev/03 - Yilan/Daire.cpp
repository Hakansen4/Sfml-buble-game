#include "Daire.hpp"
#include "Pencere.hpp"
#include "cmath"

Daire::Daire(float boyut)
{
	this->m_boyut = boyut;
	m_sekil.setRadius(boyut);
	//m_yon = YON::YON_SAG;
	m_hiz = boyut / 10;
	m_ileriHiz = 5;
	m_carpistigiDaireSayisi = 0;
	silinsinmi = false;
	isHoldingLine = false;
	chooseColor();
}

DairePtr Daire::daireUret(float boyut)
{
	return std::make_shared<Daire>(boyut);

}
void Daire::ciz(Pencere& pencere)
{
	m_sekil.setPosition(m_konum);
	if (!silinsinmi)
		pencere.ciz(m_sekil);
}

void Daire::hareket()
{
	m_konum.x += m_yon * m_hiz;
	m_konum.y -= m_ileriHiz;
}

void Daire::yonBelirle(float yon)
{
	m_yon = yon/10;
}

bool Daire::isCollidedwCircle(const sf::Vector2f& daireKonum)
{
	if (!silinsinmi)
	{
		float dx = m_konum.x - daireKonum.x;
		float dy = m_konum.y - daireKonum.y;
		float distance = std::sqrt((dx * dx) + (dy * dy));
		if (distance <= (this->m_boyut * 2))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
		return false;
}

bool Daire::isCollidedwSquareR(float squareKonumX)
{
	if (m_konum.x + m_boyut > squareKonumX)
		return true;
	else
		return false;
}

bool Daire::isCollidedwSquareL(float squareKonumX, float width)
{
	if (m_konum.x + m_boyut < squareKonumX + width)
		return true;
	else 
		return false;
}

bool Daire::isCollidedwSquareF(float konumY, float Uzunluk)
{
	if (m_konum.y + m_boyut <= konumY + Uzunluk*3)
		return true;
	else
		return false;
}

void Daire::chooseColor()
{
	int number = rand() % 4;
	if (number == 0)
	{
		m_sekil.setFillColor(sf::Color::Green);
		colorName = "Green";
	}
	else if (number == 1)
	{
		m_sekil.setFillColor(sf::Color::Red);
		colorName = "Red";
	}
	else if (number == 2)
	{
		m_sekil.setFillColor(sf::Color::Yellow);
		colorName = "Yellow";
	}
	else if (number == 3)
	{
		m_sekil.setFillColor(sf::Color::Blue);
		colorName = "Blue";
	}

}
