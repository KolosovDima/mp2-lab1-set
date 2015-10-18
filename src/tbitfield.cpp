// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if(len>0)
	{
	BitLen=len;
	MemLen=(len+sizeof(TELEM)-1)/sizeof(TELEM);
	pMem=new TELEM[MemLen];
	for (int i=0;i<MemLen;i++)
		pMem[i]=0;
	}
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	if (pMem!=bf.pMem)
	{
		BitLen=bf.BitLen;
		MemLen=bf.MemLen;
		pMem=new TELEM[MemLen];
		memcpy(pMem,bf.pMem,sizeof(TELEM)*MemLen);
	}
}

TBitField::~TBitField()
{
	if (pMem!=NULL)
		delete []pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n/sizeof(TELEM);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{	
	return 1<<(n%sizeof(TELEM));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	pMem[GetMemIndex(n)]|=GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	pMem[GetMemIndex(n)]&=(~GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  return pMem[GetMemIndex(n)]& GetMemMask(n);;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (pMem!=bf.pMem)
	{
		BitLen=bf.BitLen;
		if(MemLen!=bf.MemLen)
		{
			delete []pMem;
			MemLen=bf.MemLen;
			pMem=new TELEM[MemLen];
		}
		memcpy(pMem,bf.pMem,sizeof(TELEM)*MemLen);
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int i=MemLen;
	int sv=1;
	if (BitLen==bf.BitLen)
	{
		while (( i!=0)&&( sv!=0))
		{
			if (pMem[i]!=bf.pMem[i])
				sv=0;
			i-=1;
		}
	}
	else sv=0;
  return sv;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return ~(*this==bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{ 
	TBitField IDK(1);
	if (BitLen>bf.BitLen)
	{
		IDK=*this;
		for (int i=0;i<bf.MemLen;i++)
			pMem[i]|=bf.pMem[i];
	}
		else
	{ 
		IDK=bf;
		for (int i=0;i<MemLen;i++)
			IDK.pMem[i]|=pMem[i];
	}
	return IDK;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	TBitField IDK(1);
	if (BitLen>bf.BitLen)
	{
		 IDK=*this;
		for (int i=0;i<bf.MemLen;i++)
			IDK.pMem[i]&=bf.pMem[i];
	}
	else
	{ 
		IDK=bf;
		for (int i=0;i<MemLen;i++)
		IDK.pMem[i]&=pMem[i];
	}
	return IDK;
}

TBitField TBitField::operator~(void) // отрицание
{
	for (int i=0;i<MemLen;i++)
		pMem[i]=~pMem[i];
	return *this;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	bool bit;
	for (int i=0;i<bf.BitLen;i++)
	{
		while ((bit!=0)&&(bit!=1))
			istr>>bit;
		if (bit!=0)
			bf.SetBit(i);
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i=0;i<bf.BitLen;i++)
		ostr << bf.GetBit(i);
	return ostr;
}
