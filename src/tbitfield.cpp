// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if(len<0)
		throw "Negative_lenght";
	else
	{
		BitLen=len;
		MemLen=(len+8*sizeof(TELEM)-1)/(8*sizeof(TELEM));
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
	if(n<0)
		throw "Negative_index";
	else if(n>=BitLen)
		throw "Large_index";
	else
	return n/(8*sizeof(TELEM));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if(n<0)
		throw "Negative_index";
	else if(n>=BitLen)
		throw "Large_index";
	else
	return 1<<(n%(8*sizeof(TELEM)));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if(n<0)
		throw "Negative_index";
	else if(n>=BitLen)
		throw "Large_index";
	else
		pMem[GetMemIndex(n)]|=GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if(n<0)
		throw "Negative_index";
	else if(n>=BitLen)
		throw "Large_index";
	else
		pMem[GetMemIndex(n)]&=(~GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	int j;
	if(n<0)
		throw "Negative_index";
	else if(n>=BitLen)
		throw "Large_index";
	else
		if( (pMem[GetMemIndex(n)]&GetMemMask(n))==0)
			j=0;
		else j=1;
	return j;
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
	int i;
	int sv=1;
	if (MemLen<bf.MemLen)
	{
		i=bf.MemLen;
		while ((i-MemLen!=0)&&( sv!=0))
		{
			if (bf.pMem[i-1]!=0)
				sv=0;
			i-=1;
		}
	}
	else
	{
		i=MemLen;
		while (((i-bf.MemLen)!=0)&&( sv!=0))
		{
			if (bf.pMem[i-1]!=0)
				sv=0;
			i-=1;
		}

	}
	while (( i!=0)&&( sv!=0))
		{
			if ((pMem[i-1])!=(bf.pMem[i-1]))
				sv=0;
			i-=1;
		}
  return sv;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return 1-(*this==bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{ 
	TBitField IDK(1);
	if (BitLen>=bf.BitLen)
	{
		IDK=*this;
		for (int i=0;i<bf.MemLen;i++)
			IDK.pMem[i]|=bf.pMem[i];
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
	if (BitLen>=bf.BitLen)
	{
		IDK=bf;
		for (int i=0;i<MemLen;i++)
		IDK.pMem[i]&=pMem[i];
	}
	else
	{ 
		IDK=*this;
		for (int i=0;i<bf.MemLen;i++)
			IDK.pMem[i]&=bf.pMem[i];
	}
	return IDK;
}


TBitField TBitField::operator~(void) // отрицание
{
	TBitField IDK(*this);
	for (int i=0;i<MemLen-1;i++)
		IDK.pMem[i]=(~pMem[i]);
	for (int i=8*(MemLen-1)*sizeof(TELEM);i<BitLen;i++)
	{
		if (this->GetBit(i)==0)
			IDK.SetBit(i);
		else IDK.ClrBit(i);
	}
	return IDK;
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
		ostr << bf.GetBit(bf.BitLen-i-1);
	return ostr;
}
