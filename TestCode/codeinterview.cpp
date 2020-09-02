#include "pch.h"
#include <vector>
using std::vector;
namespace IO {

}



namespace MinNumProblem {
	
	struct Pos
	{
		int col;
		int row;
	};

	int compare(int val0,double val1, double tolrance= 0.000000001)
	{
		if ((val1 - val0 <= tolrance) || (val0 - val1 <= tolrance))
		{
			return 0;
		}
		else if (val0 - val1 > tolrance)
		{
			return 1;
		}
		else if (val1 - val0 > tolrance)
		{
		return -1;
		}
	}
	
	int MinNumProblem(int val,double** ppVal,int row,int col,vector<Pos>& vecPos)
	{
		if (ppVal == nullptr || row <= 0 || col <= 0)
		{
			return 0;
		}
		Pos pos;
		int maxCol = 0;
		int maxRow = 0;
		for (int j = 0; j < col-1; j++)
		{
			if (compare(val, ppVal[0][j]) < 0)
			{
				continue;
			}
			if (compare(val, ppVal[0][j]) == 0)
			{
				if ((compare(val, ppVal[0][j]) > 0))
				{
					pos.col = j;
					pos.row = 0;
					vecPos.push_back(pos);
					break;
				}
				else
				{
					continue;
				}

			}
			else if (compare(val, ppVal[0][j]) < 0)
			{

			}

		}
		for (int  i = 0; i < row; i++)
		{
			if (compare(val, ppVal[0][maxCol]) < 0)
			{
				maxCol = j;
			}
		}
	}

}