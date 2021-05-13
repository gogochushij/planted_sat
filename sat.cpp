#include <iostream>
#include <set>
#include <vector> 
#include <cassert>
#include <algorithm>
#include <fstream>
using namespace std;

void printboolstring(string s)
{
	for (char c : s)
	{
		cout << (char)(c+'0');
	}
	cout << endl;
}

long long factorial (int n)
{
	long long res = 1;
	for (int i=1; i<=n; i++)
	{
		res *= i;
	}
	return res;
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

vector<char> all_variables(string formula)
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
	
	return variables;
}

pair<long long, string> count_satisf_assigments(string formula, long long countupto = -1)
{
	vector<char> variables = all_variables(formula); // important to keep it sorted

	
	long long ans = 0;
	string assign1 = string(variables.size(), 0);
	string sat_assign2 = string(256, 0);
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
			sat_assign2 = assign2;
			if (countupto > -1 and ans == countupto)
				return {ans, sat_assign2};
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
	if (ans == 0)
		return {ans, {}};
	else
		return {ans, sat_assign2}; 
}

long long random_choices (string formula, string satisfying_assignment)
{
	long long ans = 0;
	vector<char> perm = all_variables(formula);
	do
	{
		vector<int> values (256, -1);
		for (char v : perm)
		{
			bool propagation_made = false;
			bool propagation_made_on_this_iteration = true;
			while (propagation_made_on_this_iteration == true)
			{
				propagation_made_on_this_iteration = false;
				
				for (int i = 0; 6*i < formula.length(); i++)
				{
					// if exists 1 in clause
					bool clause_satisfied = false;
					for (int j = 0; j < 3; j++)
					{
						if (values[formula[6*i+2*j+1]] != -1 and 
							((values[formula[6*i+2*j+1]]==1) + (formula[6*i+2*j] == '-'))%2 == 1)
							clause_satisfied = true;
					}
					if (clause_satisfied == true) continue;
					
					int number_of_0_in_clause = 0, j_novalue = -1;
					for (int j = 0; j < 3; j++)
					{
						if (values[formula[6*i+2*j+1]] == -1)
							j_novalue = j;
						else
							if (((values[formula[6*i+2*j+1]] == 1) + (formula[6*i+2*j] == '-'))%2 == 0)
								number_of_0_in_clause ++;
					}
					
					assert(number_of_0_in_clause < 3);
					if (number_of_0_in_clause == 2)
					{
						propagation_made = true;
						propagation_made_on_this_iteration = true;
						values[formula[6*i+2*j_novalue+1]] = satisfying_assignment[formula[6*i+2*j_novalue+1]];
					}
				}
			}
			
			if (values[v] == -1)
			{
				ans++;
				values[v] = satisfying_assignment[v];
			}
		}
	} while (next_permutation(perm.begin(), perm.end()));
	
	return ans;
}
/*
long long random_choices(string formula, string satisfying_assignment)
{
	enum var_value {no, zero, one};
	
	//vector<vector<string>> cur_evaluation (clauses.size(), {no, no, no});
	
	
	long long random_choices = 0;
	vector<char> perm = all_variables(formula);
	do
	{
		string values(256, no);
		int random_choices_by_perm = 0;
	
		for (char v : perm)
		{
			bool propagation_made = true;
			while (propagation_made == true)
			{
				propagation_made = false;
				
				for (int i = 0; 6*i < formula.length(); i++)
				{
					bool clause_satisfied = false;
					for (int j = 0; j < 3; j++)
					{
						if ((values[formula[6*i+2*j+1]] != no) and (( (values[formula[6*i+2*j+1]] == one) + (formula[6*i+2*j]=='-') ) % 2 == true))
						{
							clause_satisfied = true;
						}
					}
					if (clause_satisfied == true) continue;
					
					if (v = perm.back())
					{
						cout << perm.back() << endl;
						cout << "Unsat clause: ";
						for (int k=6*i; k < 6*i+6; k++) 
							cout << formula[k];
						cout << endl;
					}
					vector<vector<int>> V = {{0,1,2},{1,0,2},{2,0,1}};
					for (vector<int> o : V)
					{
						if (values[formula[6*i+2*o[0]+1]] == no and
							values[formula[6*i+2*o[1]+1]] != no and
							values[formula[6*i+2*o[2]+1]] != no and
						((values[formula[6*i+2*o[1]+1]]==one)+(formula[6*i+2*o[1]]=='-'))%2 == false and
						((values[formula[6*i+2*o[2]+1]]==one)+(formula[6*i+2*o[2]]=='-'))%2 == false)
						{
							propagation_made = true;
							values[formula[6*i+2*o[0]+1]] = (satisfying_assignment[formula[6*i+2*o[0]+1]] ? one : zero);
							break;
						}
					}
				}
			}
			if (values[v] == no)
			{
				values[v] = satisfying_assignment[v];
				random_choices_by_perm++;
			}
		}	
		
		for (char var : perm)
		{
			cout << var;
		}
		cout << endl << random_choices_by_perm << endl;
		random_choices += random_choices_by_perm;
		
	} while ( next_permutation(perm.begin(), perm.end()) );
	
	return random_choices;
}
*/

