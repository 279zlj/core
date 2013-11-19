#pragma once
#ifndef UTILITY_UNIVERSAL_UNIT_INCLUDE_H_
#define UTILITY_UNIVERSAL_UNIT_INCLUDE_H_

#include <string>
#include "property.h"
#include "setter.h"


class UniversalUnit
{
public:
	enum UnitType {Emu, Cm, Mm, Inch, Pt, Percent, Multi, Dx};

public:
	UniversalUnit();
	UniversalUnit(const int value);
	UniversalUnit(const long value);
	UniversalUnit(const size_t value);
	UniversalUnit(const double value);

	explicit UniversalUnit(const std::string& value);
	UniversalUnit(const char* value);
	UniversalUnit(const UniversalUnit& rhs);
	UniversalUnit(const UnitType type);
	UniversalUnit(const long value, const UnitType type);

	template<typename T> const UniversalUnit& operator =(const T value) {toEmu(value); return *this;}
	const UniversalUnit& operator =(const std::string& value);
	const UniversalUnit& operator =(const char* value);
	const UniversalUnit& operator =(const UniversalUnit& rhs);

public:
	operator const double() const;

public:
	const bool operator ==(const UniversalUnit& rhs) const;
	const bool operator !=(const UniversalUnit& rhs) const;
	const bool operator > (const UniversalUnit& rhs) const;
	const bool operator >=(const UniversalUnit& rhs) const;
	const bool operator < (const UniversalUnit& rhs) const;
	const bool operator <=(const UniversalUnit& rhs) const;

	template<typename T> const bool operator ==(const T value) const {return m_value == toEmu(value, Type);}
	template<typename T> const bool operator !=(const T value) const {return m_value != toEmu(value, Type);}
	template<typename T> const bool operator > (const T value) const {return m_value >  toEmu(value, Type);}
	template<typename T> const bool operator >=(const T value) const {return m_value >= toEmu(value, Type);}
	template<typename T> const bool operator < (const T value) const {return m_value <  toEmu(value, Type);}
	template<typename T> const bool operator <=(const T value) const {return m_value <= toEmu(value, Type);}

public:
	const UniversalUnit operator -() const;
	const UniversalUnit& operator +=(const UniversalUnit& rhs);
	const UniversalUnit& operator -=(const UniversalUnit& rhs);
	template<typename T> const UniversalUnit& operator +=(const T value) {m_value += toEmu(value, Type); return *this;}
	template<typename T> const UniversalUnit& operator -=(const T value) {m_value -= toEmu(value, Type); return *this;}
	template<typename T> const UniversalUnit& operator *=(const T value) {m_value *= value; return *this;}
	template<typename T> const UniversalUnit& operator /=(const T value) {m_value /= value; return *this;}

	const UniversalUnit operator +(const UniversalUnit& rhs) const;
	const UniversalUnit operator -(const UniversalUnit& rhs) const;
	template<typename T> const UniversalUnit operator +(const T rhs) const;
	template<typename T> const UniversalUnit operator -(const T rhs) const;
	template<typename T> const UniversalUnit operator *(const T rhs) const;
	template<typename T> const UniversalUnit operator /(const T rhs) const;

	friend const UniversalUnit operator +(const double lhs, const UniversalUnit& rhs);
	friend const UniversalUnit operator -(const double lhs, const UniversalUnit& rhs);
	friend const UniversalUnit operator *(const double lhs, const UniversalUnit& rhs);
	friend const UniversalUnit operator /(const double lhs, const UniversalUnit& rhs);

	friend const UniversalUnit operator +(const int lhs, const UniversalUnit& rhs);
	friend const UniversalUnit operator -(const int lhs, const UniversalUnit& rhs);
	friend const UniversalUnit operator *(const int lhs, const UniversalUnit& rhs);
	friend const UniversalUnit operator /(const int lhs, const UniversalUnit& rhs);

