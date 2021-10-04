// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len > 0)
	{
		BitLen = len;
		MemLen = (BitLen - 1 / 32) + 1; // единица не нужна????
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++)
			pMem[i] = 0;
	}
	else throw len;
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n < 0 || n >= BitLen)
		throw n;
	else
		return n / 32;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n < 0 || n >= BitLen)
		throw n;
	else
		return 1 << (n % 32);

	//TELEM mask = 1;
	//return mask << n;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0 || n >= BitLen)
		throw n;
	else
	{
		int Ind = GetMemIndex(n);
		TELEM mask = GetMemMask(n);  
		pMem[Ind] = pMem[Ind] | mask;
	}

}


void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0 || n >= BitLen)
		throw n;
	else
	{
		int Ind = GetMemIndex(n);
		TELEM mask = ~GetMemMask(n);
		pMem[Ind] = pMem[Ind] & mask;
	}

}


int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0 || n >= BitLen)
		throw n;
	else 
	{
		int Ind = GetMemIndex(n);
		TELEM mask = GetMemMask(n);
		return pMem[Ind] & mask;
	}
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	if (&bf == this)
	{
		return *this;
	}
	BitLen = bf.BitLen;
	if (MemLen != bf.MemLen)
	{
		MemLen = bf.MemLen;
		delete[] pMem;
		pMem = new TELEM[MemLen];
	}
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	if ((BitLen == bf.BitLen) && (MemLen == bf.MemLen))
	{
		int flag = 1;
		for (int i = 0; i < MemLen; i++)
		{

			if (pMem[i] == bf.pMem[i])
			{
				flag = 1;
			}
			else
			{
				flag = 0;
				break;
			}
			return flag;
		}
	}
	else
	{
		return 0;
	}
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	if ((BitLen == bf.BitLen) && (MemLen == bf.BitLen))
	{
		int flag = 0;
		for (int i = 0; i < MemLen; i++)
		{
			if (pMem[i] == bf.pMem[i])
			{
				flag = 0;
			}
			else
			{
				flag = 1;
				break;
			}
			return flag;
		}
	}
	else
	{
		return 1;
	}
}


TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	int l = BitLen;
	if (BitLen < bf.BitLen)
		l = bf.BitLen;
	TBitField Field(l);
	for (int i = 0; i < Field.MemLen; i++)
		Field.pMem[i] = pMem[i] | bf.pMem[i];
	return Field;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	int l = BitLen;
	if (BitLen < bf.BitLen)
		l = bf.BitLen;

	TBitField res(l);
	for (int i = 0; i < res.MemLen; i++)
		res.pMem[i] = pMem[i] & bf.pMem[i];
	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(*this);
	for (int i = 0; i < MemLen-1; i++)
		res.pMem[i] = ~res.pMem[i];
	if (MemLen > 0)
	{
		for (int i = (MemLen - 1) * 32; i < res.BitLen; i++)
		{ 
			if (res.GetBit(i))
				res.ClrBit(i);
			else
				res.SetBit(i);
		}
	}
	return res;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	int i = 0;
	char ch;
	do { istr >> ch; } while (ch != ' ');
	while (1)
	{
		istr >> ch;
		if (ch == '0')
			bf.ClrBit(i++);
		else if (ch == '1')
			bf.SetBit(i++);
		else break;
	}
	return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
		if (bf.GetBit(i)) ostr << 1 << " ";
		else ostr << 0 << " ";
	return ostr;
}