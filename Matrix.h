#pragma once

#include "vector"

/*
Generic matrix.
*/
template<typename T>
class Matrix
{
public:

	/*
	Parametrized constructor.
	*/
	Matrix(
		const unsigned int rowCount,
		const unsigned int columnCount
		)
	{
		data = std::vector<T>(rowCount * columnCount);
		this->rowCount = rowCount;
		this->columnCount = columnCount;
	}

	/*
	Returns value at specified position.
	*/
	T getAt(
		const unsigned int row,
		const unsigned int column
	) const
	{
		return data[row * columnCount + column];
	}

	/*
	Sets new value for item at specified position.
	*/
	void setAt(
		const unsigned int row,
		const unsigned int column,
		const T& value
	)
	{
		data[row * columnCount + column] = value;
	}

	unsigned int getRowCount() const
	{
		return rowCount;
	}

	unsigned int getColumnCount() const
	{
		return columnCount;
	}

private:
	std::vector<T> data;

	unsigned int rowCount;
	unsigned int columnCount;
};