	friend const UniversalUnit operator +(const long lhs, const UniversalUnit& rhs);
	friend const UniversalUnit operator -(const long lhs, const UniversalUnit& rhs);
	friend const UniversalUnit operator *(const long lhs, const UniversalUnit& rhs);
	friend const UniversalUnit operator /(const long lhs, const UniversalUnit& rhs);

public:
	void apply(const UniversalUnit& unit);
	const double value(const UnitType& type) const;

public:
	const std::string ToString() const;

public:
	property<int, setter::between<int, 0, 10> >	Precesion;
	property<UnitType>													Type;

private:
	long	m_value;
	
private:
	static const long toEmu(const double value, const UnitType type);
	static const double fromEmu(const long value, const UnitType type);

	void toEmu(const double value);
	const double fromEmu() const;

private:
	void fromString(const std::string& str);

private:

#pragma warning(disable:4244)
	static const long EmuinMm = 72 / 2 * 1000;
	static const long EmuinCm = 72 / 2 * 1000 * 10;
	static const long EmuinPt = 1000 * 25.4 / 2;
	static const long EmuinInch = 72 / 2 * 1000 * 25.4;
	static const long EmuinDx = 1000 / 4 * 2.54;
	static const long PercentforWrite = 100;
#pragma warning(default:4244)

	static const double MminEmu;
	static const double CminEmu;
	static const double PtinEmu;
	static const double InchinEmu;
	static const double DxinEmu;
	static const double PercentforRead;
};


const UniversalUnit operator +(const double lhs, const UniversalUnit& rhs);
const UniversalUnit operator -(const double lhs, const UniversalUnit& rhs);
const UniversalUnit operator *(const double lhs, const UniversalUnit& rhs);
const UniversalUnit operator /(const double lhs, const UniversalUnit& rhs);

const UniversalUnit operator +(const int lhs, const UniversalUnit& rhs);
const UniversalUnit operator -(const int lhs, const UniversalUnit& rhs);
const UniversalUnit operator *(const int lhs, const UniversalUnit& rhs);
const UniversalUnit operator /(const int lhs, const UniversalUnit& rhs);

const UniversalUnit operator +(const long lhs, const UniversalUnit& rhs);
const UniversalUnit operator -(const long lhs, const UniversalUnit& rhs);
const UniversalUnit operator *(const long lhs, const UniversalUnit& rhs);
const UniversalUnit operator /(const long lhs, const UniversalUnit& rhs);


template<typename T> const UniversalUnit UniversalUnit::operator +(const T rhs) const
{
	UniversalUnit unit(m_value + UniversalUnit::toEmu(rhs, Type));
	unit.Type = Type;
	return unit;
}

template<typename T> const UniversalUnit UniversalUnit::operator -(const T rhs) const
{
	UniversalUnit unit(m_value - UniversalUnit::toEmu(rhs, Type));
	unit.Type = Type;
	return unit;
}

template<typename T> const UniversalUnit UniversalUnit::operator *(const T rhs) const
{
	UniversalUnit unit(m_value * rhs);
	unit.Type = Type;
	return unit;
}

template<typename T> const UniversalUnit UniversalUnit::operator /(const T rhs) const
{
	UniversalUnit unit(m_value / rhs);
	unit.Type = Type;
	return unit;
}
/*
template<typename T> const UniversalUnit operator +(const T lhs, const UniversalUnit& rhs)
{
	UniversalUnit unit(UniversalUnit::toEmu(lsh, rhs.Type) + rhs.m_value);
	unit.Type = rhs.Type;
	return unit;
}

template<typename T> const UniversalUnit operator -(const T lhs, const UniversalUnit& rhs)
{
	UniversalUnit unit(UniversalUnit::toEmu(lsh, rhs.Type) - rhs.m_value);
	unit.Type = rhs.Type;
	return unit;
}

template<typename T> const UniversalUnit operator *(const T lhs, const UniversalUnit& rhs)
{
	UniversalUnit unit(lhs * rhs.m_value);
	unit.Type = rhs.Type;
	return unit;
}

template<typename T> const UniversalUnit operator /(const T lhs, const UniversalUnit& rhs)
{
	UniversalUnit unit(lhs / rhs.m_value);
	unit.Type = rhs.Type;
	return unit;
}
*/
#endif // UTILITY_UNIVERSAL_UNIT_INCLUDE_H_