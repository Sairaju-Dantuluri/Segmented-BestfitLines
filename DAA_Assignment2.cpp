#include <bits/stdc++.h>
#include <chrono>
#include <iostream>
#include <fstream>
using namespace std;
using namespace std::chrono;

class Point
{
public:
	double x;
	double y;

	Point(double xp, double yp)
	{
		x = xp;
		y = yp;
	}

	bool operator<(const Point &p) const
	{
		if (x != p.x)
			return x < p.x;
		else
			return y < p.y;
	}
};

//Calculates a,b values
vector<double> calWeights(vector<Point> &P, int i, int j)
{
	double xSum = 0, ySum = 0, xySum = 0, x2Sum = 0;
	double a, b;

	for (int k = i; k <= j; k++)
	{
		xSum += P[k].x;
		ySum += P[k].y;
		xySum += (P[k].x * P[k].y);
		x2Sum += (P[k].x * P[k].x);
	}
	int n = j - i + 1;

	double c = ((n * xySum) - (xSum * ySum));
	double d = ((n * x2Sum) - (xSum * xSum));
	if (d != 0)
		a = (double)(c / d);
	else
		a = 0;

	b = (double)((ySum - (a * xSum)) / n);

	vector<double> W;
	W.push_back(a);
	W.push_back(b);
	W.push_back(d);

	return W;
}

//Calculate SSE for the given set of points
double calError(vector<Point> &P, int i, int j)
{
	vector<double> W = calWeights(P, i, j);

	if (W[2] == 0) //when d=0,means points have same x values and error will be zero in that case
	{
		return 0;
	}

	double err = 0;

	for (int k = i; k <= j; k++)
	{
		err += ((P[k].y - (W[0] * P[k].x + W[1])) * (P[k].y - (W[0] * P[k].x + W[1])));
	}

	return err;
}

double segmentedLeastSquares(vector<Point> &P, int n, vector<vector<double>> &err, double C, vector<double> &OPT, vector<int> &ind)
{
	for (int j = 0; j < n; j++) //Computing least squre error for all possible i,j values
	{
		for (int i = 0; i <= j; i++)
		{
			if (i != j)
				err[j][i] = calError(P, i, j);
			else
				err[j][i] = 0;
		}
	}

	OPT[0] = 0;
	for (int j = 0; j < n; j++)
	{
		double arr[j + 1];
		for (int i = 0; i <= j; i++)
		{
			if (i > 0)
			{
				arr[i] = err[j][i] + C + OPT[i - 1];
			}
			else
			{
				arr[i] = err[j][i] + C;
			}
		}
		int index = min_element(arr, arr + j + 1) - arr;
		OPT[j] = arr[index]; // finds the minimum opt value

		ind.push_back(index); //Finds the index of minimum opt value
	}

	return OPT[OPT.size() - 1];
}

//Finds the segments
void findSegment(vector<int> &ind, vector<int> &bp, int index)
{
	if (index == 0)
	{
		bp.push_back(index);
		return;
	}
	if (index < 0)
		return;
	bp.push_back(ind[index]);

	findSegment(ind, bp, ind[index] - 1);
}

int main()
{
	double C;
	printf("Enter C value : ");
	scanf("%le", &C);
	int n;
	printf("No. of points : ");
	scanf("%d", &n);
	double l, m;
	vector<Point> points;
	vector<double> size;
	vector<vector<double>> err(n);
	for (int i = 0; i < n; i++)
	{
		err[i] = vector<double>(i + 1, -1);
	}
	vector<double> OPT(n, -1);
	vector<int> ind;

	fstream input_file;
	input_file.open("points.txt", ios::out); //writing input values into a text file for visualisation purpose
	if (!input_file)
	{
		cout << "input file not created\n";
		return 0;
	}

	for (int i = 0; i < n; i++)
	{
		scanf("%le %le", &l, &m);
		input_file << l << "," << m << "\n";
		Point p(l, m);
		points.push_back(p);
	}
	input_file.close();
	auto start = high_resolution_clock::now();
	sort(points.begin(), points.end()); //Making sure that input points are in sorted order
	double error = segmentedLeastSquares(points, n, err, C, OPT, ind);

	vector<int> breakpoints;
	breakpoints.push_back(n - 1);
	findSegment(ind, breakpoints, n - 2);
	auto stop = high_resolution_clock::now();

	cout << "\nfinal penalty : " << error << "\n";
	fstream lines;
	lines.open("ab.txt", ios::out); //writing a,b values into a text file for visualisation purpose
	if (!lines)
	{
		cout << "input file not created\n";
		return 0;
	}
	lines << error << "\n";
	lines << C << "\n";
	//Finding the intervals of each segment
	for (int i = 0; i < breakpoints.size() - 1; i++)
	{
		vector<double> line;
		line = calWeights(points, breakpoints[i + 1], breakpoints[i]);
		lines << points[breakpoints[i + 1]].x << "," << points[breakpoints[i]].x << "," << line[0] << "," << line[1] << "\n";
	}
	lines.close();
	auto duration = duration_cast<milliseconds>(stop - start);
	cout << duration.count() << "ms" << endl
		 << "\n";
	return 0;
}