pair<string,string> generate_singlesat_formula(int var_n, char starting = 0)
{
	string formula = "";
	formula.reserve(6*var_n);
	
	assert(starting - 1 + var_n <= 255);
	
	pair<long long, string> cnt = {2, string(256, 0)};
	while (cnt.first > 1)
	{
		for (int j = 0; j < 3; j++)
		{
			formula += (rand()%2 ? '+' : '-');
			char var;
			do
			{
				var = (rand()%var_n + starting);
			}
			while ((j==1 and var == *(formula.end()-2)) or (j==2 and (var == *(formula.end()-2) or var == *(formula.end()-4))));
			formula += var;
			
		}
		
		cnt = count_satisf_assigments(formula);
		
		if (cnt.first == 0)
		{
			formula.erase(formula.length()-6);
			cnt = count_satisf_assigments(formula);
		}
	}
	
	return {formula, cnt.second};
}

int main()
{
	// +a+b+c+a+b-c+a-b+c+a-b-c-a+b+c-a+b-c-a-b+c
	// string form = "+a+b+c+a+b-c+a-b+c+a-b-c-a+b+c-a+b-c-a-b+c";
	// cin >> form;
	
	/*
	assert(form.length() % 6 == 0);
	int i=0;
	for (char c : form)
	{
		if (i == 0)
			assert(c == '+' or c == '-');
		i = not i;
	}
	
	cout << random_choices(form, string(256, 1));
	*/
	
	
	int n = 9;
	long long ideal = 2*n*factorial(n)/3;
	
	set<pair<long long, string>> records;
	
	long long form_cnter = 0;
	for (int form_cnter = 0; 1; form_cnter++)
	{
		pair<string, string> formwithsat = generate_singlesat_formula(n, 'a');

		string form = formwithsat.first, satisfying_assignment = formwithsat.second;
		
		pair<long long,string> p = count_satisf_assigments(form, 2);
		assert(p.first == 1);
		
		long long r = random_choices(form, satisfying_assignment);
		records.insert({r, form});
		while (records.size() > 10)
		{
			records.erase(records.begin());
		}
		
				
		if (ideal - r < 0)
		{
			cout << "HUH" << endl;
			printformula(form);
			cout << ideal << " " << r << endl;
		}
		
		if (form_cnter % 1 == 0)
		{
			ofstream save ("records.txt");
			for (pair<long long, string> rec : records)
			{
				save << rec.first << endl << rec.second << endl << endl;
			}
			save.close();
			cout << "saved " << form_cnter << endl;
		}
	}
	
	for (pair<long long, string> rec : records)
	{
		cout << rec.first << endl << rec.second << endl << endl;
	}
	
	
	
	
}