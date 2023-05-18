#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
#include<vector>
#include<unordered_map>
#include<sstream>
#include"header.h"
//var的平均值:0.00132097   看每个点出发的边之间是否存在支配关系
//再一跳能到达destination的点有1635个
using namespace std;
using ld = long double;
extern int des = 105;
extern int jump_limit = 500;
extern ld window = 1;
extern int maxnode = 10000;

int main()
{
	vector<vector<int>> childs(10001, vector<int>());
	vector<vector<int>> origins(12, vector<int>());
	vector<vector<ld>> means(10001, vector<ld>(10001, -1)), vars(10001, vector<ld>(10001, -1));
	vector<vector<int>> solution(12, vector<int>());
	vector<vector<ld>> visited(10001, vector<ld>());          //记录每个点被访问的时刻
	vector<vector<int>> shortest_path(10001, vector<int>());
	vector<ld> shortest_mean(10001), shortest_var(10001);
	vector<ld> ini_delays(12);
	vector<vector<bool>> dominated(10001, vector<bool>(10001, false));
	read_edge_and_meanvar(means, vars, childs);
	read_origin(origins);
	read_ini_delays(ini_delays);
	read_shortest(shortest_path, shortest_mean, shortest_var);
	read_ini_solution(solution, visited, ini_delays, means);
	//check_domi(childs, means, vars, dominated);
	deal_with_conflict(solution, visited, means, vars, ini_delays, shortest_mean, shortest_var, shortest_path, origins, childs);
	check_solution(solution, visited, means, vars, ini_delays);
	write_solution(solution);
	//write_solution(solution);
	//get_mean_andvar(childs, means, vars);
	
	return 0;
}