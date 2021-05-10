#include <iostream>
#include <set>
#include <vector> 
#include <cassert>
using namespace std;

void printboolstring(string s)
{
	for (char c : s)
	{
		cout << (char)(c+'0');
	}
	cout << endl;
}

void printformula(string formula)
{
	assert(formula.length() % 6 == 0);
	for (int i = 0; 6*i < formula.length(); i++)
	{
		cout << "(";
		for (int j = 0; j < 3; j++)
		{
			if (formula[6*i + 2*j] == '-')
				cout << "-";
			cout << formula[6*i + 2*j + 1];
			if (j < 2) 
				cout << " v ";
		}
		cout << ")";
	}
}

bool test(string formula, string assignment)
{
	assert(assignment.length() == 256);
	for (char c : assignment) 
		assert(c == 0 or c == 1);
	
	for (int i = 0; 6*i < formula.length(); i++)
	{
		int ones_in_clause = 0;
		for (int j = 0; j < 3; j++)
		{
			ones_in_clause += ( assignment[formula[6*i+2*j+1]] + (formula[6*i+2*j]=='-') )%2;
		}
		if (ones_in_clause == 0)
			return false;
	}
	return true;
}

int count_satisf_assigments(string formula, bool only_existence = false)
{
	string existsvariable = string(256, 0);
	for (int i = 0; 2*i < formula.length(); i++)
	{
		existsvariable[formula[2*i+1]] = 1;
	}
	vector<char> variables; // important to keep it sorted
	for (int i = 0; i < 256; i++)
	{
		if (existsvariable[i] == 1)
			variables.push_back(i);
	}
	
	long long ans = 0;
	string assign1 = string(variables.size(), 0);
	bool overflow = false;
	while (overflow == false)
	{
		string assign2 = string(256, 0);
		for (int i = 0; i < assign1.length(); i++)
		{
			assign2[variables[i]] = assign1[i];
		}
		
		if (test(formula, assign2) == true)
		{
			ans++;
			if (only_existence == true)
				return ans;
		}
		
		bool last = (assign1 == string(3, 1));
		assign1[0]++;
		int j = 0;
		while (assign1[j] == 2)
		{
			assign1[j] = 0;
			if (j+1 < assign1.length())
				assign1[j+1] += 1;
			else
				overflow = true;
			j++;
		}
	}
	return ans; 
}

string planted_sat(int var_n, int cl_n, char starting = 0)
{
	string formula = "";
	formula.reserve(6*cl_n);
	
	assert(starting - 1 + var_n <= 255);
	
	for (int i = 0; i < cl_n; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			formula += (rand()%2 ? '+' : '-');
			formula += (char)(rand()%var_n + starting);
		}
		
		if (count_satisf_assigments(formula, true) == 0)
		{
			formula.erase(formula.length()-6);
			i--;
		}
	}
	
	return formula;
}

int main()
{
	/*
	string form;
	cin >> form;
	
	srand(0);
	
	assert(form.length() % 6 == 0);
	int i=0;
	for (char c : form)
	{
		if (i == 0)
			assert(c == '+' or c == '-');
		i = not i;
	}
	
	cout << count_satisf_assigments(form);
	*/
	
	while(true)
	{
		string f = planted_sat(4, 4, 'a');
		int cnt = count_satisf_assigments(f);
		assert(cnt > 0);
		if (cnt == 1)
		{
			printformula(f);
			break;
		}
	}
}