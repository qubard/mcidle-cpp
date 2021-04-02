#pragma once

#include <common/Typedef.hpp>

namespace mcidle
{

template <typename T> class Var
{
  public:
	Var() : m_Value(static_cast<T>(0)) {}

	Var(T value) : m_Value(value) {}

	void SetValue(T value) { m_Value = value; }

	T Value() const { return m_Value; }

	bool operator==(const T value) const { return m_Value == value; }

  protected:
	T m_Value;
};

}  // namespace mcidle
