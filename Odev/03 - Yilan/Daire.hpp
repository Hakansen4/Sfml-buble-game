#pragma once
#include "Cizilebilir.hpp"
#include "TemelTurler.hpp"
class Daire :public Cizilebilir
{
public:
	Daire(float boyut);
	static std::shared_ptr<Daire> daireUret(float boyut);
	virtual void ciz(Pencere& pencere) override;
	void hareket();
	void yonBelirle(float);
	bool isCollidedwCircle(const sf::Vector2f& daireKonum);
	bool isCollidedwSquareR(float squareKonumX);
	bool isCollidedwSquareL(float squareKonumX,float width);
	bool isCollidedwSquareF(float, float);
	void chooseColor();

	std::string colorName;
	float id;
	std::vector<float> carpistigiDaireIdler;
	float m_boyut;
	float m_hiz;
	float m_ileriHiz;
	float m_carpistigiDaireSayisi;
	bool silinsinmi;
	bool isHoldingLine;
	//YON	m_yon;
	sf::CircleShape m_sekil;
	float m_yon;



};
typedef std::shared_ptr<Daire>	DairePtr;
typedef std::vector<DairePtr>	DaireListesi;