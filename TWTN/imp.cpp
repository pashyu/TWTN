#include"header.h"
#include<fstream>
#include<string>
#include<sstream>
#include<iomanip>
#include<iostream>
#include<filesystem>
#include<random>
void read_edge_and_meanvar(vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<vector<int>>& neighbors)
{
	ld mean, var;
	ifstream infile_meanvar("..\\data\\meanvar.txt"), infile_edge("..\\data\\edges.txt");
	string linestr_meanvar, linestr_edge;
	int cnt = 0;
	while (infile_meanvar.good())
	{
		getline(infile_meanvar, linestr_meanvar);
		getline(infile_edge, linestr_edge);

		int pos = linestr_meanvar.find(" ");
		mean = stold(linestr_meanvar.substr(0, pos));
		var = stold(linestr_meanvar.substr(pos + 1));
		pos = linestr_edge.find(" ");
		if (var < 1e-4)
			++cnt;
		int node1 = stoi(linestr_edge.substr(0, pos));
		int node2 = stoi(linestr_edge.substr(pos + 1));
		neighbors[node1].emplace_back(node2);
		means[node1][node2] = mean;
		vars[node1][node2] = var;

	}
	//cout << cnt << endl;
	infile_meanvar.close();
	infile_edge.close();
}

void read_origin(vector<vector<int>>& origins)
{
	ifstream infile("..\\data\\origins.txt");
	string linestr;
	//每个飞船有14个候选起点
	for (int i = 0; i < 12; ++i)
	{
		getline(infile, linestr);

		int pos = 0, nextpos = -1;
		while ((nextpos = linestr.find(" ", pos)) != -1)
		{
			int node = stoi(linestr.substr(pos, (unsigned)nextpos - pos));
			pos = nextpos + 1;
			origins[i].emplace_back(node);
		}
		int node = stoi(linestr.substr(pos));
		origins[i].emplace_back(node);
	}

	infile.close();
}

void read_edge(vector<vector<int>>& neighbors)
{
	ifstream infile("../data/edges.txt");

	string linestr;
	while (infile.good())
	{
		getline(infile, linestr);

		int pos = linestr.find(" ");

		int node1 = stoi(linestr.substr(0, pos));
		int node2 = stoi(linestr.substr(pos + 1));
		neighbors[node1].emplace_back(node2);
	}
	infile.close();
}


void read_ini_delays(vector<ld>& ini_delays)
{
	ifstream infile("..\\data\\delays.txt");

	for (int i = 0; i < 12; ++i)
	{
		string line;
		getline(infile, line);

		ini_delays[i] = stold(line);
	}



	infile.close();
}

void read_shortest(vector<vector<int>>& shortest_path, vector<ld>& shortest_mean, vector<ld>& shortest_var)
{
	ifstream infile("..\\data\\shortest_path.txt");
	string linestr;
	for (int node = 1; node <= 10000; ++node)
	{
		getline(infile, linestr);
		int pos1 = linestr.find(" ");
		int pos2 = pos1 + 1;
		pos2 = linestr.find(" ", pos2);
		shortest_mean[node] = stold(linestr.substr(pos1 + 1, pos2 - pos1 - 1));

		shortest_var[node] = stold(linestr.substr(pos2 + 1));

		int c_node = 0;

		while (c_node != 105)
		{
			infile >> c_node;
			shortest_path[node].emplace_back(c_node);
		}
		shortest_path[node].emplace_back(105);
		getline(infile, linestr);

	}
	infile.close();
}


void bfs(vector<vector<int>>& childs, vector<vector<int>>& origins)
{
	//看一下从105到每个的起始点的最短跳数
	ofstream outfile("./data/min_hops.txt");

	vector<vector<int>> pre(10001, vector<int>());
	for (int i = 1; i <= 10000; ++i)
	{
		for (int j = 0; j < childs[i].size(); ++j)
		{
			int node = childs[i][j];
			pre[node].push_back(i);
		}
	}
	for (int i = 0; i < 12; ++i)
	{
		//i代表飞船
		vector<int> des, hops;
		vector<bool> visited(10001, false);
		int hop = 0;
		outfile << i << endl;
		vector<int> cur, next;
		next.push_back(105);
		while (!next.empty())
		{
			cur = next;
			next.clear();
			//检查cur
			for (auto node : cur)
			{
				if (find(origins[i].begin(), origins[i].end(), node) != origins[i].end())
				{
					//加入des
					des.push_back(node);
					hops.push_back(hop);
				}
				for (auto conn : pre[node])
				{
					if (visited[conn])
						continue;
					next.push_back(conn);
					visited[conn] = true;
				}
			}


			if (des.size() == 14)
				break;
			++hop;
		}
		for (int j = 0; j < 14; ++j)
		{
			outfile << "des:" << des[j] << " hops:" << hops[j] << endl;
		}
	}
	outfile.close();
}


void find_path(vector<vector<int>>& childs, int origin, vector<vector<ld>>& means, vector<vector<ld>>& vars, ld ini_delay, ld tw_start)
{
	ld tw_end = tw_start + window;

	//假设飞船从origin开始，初始ini_delay，尝试找到一条路，到达des的时间落在tw_start到tw_end

	//统计一个点被经过了多少次
	vector<bool> vis_cnt(maxnode + 1, false);

	int current_node = origin;
	vis_cnt[origin] = true;

	ld total_delay = ini_delay, total_var = 0;

	/*while (current_node != des)
	{
		//每次进行一跳







	}*/


}


bool is_in_time_window(ld time)
{
	return (time > -0.5 && time < 0.5);
}


void read_ini_solution(vector<vector<int>>& solution, vector<bool>& is_visited, vector<ld>& ini_delays, vector<vector<ld>>& means)
{
	ifstream infile("../data/unfeasible/273.69.txt");
	for (int i = 0; i < 12; ++i)
	{
		int node = 0;
		ld delay = ini_delays[i];
		infile >> node;
	
		is_visited[node] = true;
		solution[i].emplace_back(node);
		
		int prenode = node;
		while (node != 105)
		{
			infile >> node;

			delay += means[prenode][node];
			
			is_visited[node] = true;
			solution[i].emplace_back(node);
			
			prenode = node;
		}
	}
	infile.close();
}

void read_0_11_path(vector<vector<int>>& solution, vector<vector<ld>>& visited, vector<ld>& ini_delays, vector<vector<ld>>& means, string filename)
{
	ifstream infile(filename);
	for (int i = 0; i < 12; i+=11)
	{
		int node = 0;
		ld delay = ini_delays[i];
		infile >> node;

		visited[node].emplace_back(delay);
		solution[i].emplace_back(node);
		
		int prenode = node;
		while (node != 105)
		{
			infile >> node;

			delay += means[prenode][node];
			visited[node].emplace_back(delay);
			solution[i].emplace_back(node);
			
			prenode = node;
		}
		solution[i].emplace_back(105);
	}
	infile.close();
}

void get_mean_andvar(vector<vector<int>>& childs, vector<vector<ld>>& means, vector<vector<ld>>& vars)
{
	//从105倒着搜，记录下每个点到105的最小var，和带来的时间偏移，以及对应的路径，dijstra
	vector<int> pre(10001, -1);
	pre[105] = 105;

	vector<ld> node_var(10001, 100), node_mean(10001);
	node_var[105] = 0;
	node_mean[105] = 0;
	vector<bool> visited(10001, false);

	//child转到parent
	vector<vector<int>> parent(10001, vector<int>());

	vector<vector<ld>> p_means(10001, vector<ld>()), p_vars(10001, vector<ld>());

	for (int node = 1; node <= 10000; ++node)
	{
		for (int j = 0; j < childs[node].size(); ++j)
		{
			int child = childs[node][j];
			parent[child].emplace_back(node);

			p_means[child].emplace_back(means[node][j]);
			p_vars[child].emplace_back(vars[node][j]);
		}
	}

	int iter = 1;
	while (iter <= 10000)
	{
		//每次拓一个
		int node_choose = 0;
		ld min_var = 0;
		for (int node = 1; node <= 10000; ++node)
		{
			if (visited[node])
				continue;

			if (node_var[node] < 50)
			{
				//当前node是可达的
				if (node_choose == 0)
				{
					node_choose = node;
					min_var = node_var[node];
				}
				else if (node_var[node] < min_var)
				{
					node_choose = node;
					min_var = node_var[node];
				}
			}
		}


		//选好了node
		visited[node_choose] = true;

		for (int i = 0; i < parent[node_choose].size(); ++i)
		{
			int p_node = parent[node_choose][i];
			if (node_var[p_node] > p_vars[node_choose][i] + node_var[node_choose])
			{
				node_var[p_node] = p_vars[node_choose][i] + node_var[node_choose];
				node_mean[p_node] = p_means[node_choose][i] + node_mean[node_choose];
				pre[p_node] = node_choose;
			}
		}
		++iter;
	}

	ofstream outfile("./data/shortest_path.txt");

	for (int this_node = 1; this_node <= 10000; ++this_node)
	{
		int node = this_node;
		stringstream ss1, ss2;
		ss1 << setprecision(20) << node_mean[node];
		ss2 << setprecision(20) << node_var[node];
		outfile << node << " " << ss1.str() << " " << ss2.str() << endl;
		ss1.clear();
		ss2.clear();
		while (pre[node] != node)
		{
			outfile << node << " ";
			node = pre[node];
		}
		outfile << "105" << endl;
	}
	cout << fixed << setprecision(20) << node_mean[1] << " " << node_var[1] << endl;
	outfile.close();

}

ld cal_path_var(vector<int>& path, vector<vector<ld>>& visited, vector<vector<ld>>& means, vector<vector<ld>>& vars, ld ini_delay)
{
	ld current_delay = ini_delay;
	ld res = 0;
	for (int i = 0; i < path.size() - 1; ++i)
	{
		int node = path[i], next_node = path[i + 1];
		int k = 0;
		//查找visited,看有多少访问的时间是小于current_delay的

		for (auto visit_time : visited[node])
		{
			if (visit_time < current_delay)
				++k;
		}

		res += vars[node][next_node] * pow(10, k);



		current_delay = current_delay + means[node][next_node];
	}
	return res;
}

void gene_solution(int ship, vector<vector<int>>& childs, vector<vector<int>>& solution, vector<vector<ld>>& visited, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<ld>& shortest_mean, vector<ld>& shortest_var, vector<ld>& ini_delays, vector<vector<int>>& shortest_path, vector<vector<int>>& origins, vector<vector<bool>>& dominated)
{

	ld min_var = 500;
	vector<int> best_path;
	for (auto origin : origins[ship])
	{
		//从origin开始
		ld current_delay = ini_delays[ship];
		bool origin_repeat = false;
		//看origin有没有和别的点重复了
		for (int k = 0; k < visited[origin].size(); ++k)
		{
			ld time = visited[origin][k];
			if (time > current_delay)
			{
				origin_repeat = true;
				break;
			}
		}
		if (origin_repeat)
			continue;
		int node = origin;
		vector<int> path;
		path.emplace_back(node);


		while (node != 105)
		{
			if (is_in_time_window(current_delay + shortest_mean[node]))
			{
				//最优路径有没有重复
				int j = 1;
				int test_node = shortest_path[node][j];
				while (test_node != 105)
				{
					if (visited[test_node].size() > 0)
						break;
					if (find(path.begin(), path.end(), test_node) != path.end())
						break;
					test_node = shortest_path[node][++j];
				}

				if (test_node == 105)
				{
					//走到头了
					//把node的最优路径放进来，结束
					j = 1;
					test_node = shortest_path[node][j];

					while (test_node != 105)
					{
						path.emplace_back(test_node);
						test_node = shortest_path[node][++j];
					}

					path.emplace_back(105);
					current_delay += shortest_var[node];
					break;
				}
				else
				{
					//中间有重复
					//可能考虑允许在最优路径中间出现一定次数的重复
					//同一个点最多访问两次
				}

			}

			//普通寻路，找下一跳
			int next_node = -1;
			ld next_var = 1;
			bool visit_feasible = false;
			int visit_cnt = 0;
			ld virtual_delay = 0;
			for (auto child : childs[node])
			{
				if (next_node != -1 && dominated[node][child])
					continue;
				if (vars[node][child] > 1.8e-3)
					continue;
				if (current_delay < -0.5 && means[node][child] < 0)
					continue;
				if (current_delay > 0.5 && means[node][child] > 0)
					continue;
				//不对同一个点进行二次访问
				if (find(path.begin(), path.end(), child) != path.end())
					continue;

				int k = visited[child].size();

				bool flag = false;
				ld visit_time = current_delay + means[node][child];
				for (int t = 0; t < k; ++t)
				{
					if (visited[child][t] > visit_time)
					{
						flag = true;

						break;
					}
				}

				if (child == 105)
				{
					if (is_in_time_window(visit_time))
					{
						next_node = 105;
						break;
					}
					else
						continue;
				}
				if (flag)
					continue;
				if (next_node == -1)
				{
					next_node = child;
					next_var = vars[node][child];
					visit_cnt = k;
					virtual_delay = visit_time;
					if (is_in_time_window(visit_time))
						visit_feasible = true;
					else
						visit_feasible = false;
				}
				else if (k < visit_cnt)
				{
					//要么原本delay满足，要么修改后满足
					/*if (is_in_time_window(current_delay)  || is_in_time_window(visit_time))
					{
						next_node = child;
						next_var = vars[node][child];
						visit_cnt = k;
						virtual_delay = visit_time;
					}*/
					//if (visit_feasible && is_in_time_window(visit_time) || !visit_feasible)
					//{
					next_node = child;
					next_var = vars[node][child];
					visit_cnt = k;
					virtual_delay = visit_time;
					if (is_in_time_window(visit_time))
						visit_feasible = true;
					else
						visit_feasible = false;
					//}
				}
				else if (k == visit_cnt)
				{
					//virtual_delay是否在时间窗里
					/*if (is_in_time_window(virtual_delay))
					{
						//原本就在
						if (is_in_time_window(visit_time))
						{
							//新的也在
							//比较一下var
							if (vars[node][child] < next_var)
							{
								next_node = child;
								next_var = vars[node][child];
								visit_cnt = k;
								virtual_delay = visit_time;
							}
						}
						else
						{

						}
					}
					else
					{
						//原本不在
						if (is_in_time_window(visit_time))
						{
							//新的在
							next_node = child;
							next_var = vars[node][child];
							visit_cnt = k;
							virtual_delay = visit_time;
						}
						else
						{
							//新的也不在
							if (abs(visit_time) < abs(virtual_delay))
							{
								next_node = child;
								next_var = vars[node][child];
								visit_cnt = k;
								virtual_delay = visit_time;
							}
						}
					}*/
					if (vars[node][child] < next_var)
					{
						next_node = child;
						next_var = vars[node][child];
						visit_cnt = k;
						virtual_delay = visit_time;


						if (is_in_time_window(visit_time))
							visit_feasible = true;
						else
							visit_feasible = false;
					}
				}
				else
				{
					//k>visit_cnt
					//child的访问次数比目前拟定的下一跳还多
					/*if ((!is_in_time_window(virtual_delay)) && is_in_time_window(visit_time))
					{
						next_node = child;
						next_var = vars[node][child];
						visit_cnt = k;
						virtual_delay = visit_time;
					}*/
				}
			}





			if (next_node != -1)
			{
				path.emplace_back(next_node);
				current_delay += means[node][next_node];
				node = next_node;
			}
			else
			{
				break;
			}



		}




		//算一下path一共的var,小于min_var就替换
		if (path.back() != 105)
			continue;

		ld path_var = cal_path_var(path, visited, means, vars, ini_delays[ship]);

		if (path_var < min_var)
		{
			min_var = path_var;
			best_path = path;
		}
	}

	if (best_path.size() == 0)
	{
		cout << "can't find a path for ship " << ship << endl;
		return;
	}
	//依照best_path修改visited,best_path放入solution
	solution[ship] = best_path;
	ld current_delay = ini_delays[ship];
	for (int i = 0; i < best_path.size(); ++i)
	{
		int node = best_path[i];
		cout << node << " ";
		visited[node].emplace_back(current_delay);
		if (visited[node].size() > 1 && node != 105)
		{
			//cout << "repeat" << endl;

		}
		if (node == 105)
			break;

		int next_node = best_path[i + 1];
		current_delay += means[node][next_node];


	}
	cout << endl << "ship " << ship << " plan finish  min var:" << min_var << "  " << best_path.size() << " hops  arrive at" << current_delay << endl;

}


void check_solution(vector<vector<int>>& solution, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<ld>& ini_delays)
{
	//看一下每个ship有多少个点是重复的，重新计算一次var
	int repeat_cnt = 0;
	for (int ship = 0; ship < 12; ++ship)
	{
		ld current_delay = ini_delays[ship];
		ld total_var = 0;
		vector<int>& route = solution[ship];
		ld big_var = 0;
		int node = route[0];
		int j = 1;
		int cnt = 0;
		while (node != 105)
		{
			
			if (vars[node][route[j]] > 3e-4)
				++cnt;
			
			//total_var += vars[node][route[j]] * pow(10, k);
			total_var += vars[node][route[j]];
			big_var = max(big_var, vars[node][route[j]]);
			current_delay += means[node][route[j]];
			/*if (visited[node].size() > 1)
				cout << "node " << node << " repeat" << endl;*/
			node = route[j];


			++j;
		}
		cout << "ship " << ship << " final var: " << total_var << "  arrive time: " << current_delay << "  ave var: " << total_var / j << "  big var: " << big_var << " hops: " << j << endl;
	}

	//cout << "total repeat: " << repeat_cnt << endl;
}


void write_solution(vector<vector<int>>& solution, string filename)
{
	string filepath, name;

	/*int path_pos = filename.find_last_of('\\');
	filepath = filename.substr(0, path_pos + 1);
	name = filename.substr(path_pos + 1);
	filepath += "exceed-timewindow\\";*/

	//ofstream outfile(filepath + name);
	ofstream outfile(filename);
	for (int ship = 0; ship < 12; ship ++)
	{
		vector<int>& route = solution[ship];

		int cnt = 0;
		for (auto node : route)
		{
			if (node == 105)
				break;

			outfile << node << " ";
			++cnt;
		}
		outfile << 105 << endl;
		/*while (cnt < 500)
		{
			outfile << 0 << ",";
			++cnt;
		}*/
	}
	
	outfile.close();
}

void delete_ship_path(int ship, vector<vector<int>>& solution, vector<vector<ld>>& visited, vector<vector<ld>>& means, vector<ld>& ini_delays)
{
	//删除掉某个ship对应的path

	ld current_delay = ini_delays[ship];

	vector<int>& path = solution[ship];


	for (int i = 0; path[i] != 105; ++i)
	{
		int node = path[i];
		int next_node = path[i + 1];

		if (visited[node].size() == 1)
			visited[node].clear();
		else
		{
			for (int k = 0; k < visited[node].size(); ++k)
			{
				if (abs(visited[node][k] - current_delay) < 1e-7)
				{
					visited[node].erase(visited[node].begin() + k);
					break;
				}
			}
		}




		current_delay += means[node][next_node];
	}

	solution[ship].clear();

}

void dfs(vector<vector<int>>& childs, vector<vector<ld>>& visited, ld left_time, ld right_time, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<vector<bool>>& dominated, vector<int>& path, ld current_delay, int node, ld current_var, vector<ld>& shortest_mean, vector<ld>& shortest_var, vector<vector<int>>& shortest_path, vector<int>& best_path, ld& best_var)
{
	if (node == 105)
	{
		if (current_delay > left_time && current_delay < right_time)
		{
			if (current_var < best_var)
			{
				best_path = path;
				best_var = current_var;
				cout << "found best " << best_var << endl;
			}
		}

		return;
	}
	if (current_var > best_var || current_var + shortest_var[path.back()] > best_var)
	{
		//cout << "exceed" << endl;
		return;
	}
	int test_node = path.back();
	if (current_delay + shortest_mean[test_node] > left_time && current_delay + shortest_mean[test_node] < right_time)
	{
		int j = 0;
		while (test_node != 105)
		{
			if (visited[test_node].size() > 0)
				break;
			if (find(path.begin(), path.end(), test_node) != path.end())
				break;

			++j;
			test_node = shortest_path[test_node][j];
		}
	}
	if (test_node == 105)
	{
		best_var = current_var + shortest_var[path.back()];
		best_path = path;
		int node = path.back();
		best_path.erase(best_path.end() - 1);
		best_path.insert(best_path.end(), shortest_path[node].begin(), shortest_path[node].end());

		return;
	}
	for (auto child : childs[node])
	{
		if (visited[child].size() > 0 && child != 105 || find(path.begin(), path.end(), child) != path.end())
			continue;
		if (dominated[node][child])
			continue;
		path.emplace_back(child);
		dfs(childs, visited, left_time, right_time, means, vars, dominated, path, current_delay + means[node][child], child, current_var + vars[node][child], shortest_mean, shortest_var, shortest_path, best_path, best_var);
		path.erase(path.end() - 1);
	}
	return;
}

void adjust_solution_later(vector<vector<int>>& solution, ld left_time, ld right_time, vector<vector<ld>>& visited, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<ld>& ini_delays, vector<ld>& shortest_mean, vector<ld>& shortest_var, vector<vector<int>>& shortest_path, vector<vector<int>>& origins, vector<vector<int>>& childs, vector<vector<bool>>& dominated)

{
	//整体时间窗后移
	//主要看0,1,2和8,9,10,11七个
	//3-7的ship直接检查是否在left~right区间内，不在就delete_path然后重新generate

	//对后移来说，先看8-11，看他们路径中间哪里提前满足了
	vector<int> old_path;
	for (int ship = 11; ship >= 8; --ship)
	{
		old_path = solution[ship];
		delete_ship_path(ship, solution, visited, means, ini_delays);

		//检查old_path
		ld current_delay = ini_delays[ship];

		for (int i = 0; old_path[i] != 105; ++i)
		{
			int node = old_path[i];

			//check if shortest feasible
			bool shortet_feasible = false;

			if (current_delay + shortest_mean[node] < right_time && current_delay + shortest_mean[node] > left_time)
			{
				//时间满足

				//检查冲突
				for (auto check_node : shortest_path[node])
				{
					if (check_node == 105)
					{
						shortet_feasible = true;
						break;
					}

					if (visited[check_node].size() > 0)
					{
						break;
					}
				}
			}



			if (shortet_feasible)
			{
				//old_path从node开始改成shortest_path[node]
				int j = i;
				for (auto check_node : shortest_path[node])
				{
					old_path[j] = check_node;
					++j;
					if (check_node == 105)
						break;
				}
				cout << "ship " << ship << "  best at hop " << j << endl;
				break;
			}

			current_delay += means[node][old_path[i + 1]];
		}

		//新的path写回solution
		solution[ship] = old_path;

		//修改visited
		current_delay = ini_delays[ship];
		for (int i = 0; solution[ship][i] != 105; ++i)
		{
			int node = solution[ship][i];

			visited[node].emplace_back(current_delay);

			current_delay += means[node][solution[ship][i + 1]];
		}
	}



	//再看0-2,从原有路径上继续拓展
	//ship0-ship2的路径上时间层面最靠后的就是105之前的一个点
	for (int ship = 0; ship < 3; ++ship)
	{
		//找到105之前means最大的一个点
		/*ld current_delay = ini_delays[ship];
		int flag_node = solution[ship][0];
		ld max_delay = current_delay;

		for (int i = 1; solution[ship][i] != 105; ++i)
		{
			int pre_node = solution[ship][i - 1], node = solution[ship][i];
			current_delay += means[pre_node][node];

			if (current_delay > max_delay)
			{
				max_delay = current_delay;
				flag_node = node;
			}
		}
		int j = 0;
		for (j = 0; solution[ship][j] != flag_node; ++j)
		{

		}
		cout << "ship " << ship << endl;
		for (j; solution[ship][j] != 105; ++j)
		{
			cout << solution[ship][j] << "  ";
		}
		cout << 105 << endl;*/
		//在old_path的基础上继续向时间更晚的方向前进
		int para_0 = 13;
		ld current_delay = ini_delays[ship];
		ld current_var = 0;
		vector<int> path1, path2;
		if (ship == 0)
		{
			for (int i = 1; solution[ship][i + para_0] != 105; ++i)
			{
				int pre_node = solution[ship][i - 1], node = solution[ship][i];
				current_delay += means[pre_node][node];
				current_var += vars[pre_node][node];
			}
			path1 = solution[1];
			path2 = solution[2];
			delete_ship_path(1, solution, visited, means, ini_delays);
			delete_ship_path(2, solution, visited, means, ini_delays);
		}
		else
		{
			for (int i = 1; solution[ship][i] != 105; ++i)
			{
				int pre_node = solution[ship][i - 1], node = solution[ship][i];
				current_delay += means[pre_node][node];
				current_var += vars[pre_node][node];
			}
		}
		vector<int> path = solution[ship];
		vector<int> rec_path = solution[ship];
		delete_ship_path(ship, solution, visited, means, ini_delays);
		//current_delay
		int node = *(path.end() - 2);
		if (ship == 0)
			node = *(path.end() - para_0 - 2);
		cout << node << endl;
		if (ship > 0)
		{
			path.erase(path.end() - 1);
		}
		else
		{
			path.erase(path.end() - para_0 - 1, path.end());
		}
		cout << path.back() << endl;
		vector<int> best_path;
		ld best_var = 0.06;
		dfs(childs, visited, left_time, right_time, means, vars, dominated, path, current_delay, node, current_var, shortest_mean, shortest_var, shortest_path, best_path, best_var);
		cout << best_var << endl;

		if (best_path.size() > 0)
		{
			//生成了新路

			//加入path的visit_time
			ld current_delay = ini_delays[ship];
			for (int i = 0; best_path[i] != 105; ++i)
			{
				int node = best_path[i];
				visited[node].emplace_back(current_delay);
				current_delay += means[node][best_path[i + 1]];
			}

			solution[ship] = best_path;
		}
		else
		{
			//新路找失败了
			best_path = rec_path;
			ld current_delay = ini_delays[ship];
			for (int i = 0; best_path[i] != 105; ++i)
			{
				int node = best_path[i];
				visited[node].emplace_back(current_delay);
				current_delay += means[node][best_path[i + 1]];
			}

			solution[ship] = best_path;
		}
		if (ship == 0)
		{
			ld current_delay = ini_delays[1];
			for (int i = 0; path1[i] != 105; ++i)
			{
				int node = path1[i];
				visited[node].emplace_back(current_delay);
				current_delay += means[node][path1[i + 1]];
			}
			solution[1] = path1;

			current_delay = ini_delays[2];
			for (int i = 0; path2[i] != 105; ++i)
			{
				int node = path2[i];
				visited[node].emplace_back(current_delay);
				current_delay += means[node][path2[i + 1]];
			}
			solution[2] = path2;
		}
	}


}


void adjust_solution_earlier(vector<vector<int>>& solution, ld left_time, ld right_time, vector<vector<ld>>& visited, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<ld>& ini_delays, vector<ld>& shortest_mean, vector<ld>& shortest_var, vector<vector<int>>& shortest_path, vector<vector<int>>& origins, vector<vector<int>>& childs)
{
	//时间窗前移



}

void check_domi(vector<vector<int>>& childs, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<vector<bool>>& dominated)
{
	int cnt = 0;
	for (int node = 1; node <= 10000; ++node)
	{
		for (auto child : childs[node])
		{
			ld child_mean = means[node][child], child_var = vars[node][child];
			bool flag1 = false, flag2 = false;
			for (auto other : childs[node])
			{
				if (other == child)
					continue;
				if (means[node][other] < child_mean && vars[node][other] < child_var)
				{
					flag1 = true;
				}
				if (means[node][other] > child_mean && vars[node][other] < child_var)
				{
					flag2 = true;
				}
			}
			if (flag1 && flag2)
				dominated[node][child] = true;
		}
	}
}





void deal_with_conflict(vector<vector<int>>& solution, vector<vector<ld>>& visited, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<ld>& ini_delays, vector<ld>& shortest_mean, vector<ld>& shortest_var, vector<vector<int>>& shortest_path, vector<vector<int>>& origins, vector<vector<int>>& childs)
{
	//先解决ship 0 和 ship 11
	ld ship_0_var = 0, ship_11_var = 0;

	//各走一遍，记下初始状态的var
	for (int i = 1; i < solution[0].size(); ++i)
	{
		int prenode = solution[0][i - 1], node = solution[0][i];
		ship_0_var += vars[prenode][node];
	}

	for (int i = 1; i < solution[11].size(); ++i)
	{
		int prenode = solution[11][i - 1], node = solution[11][i];
		ship_11_var += vars[prenode][node];
	}


	//两个ship的起点肯定不会重复
	//search_width不能小于3，小于3就大于275.9了
	int search_width = 7;
	for (int i = 0; solution[0][i] != 105; ++i)
	{
		if (visited[solution[0][i]].size() > 1)
		{
			//有重复点
			//ship0关于这个重复点左右两个最近的不重复的点

			int conf_node = solution[0][i];

			int ship_0_left_node = solution[0][0], ship_0_right_node = 105, ship_0_left_index = 0, ship_0_right_index = solution[0].size() - 1;
			int cnt = 0;
			for (int j = i - 1; j >= 0; --j)
			{
				int node = solution[0][j];
				if (visited[node].size() == 1)
				{
					++cnt;
					if (cnt == search_width)
					{
						ship_0_left_index = j;
						ship_0_left_node = node;

						break;
					}
				}
			}
			cnt = 0;
			for (int j = i + 1; j < solution[0].size(); ++j)
			{
				int node = solution[0][j];
				if (visited[node].size() == 1)
				{
					++cnt;
					if (cnt == search_width)
					{
						ship_0_right_index = j;
						ship_0_right_node = node;

						break;
					}
				}
			}

			int k = 0;
			for (k = 0; solution[11][k] != 105; ++k)
			{
				if (solution[11][k] == conf_node)
					break;
			}

			if (solution[11][k] == 105)
				continue;

			int ship_11_left_node = solution[11][0], ship_11_right_node = 105, ship_11_left_index = 0, ship_11_right_index = solution[11].size() - 1;
			cnt = 0;
			for (int j = k - 1; j >= 0; --j)
			{
				int node = solution[11][j];
				if (visited[node].size() == 1)
				{
					++cnt;
					if (cnt == search_width)
					{
						ship_11_left_index = j;
						ship_11_left_node = node;

						break;
					}
				}
			}
			cnt = 0;
			for (int j = k + 1; j < solution[11].size(); ++j)
			{
				int node = solution[11][j];
				if (visited[node].size() == 1)
				{
					++cnt;
					if (cnt == search_width)
					{
						ship_11_right_index = j;
						ship_11_right_node = node;

						break;
					}
				}
			}

			//记录下ship0和ship11原始的从left_node到right_node的时间偏移和var
			ld ship_0_old_mean = 0, ship_0_old_var = 0, ship_11_old_mean = 0, ship_11_old_var = 0;

			for (int j = ship_0_left_index + 1; j <= ship_0_right_index; ++j)
			{
				int prenode = solution[0][j - 1], node = solution[0][j];
				ship_0_old_mean += means[prenode][node];
				ship_0_old_var += vars[prenode][node];
			}

			for (int j = ship_11_left_index + 1; j <= ship_11_right_index; ++j)
			{
				int prenode = solution[11][j - 1], node = solution[11][j];
				ship_11_old_mean += means[prenode][node];
				ship_11_old_var += vars[prenode][node];
			}


			ld ship_0_best_var = ship_0_old_var*2, ship_11_best_var = ship_11_old_var*2;
			//ship 0 去掉中间部分的左右两侧路径
			vector<vector<int>> partial_paths(2);
			partial_paths[0].assign(solution[0].begin(), solution[0].begin() + ship_0_left_index);

			partial_paths[1].assign(solution[0].begin() + ship_0_right_index, solution[0].end());


			vector<int> ship_0_best_path, ship_0_path;


			ship_0_path.emplace_back(ship_0_left_node);


			//cout << "start search ship0  old var: "<<ship_0_old_var <<"  " << ship_0_left_node << " to " << ship_0_right_node << endl;
			dfs_partial(0, ship_0_right_node, childs, partial_paths, solution[11], ship_0_old_mean, means, vars, ship_0_path, 0, ship_0_left_node, 0, ship_0_best_path, ship_0_best_var, shortest_var);

			//cout << ship_0_best_var << endl;


			//搜ship11
			partial_paths.clear();
			partial_paths[0].assign(solution[11].begin(), solution[11].begin() + ship_11_left_index);

			partial_paths[1].assign(solution[11].begin() + ship_11_right_index, solution[11].end());


			vector<int> ship_11_best_path, ship_11_path;


			ship_11_path.emplace_back(ship_11_left_node);


			//cout << "start search ship11  old var: " << ship_11_old_var << "  " << ship_11_left_node << " to " << ship_11_right_node << endl;
			//if(!(ship_11_right_node == 105 && abs(ship_11_old_var - shortest_var[ship_11_left_node]) < 1e-6))
			dfs_partial(11, ship_11_right_node, childs, partial_paths, solution[0], ship_11_old_mean, means, vars, ship_11_path, 0, ship_11_left_node, 0, ship_11_best_path, ship_11_best_var, shortest_var);

			//cout << ship_11_best_var << endl;


			if (ship_0_best_path.size() == 0 && ship_11_best_path.size() == 0)
			{
				cout << "no route" << endl;
				continue;
			}

			if (ship_11_best_path.size() == 0 || ship_0_best_path.size() > 0 && max(ship_0_var - ship_0_old_var + ship_0_best_var, ship_11_var) < max(ship_0_var, ship_11_var - ship_11_old_var+ship_11_best_var))
			{
				//换ship0
	
				for (int j = ship_0_left_index + 1; j <= ship_0_right_index; ++j)
				{
					//消除这些node的visited记录
					int node = solution[0][j];
					if (visited[node].size() == 1)
						visited[node].clear();
					else
					{
						if (visited[node][0] < 0)
						{
							visited[node].erase(visited[node].begin());
						}
						else
						{
							visited[node].erase(visited[node].begin() + 1);
						}
					}
				}
				ld curr_delay = 0;
				for (int j = 1; j <= ship_0_left_index; ++j)
				{
					int node = solution[0][j], prenode = solution[0][j - 1];
					curr_delay += means[prenode][node];

				}
				for (int j = 1; j < ship_0_best_path.size(); ++j)
				{
					int node = ship_0_best_path[j], prenode = ship_0_best_path[j - 1];
					curr_delay += means[prenode][node];
					visited[node].emplace_back(curr_delay);
				}
				solution[0].erase(solution[0].begin() + ship_0_left_index, solution[0].begin() + ship_0_right_index + 1);
				solution[0].insert(solution[0].begin() + ship_0_left_index, ship_0_best_path.begin(), ship_0_best_path.end());
				ship_0_var = ship_0_var - ship_0_old_var + ship_0_best_var;
			}
			else
			{
				
				//换ship11

				for (int j = ship_11_left_index + 1; j <= ship_11_right_index; ++j)
				{
					//消除visited
					int node = solution[11][j];
					if (visited[node].size() == 1)
						visited[node].clear();
					else
					{
						if (visited[node][0] > 0)
						{
							visited[node].erase(visited[node].begin());
						}
						else
						{
							visited[node].erase(visited[node].begin() + 1);
						}
					}
				}
				ld curr_delay = 0;
				for (int j = 1; j <= ship_11_left_index; ++j)
				{
					int node = solution[11][j], prenode = solution[11][j - 1];
					curr_delay += means[prenode][node];

				}
				for (int j = 1; j < ship_11_best_path.size(); ++j)
				{
					int node = ship_11_best_path[j], prenode = ship_11_best_path[j - 1];
					curr_delay += means[prenode][node];
					visited[node].emplace_back(curr_delay);
				}
				solution[11].erase(solution[11].begin() + ship_11_left_index, solution[11].begin() + ship_11_right_index + 1);
				solution[11].insert(solution[11].begin() + ship_11_left_index, ship_11_best_path.begin(), ship_11_best_path.end());
				ship_11_var = ship_11_var - ship_11_old_var + ship_11_best_var;
			}
		}
	}
}


void dfs_partial(int ship, int destination, vector<vector<int>>& childs, vector<vector<int>>& partial_paths, vector<int>& other_path, ld old_mean, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<int>& path, ld current_delay, int node, ld current_var, vector<int>& best_path, ld& best_var, vector<ld>& shortest_var)
{
	if (node == destination)
	{
		/*if (ship == 0 && current_delay > old_mean || ship == 11 && current_delay < old_mean)
		{
			if (current_var < best_var)
			{
				best_path = path;
				best_var = current_var;
				cout << "found best " << best_var <<"  hops: "<<path.size() << endl;
			}
		}*/
		if (current_var < best_var)
		{
			best_path = path;
			best_var = current_var;
			//cout << "found best " << best_var << "  hops: " << path.size() << endl;
		}
		return;
	}
	if (current_var > best_var)
	{
		return;
	}
	if (node == 105)
		return;
	for (auto child : childs[node])
	{
		if (child != destination)
		{
			//检查重复 partial_paths和other_path
			bool repeat = false;
			for (auto& partial_path : partial_paths)
			{
				if (find(partial_path.begin(), partial_path.end(), child) != partial_path.end())
				{
					repeat = true;
					break;
				}
			}
			if (find(other_path.begin(), other_path.end(), child) != other_path.end())
				repeat = true;

			if (find(path.begin(), path.end(), child) != path.end())
				repeat = true;

			if (repeat)
				continue;
		}
		if (destination ==105 && current_var + shortest_var[child] > best_var)
			continue;

		if (vars[node][child] > 3e-4)
			continue;
		path.emplace_back(child);
		dfs_partial(ship, destination, childs, partial_paths, other_path, old_mean, means, vars, path, current_delay + means[node][child], child, current_var + vars[node][child], best_path, best_var, shortest_var);
		path.erase(path.end() - 1);
		
	}
	return;
}

void refinement(vector<vector<int>>& solution, vector<vector<int>>& childs, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<ld>& ini_delays, ld max_var, ld left_time, ld right_time)
{
	//处理完冲突，ship0和11的路线超过了时间窗约束，在这里完善，让0和11多绕一些路
	//两条路的var都不超过max_var, 时间落在left_time和right_time里
	vector<int> ship_0_path = solution[0], ship_11_path = solution[11];

	int search_depth = 4;

	ld ship_0_mean = 0, ship_0_var = 0, ship_11_mean = 0, ship_11_var = 0;


	//统计一遍旧的
	ship_0_mean = ini_delays[0];
	for (int i = 1; i < ship_0_path.size(); ++i)
	{
		int node = ship_0_path[i], prenode = ship_0_path[i - 1];
		ship_0_mean += means[prenode][node];
		ship_0_var += vars[prenode][node];
	}

	ship_11_mean = ini_delays[11];
	for (int i = 1; i < ship_11_path.size(); ++i)
	{
		int node = ship_11_path[i], prenode = ship_11_path[i - 1];
		ship_11_mean += means[prenode][node];
		ship_11_var += vars[prenode][node];
	}

	//每一轮只挑一处做改动，如果不能改到时间窗里，就选一个靠近时间窗的，var增加最小的

	//先处理ship 0
	while (ship_0_mean < left_time)
	{
		int insert_pos = -1;
		vector<int> new_path;
		ld path_var = 0;


		//修改后能否满足时间窗
		bool feasible = false;

		ld var_increase = 1;
		ld mean_change = 0;

		break;
		for (int i = 1; ship_0_path[i] != 105; ++i)
		{
			int prenode = ship_0_path[i - 1], node = ship_0_path[i];

			ld old_mean = means[prenode][node];

			vector<int> path;
			ld best_var = 0.01, best_path_mean = 0;
			vector<int> best_path;
			path.emplace_back(prenode);
			//做一个dfs，到达node时的深度不能为1，不超过search_depth,选择一个mean符合ship要求的，var最小的
			dfs_refine(0, node, childs, ship_0_path, ship_11_path, old_mean, means, vars, path, 0, prenode, 0, best_path, best_var, best_path_mean, search_depth);


			if (best_path.size() == 0)
				continue;

			//检查是否替换new_path
			if (feasible)
			{
				if (ship_0_mean - old_mean + best_path_mean > left_time)
				{
					//best_path也feasible
					ld old_var = vars[prenode][node];
					if (best_var - old_var < var_increase)
					{
						new_path = best_path;
						var_increase = best_var - old_var;
						mean_change = best_path_mean - old_mean;
						insert_pos = i - 1;
					}
				}
			}
			else
			{
				if (ship_0_mean - old_mean + best_path_mean > left_time)
				{
					//best_path  feasible
					ld old_var = vars[prenode][node];

					feasible = true;
					new_path = best_path;
					var_increase = best_var - old_var;
					mean_change = best_path_mean - old_mean;
					insert_pos = i - 1;

				}
				else
				{
					ld old_var = vars[prenode][node];

					if (best_var - old_var < var_increase)
					{
						new_path = best_path;
						var_increase = best_var - old_var;
						mean_change = best_path_mean - old_mean;
						insert_pos = i - 1;
					}
				}
			}
			if (insert_pos >= 0)
				break;
		}

		//new_path有没有内容
		if (insert_pos >= 0)
		{
			int prenode = ship_0_path[insert_pos], node = ship_0_path[insert_pos + 1];
			ld old_mean = means[prenode][node], old_var = vars[prenode][node];

			if (ship_0_var + var_increase > max_var)
				break;
			/*ld test = 0;
			for (int i = 1; i < new_path.size(); ++i)
			{
				test += means[new_path[i - 1]][new_path[i]];
			}
			cout << "mean change : " << mean_change << "  check val:  " << test - old_mean << endl;
			test = 0;
			for (int i = 1; i < new_path.size(); ++i)
			{
				test += vars[new_path[i - 1]][new_path[i]];
			}
			cout << "var change : " << var_increase << "  check val:  " << test - old_var << endl;*/
			for (int i = 1; new_path[i] != node; ++i)
			{
				ship_0_path.insert(ship_0_path.begin() + insert_pos + i, new_path[i]);
			}

			ship_0_mean += mean_change;
			ship_0_var += var_increase;
			cout << "ship 0 mean: " << ship_0_mean << "  var : " << ship_0_var << endl;
		}
		else
		{
			//找不到路了
			break;
		}

		

	}
	solution[0] = ship_0_path;
	//处理ship11

	while (ship_11_mean > right_time)
	{
		int insert_pos = -1;
		vector<int> new_path;
		ld path_var = 0;


		//修改后能否满足时间窗
		bool feasible = false;

		ld var_increase = 1;
		ld mean_change = 0;


		for (int i = 1; ship_11_path[i] != 105; ++i)
		{
			int prenode = ship_11_path[i - 1], node = ship_11_path[i];

			ld old_mean = means[prenode][node];

			vector<int> path;
			ld best_var = 0.01, best_path_mean = 0;
			vector<int> best_path;
			path.emplace_back(prenode);
			//做一个dfs，到达node时的深度不能为1，不超过search_depth,选择一个mean符合ship要求的，var最小的
			dfs_refine(11, node, childs, ship_0_path, ship_11_path, old_mean, means, vars, path, 0, prenode, 0, best_path, best_var, best_path_mean, search_depth);


			if (best_path.size() == 0)
				continue;

			//检查是否替换new_path
			if (feasible)
			{
				if (ship_11_mean - old_mean + best_path_mean < right_time)
				{
					//best_path也feasible
					ld old_var = vars[prenode][node];
					if (best_var - old_var < var_increase)
					{
						new_path = best_path;
						var_increase = best_var - old_var;
						mean_change = best_path_mean - old_mean;
						insert_pos = i - 1;
					}
				}
			}
			else
			{
				if (ship_11_mean - old_mean + best_path_mean < right_time)
				{
					//best_path  feasible
					ld old_var = vars[prenode][node];

					feasible = true;
					new_path = best_path;
					var_increase = best_var - old_var;
					mean_change = best_path_mean - old_mean;
					insert_pos = i - 1;

				}
				else
				{
					ld old_var = vars[prenode][node];

					if (best_var - old_var < var_increase)
					{
						new_path = best_path;
						var_increase = best_var - old_var;
						mean_change = best_path_mean - old_mean;
						insert_pos = i - 1;
					}
				}
			}
			if (insert_pos >= 0)
				break;
		}

		//new_path有没有内容
		if (insert_pos >= 0)
		{
			int prenode = ship_11_path[insert_pos], node = ship_11_path[insert_pos + 1];
			ld old_mean = means[prenode][node], old_var = vars[prenode][node];

			if (ship_11_var + var_increase > max_var)
				break;
			/*ld test = 0;
			for (int i = 1; i < new_path.size(); ++i)
			{
				test += means[new_path[i - 1]][new_path[i]];
			}
			cout << "mean change : " << mean_change << "  check val:  " << test - old_mean << endl;
			test = 0;
			for (int i = 1; i < new_path.size(); ++i)
			{
				test += vars[new_path[i - 1]][new_path[i]];
			}
			cout << "var change : " << var_increase << "  check val:  " << test - old_var << endl;*/
			for (int i = 1; new_path[i] != node; ++i)
			{
				ship_11_path.insert(ship_11_path.begin() + insert_pos + i, new_path[i]);
			}

			ship_11_mean += mean_change;
			ship_11_var += var_increase;

			cout << "ship 11 mean: " << ship_11_mean << "  var : " << ship_11_var << endl;
		}
		else
		{
			//找不到路了
			break;
		}


	}
	solution[11] = ship_11_path;
}

void dfs_refine(int ship, int destination, vector<vector<int>>& childs, vector<int>& ship_0_path, vector<int>& ship_11_path, ld old_mean, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<int>& path, ld current_delay, int node, ld current_var, vector<int>& best_path, ld& best_var, ld& best_path_mean, int search_depth)
{
	if (search_depth < 0)
		return;

	if (node == destination)
	{
		if (ship == 0 && current_delay > old_mean && path.size() > 2 || ship == 11 && current_delay < old_mean && path.size() > 2)
		{
			if (current_var < best_var)
			{
				best_path = path;
				best_var = current_var;
				best_path_mean = current_delay;
				//cout << "found best " << best_var <<"  hops: "<<path.size() << endl;
			}
		}
		
		return;
	}
	if (current_var > best_var)
	{
		return;
	}
	
	for (auto child : childs[node])
	{
		if (child != destination)
		{
			//检查重复 
			bool repeat = false;
			if (find(ship_0_path.begin(), ship_0_path.end(), child) != ship_0_path.end())
				repeat = true;

			if (find(ship_11_path.begin(), ship_11_path.end(), child) != ship_11_path.end())
				repeat = true;

			if (find(path.begin(), path.end(), child) != path.end())
				repeat = true;

			if (repeat)
				continue;
		}
		
		path.emplace_back(child);
		dfs_refine(ship, destination, childs, ship_0_path, ship_11_path, old_mean, means, vars, path, current_delay + means[node][child], child, current_var + vars[node][child], best_path, best_var, best_path_mean, search_depth - 1);
		path.erase(path.end() - 1);

	}
	return;

}


void transfer_file(string filename)
{
	ifstream infile(filename);


	if (infile.good())
		cout << "good" << endl;

	string linestr;

	//getline(infile, linestr);

	


	string filepath, name;
	int pathpos = filename.find_last_of('\\');
	filepath = filename.substr(0, pathpos + 1);
	name = filename.substr(pathpos + 1);



	ofstream outfile(filepath + "new-" +name);
	int ship = 0;
	while (ship < 12)
	{
		getline(infile, linestr);

		for (auto& c : linestr)
		{
			if (c == ',')
				c = ' ';
		}
		
		++ship;
		outfile << linestr << endl;
	}



	infile.close();
	outfile.close();
}

void make_tree_var(vector<vector<int>>& reverse_childs, vector<ld>& result, vector<vector<ld>>& vars, vector<bool>& is_in_tree, vector<bool>& is_in_path)
{
	//加入到树里的点标20，本来不可达的标10
	
	//先记录一个reverse_childs
	

	result.assign(501, 10);

	vector<ld> distance(10001, 10);
	



	result[0] = 0;
	distance[105] = 20;
	for (auto t_node : reverse_childs[105])
	{
		distance[t_node] = vars[t_node][105];
	}


	int iter = 1;
	while (iter <= 500)
	{
		ld edge_cost = 5;
		int add_node = 0;

		//选点
		for (int node = 1; node <= 10000; ++node)
		{
			if (is_in_path[node])
			{
				distance[node] = 20;
				continue;
			}
			if (distance[node] < edge_cost)
			{
				add_node = node;
				edge_cost = distance[node];
			}
		}

		if (add_node == 0)
		{
			cout << "add node 0 error" << endl;
			break;
		}

		result[iter] = result[iter - 1] + edge_cost;
		distance[add_node] = 20;
		is_in_tree[add_node] = true;

		for (auto node : reverse_childs[add_node])
		{
			if (distance[node] > 15)
				continue;

			distance[node] = min(distance[node], vars[node][add_node]);
		}


		++iter;

	}

	/*ofstream outfile("../data/tree_var.txt");
	for (int i = 1; i <= 500; ++i)
	{
		outfile << i << " " << result[i] << endl;
	}
	outfile.close();*/
}


void make_tree_posi(vector<vector<int>>& reverse_childs, vector<ld>& result, vector<vector<ld>>& means, vector<bool>& is_in_tree, vector<bool>& is_in_path)
{


	result.assign(501, 10);

	vector<ld> distance(10001, -10);




	result[0] = 0;
	distance[105] = -20;
	for (auto t_node : reverse_childs[105])
	{
		distance[t_node] = means[t_node][105];
	}


	int iter = 1;
	while (iter <= 500)
	{
		ld edge_gain = -5;
		int add_node = 0;

		//选点
		for (int node = 1; node <= 10000; ++node)
		{
			if (is_in_path[node])
			{
				distance[node] = -20;
				continue;
			}
			if (distance[node] > edge_gain)
			{
				add_node = node;
				edge_gain = distance[node];
			}
		}

		if (add_node == 0)
		{
			cout << "add node 0 error" << endl;
			break;
		}

		result[iter] = result[iter - 1] + edge_gain;
		distance[add_node] = -20;
		is_in_tree[add_node] = true;

		for (auto node : reverse_childs[add_node])
		{
			if (distance[node] < -15)
				continue;

			distance[node] = max(distance[node], means[node][add_node]);
		}


		++iter;

	}

	/*ofstream outfile("../data/tree_posi.txt");
	for (int i = 1; i <= 500; ++i)
	{
		outfile << i << " " << result[i] << endl;
	}
	outfile.close();*/
}

void make_tree_nege(vector<vector<int>>& reverse_childs, vector<ld>& result, vector<vector<ld>>& means, vector<bool>& is_in_tree, vector<bool>& is_in_path)
{

	result.assign(501, 10);

	vector<ld> distance(10001, 10);




	result[0] = 0;
	distance[105] = 20;
	for (auto t_node : reverse_childs[105])
	{
		distance[t_node] = means[t_node][105];
	}


	int iter = 1;
	while (iter <= 500)
	{
		ld edge_cost = 5;
		int add_node = 0;

		//选点
		for (int node = 1; node <= 10000; ++node)
		{
			if (is_in_path[node])
			{
				distance[node] = 20;
				continue;
			}
			if (distance[node] < edge_cost)
			{
				add_node = node;
				edge_cost = distance[node];
			}
		}

		if (add_node == 0)
		{
			cout << "add node 0 error" << endl;
			break;
		}

		result[iter] = result[iter - 1] + edge_cost;
		distance[add_node] = 20;
		is_in_tree[add_node] = true;

		for (auto node : reverse_childs[add_node])
		{
			if (distance[node] > 15)
				continue;

			distance[node] = min(distance[node], means[node][add_node]);
		}


		++iter;

	}

	/*ofstream outfile("../data/tree_nege.txt");
	for (int i = 1; i <= 500; ++i)
	{
		outfile << i << " " << result[i] << endl;
	}
	outfile.close();*/
}

void read_bound(string bound_path, vector<ld>& var_bound, vector<ld>& mean_posi_bound, vector<ld>& mean_nege_bound)
{
	string filename = bound_path + "tree_var.txt";

	ifstream infile(filename);

	for (int i = 1; i <= 500; ++i)
	{
		int iter;
		infile >> iter >> var_bound[i];
	}

	infile.close();


	filename = bound_path + "tree_posi.txt";
	infile.open(filename);

	for (int i = 1; i <= 500; ++i)
	{
		int iter;
		infile >> iter >> mean_posi_bound[i];
	}
	infile.close();

	filename = bound_path + "tree_nege.txt";
	infile.open(filename);

	for (int i = 1; i <= 500; ++i)
	{
		int iter;
		infile >> iter >> mean_nege_bound[i];
	}
	infile.close();

}



void path_find(ld left_time, ld right_time, vector<vector<int>>& childs, vector<vector<int>>& solution, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<ld>& ini_delays, vector<vector<int>>& origins, ld var_limit, vector<ld>& shortest_var)
{
	
	vector<bool> is_in_tree(10001, false);
	vector<ld> tree_var(501), tree_posi(501), tree_nege(501);

	vector<vector<int>> reverse_childs(10001, vector<int>());

	for (int node = 1; node <= 10000; ++node)
	{
		for (auto child : childs[node])
		{
			reverse_childs[child].emplace_back(node);
		}
	}
	//优先规划ship0, 再ship11
	vector<int> ship_0_rec_path, ship_11_rec_path;
	ld ship_0_rec_var = var_limit , ship_11_rec_var = var_limit;

	vector<int> ship_0_path;
	vector<bool> is_in_path(10001, false);
	make_tree_var(reverse_childs, tree_var, vars, is_in_tree, is_in_path);
	make_tree_posi(reverse_childs, tree_posi, means, is_in_tree, is_in_path);
	make_tree_nege(reverse_childs, tree_nege, means, is_in_tree, is_in_path);

	for (int node = 1;node<=10000;++node)
	{
		sort(childs[node].begin(), childs[node].end(), [&](int ca, int cb) {return vars[node][ca] < vars[node][cb]; });
	}

	for (auto origin : origins[0])
	{
		cout << "an origin" << endl;
		ship_0_path.emplace_back(origin);
		is_in_path[origin] = true;
		dfs_path_find(0, left_time, right_time, childs, means, vars, ship_0_path, ini_delays[0], origin, 0, tree_var, tree_posi, tree_nege, is_in_tree, ship_0_rec_path, ship_0_rec_var, shortest_var, 0, reverse_childs, is_in_path);
		ship_0_path.clear();
		is_in_path.assign(10001, false);
	}
	solution[0] = ship_0_rec_path;	

	//优先ship11，再ship0

}


void dfs_path_find(int ship, ld left_time, ld right_time, vector<vector<int>>& childs, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<int>& path, ld current_delay, int node, ld current_var, vector<ld>& tree_var, vector<ld>& tree_posi, vector<ld>& tree_nege, vector<bool>& is_in_tree, vector<int>& rec_path, ld& rec_var, vector<ld>& shortest_var, int depth, vector<vector<int>>& reverse_childs, vector<bool>& is_in_path)
{
	//cout << depth << endl;
	if (depth >= 500)
		return;
	if (node == 105)
	{
		if (current_delay > left_time && current_delay < right_time)
		{
			if (current_var < rec_var)
			{
				rec_path = path;
				rec_var = current_var;
				cout << "found best " << rec_var << endl;
			}
		}

		return;
	}
	if (current_var > rec_var)
	{
		//cout << "exceed" << endl;
		//cout << "cut at depth " << depth << endl;
		return;
	}

	//vector<bool> old_is_in_tree = is_in_tree;
	//vector<ld> old_tree_var = tree_var, old_tree_posi = tree_posi, old_tree_nege = tree_nege;


	//if (is_in_tree[node])
	//{
	//	//当前使用的最小生成树包含了node，要重新算
	//	is_in_tree.assign(10001, false);
	//	make_tree_var(reverse_childs, tree_var, vars, is_in_tree, is_in_path);
	//	make_tree_posi(reverse_childs, tree_posi, means, is_in_tree, is_in_path);
	//	make_tree_nege(reverse_childs, tree_nege, means, is_in_tree, is_in_path);
	//}
	//检查满足时间窗还要多少跳，经过这些跳之后最少还要多少cost
	if (current_delay < left_time)
	{
		int hops = 500;
		for (int k = 1; k <= 500; ++k)
		{
			if (current_delay + tree_posi[k] > left_time)
			{
				hops = k;
				break;
			}
		}
		
		if (current_var + tree_var[hops] > rec_var)
		{
			/*is_in_tree = old_is_in_tree;
			tree_var = old_tree_var;
			tree_posi = old_tree_posi;
			tree_nege = old_tree_nege;*/
			//cout << "cut at depth " << depth <<" var : "<<current_var << endl;
			return;
		}
	}
	else if (current_delay > right_time)
	{
		int hops = 500;
		for (int k = 1; k <= 500; ++k)
		{
			if (current_delay + tree_nege[k] < right_time)
			{
				hops = k;
				break;
			}
		}
		if (current_var + tree_var[hops] > rec_var)
		{
			/*is_in_tree = old_is_in_tree;
			tree_var = old_tree_var;
			tree_posi = old_tree_posi;
			tree_nege = old_tree_nege;*/
			//cout << "cut at depth " << depth << " var : " << current_var << endl;
			return;
		}
	}

	//选一些child拓展，不考虑全部的了
	int same_direc = 1, oppo_direc = 1;
	vector<int> posi_childs;
	vector<int> nege_childs;
	//posi_childs.reserve(same_direc);
	//nege_childs.reserve(same_direc);

	for (auto child : childs[node])
	{
		if (is_in_path[child])
			continue;
		if (vars[node][child] > 3e-3)
			break;
		if (child != 105 && current_var + vars[node][child] + shortest_var[child] > rec_var)
			continue;
		if (child == 105)
			continue;
		if (means[node][child] > 0 && posi_childs.size() < same_direc)
		{
			posi_childs.emplace_back(child);
		}
		else if(means[node][child] < 0 && nege_childs.size() < oppo_direc)
		{
			nege_childs.emplace_back(child);
		}

		if (posi_childs.size() >= same_direc && nege_childs.size() >= oppo_direc)
			break;
	}
	vector<int> childs_pool;
	//childs_pool.reserve(2 * same_direc);
	//可供选择的child分为了posi和nege,其中可能有一个包含105
	
	if (vars[node][105] > 0)
	{
		childs_pool.emplace_back(105);
	}

	/*sort(posi_childs.begin(), posi_childs.end(), [&](int childa, int childb) {return vars[node][childa] < vars[node][childb]; });
	sort(nege_childs.begin(), nege_childs.end(), [&](int childa, int childb) {return vars[node][childa] < vars[node][childb]; });*/
	if (current_delay < left_time)
	{
		same_direc = min(same_direc, (int)posi_childs.size());
		for (int i = 0; i < same_direc; ++i)
		{
			childs_pool.emplace_back(posi_childs[i]);
		}

		oppo_direc = min(oppo_direc, (int)nege_childs.size());
		for (int i = 0; i < oppo_direc; ++i)
		{
			childs_pool.emplace_back(nege_childs[i]);
		}

	}
	else if (current_delay > right_time)
	{
		same_direc = min(same_direc, (int)nege_childs.size());
		for (int i = 0; i < same_direc; ++i)
		{
			childs_pool.emplace_back(nege_childs[i]);
		}

		oppo_direc = min(oppo_direc, (int)posi_childs.size());
		for (int i = 0; i < oppo_direc; ++i)
		{
			childs_pool.emplace_back(posi_childs[i]);
		}

	}
	else
	{
		same_direc = 1, oppo_direc = 1;
		same_direc = min(same_direc, (int)posi_childs.size());
		for (int i = 0; i < same_direc; ++i)
		{
			childs_pool.emplace_back(posi_childs[i]);
		}

		oppo_direc = min(oppo_direc, (int)nege_childs.size());
		for (int i = 0; i < oppo_direc; ++i)
		{
			childs_pool.emplace_back(nege_childs[i]);
		}
	}


	for (auto child : childs_pool)
	{
		path.emplace_back(child);
		is_in_path[child] = true;
		dfs_path_find(ship, left_time, right_time, childs, means, vars, path, current_delay + means[node][child], child, current_var + vars[node][child], tree_var, tree_posi, tree_nege, is_in_tree, rec_path, rec_var, shortest_var, depth + 1, reverse_childs, is_in_path);
		path.erase(path.end() - 1);
		is_in_path[child] = false;
	}

	/*is_in_tree = old_is_in_tree;
	tree_var = old_tree_var;
	tree_posi = old_tree_posi;
	tree_nege = old_tree_nege;*/

	return;
}

void read_inis(vector<vector<vector<int>>>& ini_solutions, vector<pair<ld, ld>>& ini_solution_delays, vector<pair<ld, ld>>& ini_solution_vars, vector<ld>& ini_delays, vector<vector<ld>>& means, vector<vector<ld>>& vars)
{
	int cnt = 0;
	ini_solutions.resize(112);
	ini_solution_delays.resize(112);
	ini_solution_vars.resize(112);
	for (auto& ini : ini_solutions)
		ini.resize(12);
	for (auto& i : filesystem::directory_iterator("..\\data\\ini-solutions"))
	{
		string filename = i.path().string();
		ifstream infile(filename);
		int node = 0;
		while (node != 105)
		{
			infile >> node;
			ini_solutions[cnt][0].emplace_back(node);
		}

		node = 0;
		while (node != 105)
		{
			infile >> node;
			ini_solutions[cnt][11].emplace_back(node);
		}

		infile.close();
		++cnt;
	}

	for (int i = 0; i < 112; ++i)
	{
		ld delay = ini_delays[0];
		ld var = 0;
		vector<int>& ship_0 = ini_solutions[i][0];
		for (int j = 1; j < ship_0.size(); ++j)
		{
			int node = ship_0[j], prenode = ship_0[j - 1];
			delay += means[prenode][node];
			var += vars[prenode][node];
		}
		ini_solution_delays[i].first = delay;
		ini_solution_vars[i].first = var;

		delay = ini_delays[11];
		var = 0;
		vector<int>& ship_11 = ini_solutions[i][11];
		for (int j = 1; j < ship_11.size(); ++j)
		{
			int node = ship_11[j], prenode = ship_11[j - 1];
			delay += means[prenode][node];
			var += vars[prenode][node];
		}
		ini_solution_delays[i].second = delay;
		ini_solution_vars[i].second = var;
	}
	/*cout << ini_solution_delays[0].second - ini_solution_delays[0].first << endl;
	cout << ini_solution_vars[0].first << "  " << ini_solution_vars[0].second << endl;*/
}

void check_simi(vector<vector<vector<int>>>& ini_solutions)
{
	int a = 0, b = 0;
	int ship_0_min_simi = 300, ship_11_min_simi = 300;
	for (a = 0; a<112;++a)
	{
		for (b = a + 1; b < 112; ++b)
		{
			int cnt = 0;
			for (auto node : ini_solutions[a][0])
			{
				if (find(ini_solutions[b][0].begin(), ini_solutions[b][0].end(), node) != ini_solutions[b][0].end())
					++cnt;
			}
			ship_0_min_simi = min(ship_0_min_simi, cnt);
			cnt = 0;
			for (auto node : ini_solutions[a][11])
			{
				if (find(ini_solutions[b][11].begin(), ini_solutions[b][11].end(), node) != ini_solutions[b][11].end())
					++cnt;
			}
			ship_11_min_simi = min(ship_11_min_simi, cnt);
		}
		
	}
	cout << "ship 0 min simi: " << ship_0_min_simi << endl;    //253
	cout << "ship 11 min simi: " << ship_11_min_simi << endl;   //238
}

void inis_crossover(int index, vector<vector<vector<int>>>& ini_solutions, vector<pair<ld, ld>>& ini_solution_delays, vector<pair<ld, ld>>& ini_solution_vars, vector<ld>& ini_delays, vector<vector<ld>>& means, vector<vector<ld>>& vars)
{
	//用路径交叉的方式尝试修复ini_solutions中index下标的两个ship的路线
	
	vector<int>& ship_0_path = ini_solutions[index][0], &ship_11_path = ini_solutions[index][11];

	ld ship_0_old_delay = ini_solution_delays[index].first, ship_11_old_delay = ini_solution_delays[index].second;
	ld ship_0_old_var = ini_solution_vars[index].first, ship_11_old_var = ini_solution_vars[index].second;

	for (int i = 0; ship_0_path[i] != 105; ++i)
	{
		int node = ship_0_path[i];
		if (find(ship_11_path.begin(), ship_11_path.end(), node) != ship_11_path.end())
		{
			//消解冲突
			int pos_in_ship0 = i, pos_in_ship11 = find(ship_11_path.begin(), ship_11_path.end(), node) - ship_11_path.begin();
			int ship_0_rightpos = i, ship_0_leftpos = i;
			int ship_11_rightpos = pos_in_ship11, ship_11_leftpos = pos_in_ship11;


			bool conflict = true;
			while (conflict)
			{
				//ship0和ship11分别尝试往左右探一个
				int old_gap_0 = ship_0_rightpos - ship_0_leftpos;
				if (ship_0_leftpos > 0)
					--ship_0_leftpos;
				if (ship_0_path[ship_0_rightpos] != 105)
					++ship_0_rightpos;
				while (ship_0_path[ship_0_rightpos] != 105 && find(ship_11_path.begin(), ship_11_path.end(), ship_0_path[ship_0_rightpos]) != ship_11_path.end())
					++ship_0_rightpos;

				int old_gap_11 = ship_11_rightpos - ship_11_leftpos;
				if (ship_11_leftpos > 0)
					--ship_11_leftpos;
				if (ship_11_path[ship_11_rightpos] != 105)
					++ship_11_rightpos;
				while (ship_11_path[ship_11_rightpos] != 105 && find(ship_0_path.begin(), ship_0_path.end(), ship_11_path[ship_11_rightpos]) != ship_0_path.end())
					++ship_11_rightpos;

				if (ship_0_rightpos - ship_0_leftpos == old_gap_0 && ship_11_rightpos - ship_11_leftpos == old_gap_11)
					break;


				int ship0_start_node = ship_0_path[ship_0_leftpos], ship0_end_node = ship_0_path[ship_0_rightpos];
				int ship11_start_node = ship_11_path[ship_11_leftpos], ship11_end_node = ship_11_path[ship_11_rightpos];


				ld ship0_partial_delay = 0, ship0_partial_var = 0;
				ld ship0_replace_delay = 1, ship0_replace_var = 1;
				vector<int> ship0_replace_route;

				ld ship11_partial_delay = 0, ship11_partial_var = 0;
				ld ship11_replace_delay = 1, ship11_replace_var = 1;
				vector<int> ship11_replace_route;

				for (int pos = ship_0_leftpos + 1; pos <= ship_0_rightpos; ++pos)
				{
					int node = ship_0_path[pos], prenode = ship_0_path[pos - 1];
					ship0_partial_delay += means[prenode][node];
					ship0_partial_var += vars[prenode][node];
				}

				for (int pos = ship_11_leftpos + 1; pos <= ship_11_rightpos; ++pos)
				{
					int node = ship_11_path[pos], prenode = ship_11_path[pos - 1];
					ship11_partial_delay += means[prenode][node];
					ship11_partial_var += vars[prenode][node];
				}

				cout << "ship0 partial delay: " << ship0_partial_delay << endl;
				cout << "ship11 partial delay: " << ship11_partial_delay << endl;

				for (int j = 0; j < 112; ++j)
				{
					if (j == index)
						continue;

					vector<int>& ship0_ref_path = ini_solutions[j][0];
					vector<int>& ship11_ref_path = ini_solutions[j][11];

					//ship0能不能换
					if (find(ship0_ref_path.begin(), ship0_ref_path.end(), ship0_start_node) != ship0_ref_path.end()
						&& find(ship0_ref_path.begin(), ship0_ref_path.end(), ship0_end_node) != ship0_ref_path.end())
					{
						int pos1 = find(ship0_ref_path.begin(), ship0_ref_path.end(), ship0_start_node) - ship0_ref_path.begin();
						int pos2 = find(ship0_ref_path.begin(), ship0_ref_path.end(), ship0_end_node) - ship0_ref_path.begin();

						if (pos2 > pos1)
						{
							ld ref_partial_delay = 0, ref_partial_var = 0;
							bool repeat = false;
							for (int pos = pos1 + 1; pos <= pos2; ++pos)
							{
								int node = ship0_ref_path[pos], prenode = ship0_ref_path[pos - 1];
								ref_partial_delay += means[prenode][node];
								ref_partial_var += vars[prenode][node];
								if (pos != pos2 && !repeat)
								{
									if (count(ship_0_path.begin(), ship_0_path.begin() + ship_0_leftpos, node) != 0
										|| count(ship_0_path.begin() + ship_0_rightpos, ship_0_path.end(), node) != 0)
									{
										repeat = true;
									}
									if (count(ship_11_path.begin(), ship_11_path.end(), node) != 0)
									{
										repeat = true;
									}
										
								}
							}
							if (!repeat)
							{
								cout << "ref delay : " << ref_partial_delay << endl;
								if (ref_partial_var < ship0_replace_var)
								{
									ship0_replace_var = ref_partial_var;
									ship0_replace_delay = ref_partial_delay;
									ship0_replace_route.clear();
									for (int pos = pos1; pos <= pos2; ++pos)
									{
										ship0_replace_route.emplace_back(ship0_ref_path[pos]);
									}
								}
							}


						}
					}


					if (find(ship11_ref_path.begin(), ship11_ref_path.end(), ship11_start_node) != ship11_ref_path.end()
						&& find(ship11_ref_path.begin(), ship11_ref_path.end(), ship11_end_node) != ship11_ref_path.end())
					{
						int pos1 = find(ship11_ref_path.begin(), ship11_ref_path.end(), ship11_start_node) - ship11_ref_path.begin();
						int pos2 = find(ship11_ref_path.begin(), ship11_ref_path.end(), ship11_end_node) - ship11_ref_path.begin();

						if (pos2 > pos1)
						{
							ld ref_partial_delay = 0, ref_partial_var = 0;
							bool repeat = false;

							for (int pos = pos1 + 1; pos <= pos2; ++pos)
							{
								
							}

						}

					}

				}

				if (ship0_replace_route.size() > 0)
				{
					cout << "old route: " << endl;
					for (int pos = ship_0_leftpos; pos <= ship_0_rightpos; ++pos)
					{
						cout << ship_0_path[pos] << "  ";
					}
					cout << endl;
					
					ship_0_path.erase(ship_0_path.begin() + ship_0_leftpos, ship_0_path.begin() + ship_0_rightpos + 1);
					ship_0_path.insert(ship_0_path.begin() + ship_0_leftpos, ship0_replace_route.begin(), ship0_replace_route.end());

					cout << "new route: " << endl;
					for (int pos = ship_0_leftpos; ship_0_path[pos] != ship0_end_node; ++pos)
					{
						cout << ship_0_path[pos] << "  ";
					}
					cout <<ship0_end_node<< endl;
					
					conflict = false;
				}
			}


			if (conflict)
			{
				//没能消解掉
				cout << "no deal" << endl;
			}
			else
			{
				cout << "make a deal" << endl;
			}


		}
	}


	cout << "check: " << endl;
	ld new_var = 0;
	for (int i = 1; i<ship_0_path.size(); ++i)
	{
		int node = ship_0_path[i], prenode = ship_0_path[i-1];
		new_var += vars[prenode][node];
	}
	cout << new_var << endl;
	cout << "hops: " << ship_0_path.size() << endl;
}

void build_path(vector<vector<int>>& childs, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<vector<int>>& solution, vector<ld>& ini_delays, vector<vector<int>>& origins, ld var_limit, vector<ld>& shortest_var)
{
	//todo:ship0 和 ship11同时考虑，交替进行
	
	vector<pair<int, int>> sorted_edges_posi;
	vector<pair<int, int>> sorted_edges_nege;


	for (int node = 1; node <= 10000; ++node)
	{
		sort(childs[node].begin(), childs[node].end(), [&](int ca, int cb) {return vars[node][ca] < vars[node][cb]; });
	}


	sort_edges(true, childs, means, vars, sorted_edges_posi);
	sort_edges(false, childs, means, vars, sorted_edges_nege);


	vector<int> rec_is_in_path(10001, -1);    //表示一个点是否被包含在路径中，取值可能是-1，0，11
	vector<int> rec_out_going(10001, 0), rec_in_coming(10001, 0);


	ld delay_0 = ini_delays[0], delay_11 = ini_delays[11];
	ld var_0 = 0, var_11 = 0;

	vector<pair<int, int>> rec_partial_paths_s0, rec_partial_paths_s11;

	select_edges(delay_0, var_0, delay_11, var_11, -0.6, 0.6, rec_partial_paths_s0, rec_partial_paths_s11, means, vars, sorted_edges_posi, sorted_edges_nege, rec_is_in_path, rec_out_going, rec_in_coming);

	
	cout << "ship 0 " << rec_partial_paths_s0.size() << " partial paths " << endl;
	cout << "ship 11 " << rec_partial_paths_s11.size() << " partial paths " << endl;

	//todo

	ld best_start_var = 10, best_start_mean = 0;
	vector<int> best_start_path;
	for (auto origin : origins[0])
	{
		auto partial_paths = rec_partial_paths_s0;

		auto out_going = rec_out_going, in_coming = rec_in_coming;
		auto is_in_path = rec_is_in_path;

		ld res_var = 10, res_mean = 0;
		vector<int> res_path;
		for (auto p_p : partial_paths)
		{
			//cout << "to find " << origin << "  to  " << p_p.first << endl;
			connect_nodes(0, origin, p_p.first, childs, means, vars, is_in_path, 5, res_path, res_var, res_mean);
		}
		//cout << "res var: " << res_var << endl;
		if (res_var < best_start_var)
		{
			best_start_var = res_var;
			best_start_path = res_path;
			best_start_mean = res_mean;
		}
	}

	//把头加进来
	int start_des = best_start_path.back();
	for (int i = 0; i < rec_partial_paths_s0.size(); ++i)
	{
		if (rec_partial_paths_s0[i].first == start_des)
		{
			rec_partial_paths_s0[i].first = best_start_path[0];
			if (i > 0)
			{
				swap(rec_partial_paths_s0[i], rec_partial_paths_s0[0]);
			}
			break;
		}
	}
	//修改拓扑
	for (int i = 1; i < best_start_path.size(); ++i)
	{
		int node = best_start_path[i], prenode = best_start_path[i - 1];
		rec_in_coming[node] = prenode;
		rec_out_going[prenode] = node;
		rec_is_in_path[prenode] = 0;
	}

	delay_0 += best_start_mean;
	var_0 += best_start_var;
	//cout << "after var  " << var << "  mean:  " << delay << endl;

	//连接ship11的头部
	best_start_var = 10, best_start_mean = 0;
	best_start_path.clear();
	for (auto origin : origins[11])
	{
		auto partial_paths = rec_partial_paths_s11;

		auto out_going = rec_out_going, in_coming = rec_in_coming;
		auto is_in_path = rec_is_in_path;

		ld res_var = 10, res_mean = 0;
		vector<int> res_path;
		for (auto p_p : partial_paths)
		{
			//cout << "to find " << origin << "  to  " << p_p.first << endl;
			connect_nodes(11, origin, p_p.first, childs, means, vars, is_in_path, 5, res_path, res_var, res_mean);
		}
		//cout << "res var: " << res_var << endl;
		if (res_var < best_start_var)
		{
			best_start_var = res_var;
			best_start_path = res_path;
			best_start_mean = res_mean;
		}
	}
	start_des = best_start_path.back();
	for (int i = 0; i < rec_partial_paths_s11.size(); ++i)
	{
		if (rec_partial_paths_s11[i].first == start_des)
		{
			rec_partial_paths_s11[i].first = best_start_path[0];
			if (i > 0)
			{
				swap(rec_partial_paths_s11[i], rec_partial_paths_s11[0]);
			}
			break;
		}
	}
	//修改拓扑
	for (int i = 1; i < best_start_path.size(); ++i)
	{
		int node = best_start_path[i], prenode = best_start_path[i - 1];
		rec_in_coming[node] = prenode;
		rec_out_going[prenode] = node;
		rec_is_in_path[prenode] = 11;
	}

	delay_11 += best_start_mean;
	var_11 += best_start_var;

	//todo
	//再来连接 ship0 尾部105
	auto is_in_path = rec_is_in_path;
	ld res_var = 10, res_mean = 0;
	vector<int> res_path;
	for (int i = 1; i<rec_partial_paths_s0.size(); ++i)
	{
		//cout << "to find " << origin << "  to  " << p_p.first << endl;
		auto& p_p = rec_partial_paths_s0[i];
		connect_nodes(0, p_p.second, 105, childs, means, vars, is_in_path, 5, res_path, res_var, res_mean);
	}

	int start = res_path[0];
	for (int i = 1; i < rec_partial_paths_s0.size(); ++i)
	{
		if (rec_partial_paths_s0[i].second == start)
		{
			rec_partial_paths_s0[i].second = 105;
			if (i != rec_partial_paths_s0.size() - 1)
			{
				swap(rec_partial_paths_s0[i], rec_partial_paths_s0[rec_partial_paths_s0.size() - 1]);
			}
			break;
		}
	}
	//修改拓扑
	for (int i = 1; i < res_path.size(); ++i)
	{
		int node = res_path[i], prenode = res_path[i - 1];
		rec_in_coming[node] = prenode;
		rec_out_going[prenode] = node;
		rec_is_in_path[node] = 0;
	}

	delay_0 += res_mean;
	var_0 += res_var;


	//ship 11尾部
	is_in_path = rec_is_in_path;
	res_var = 10, res_mean = 0;
	res_path.clear();
	for (int i = 1; i < rec_partial_paths_s11.size(); ++i)
	{
		//cout << "to find " << origin << "  to  " << p_p.first << endl;
		auto& p_p = rec_partial_paths_s11[i];
		connect_nodes(11, p_p.second, 105, childs, means, vars, is_in_path, 5, res_path, res_var, res_mean);
	}

	start = res_path[0];
	for (int i = 1; i < rec_partial_paths_s11.size(); ++i)
	{
		if (rec_partial_paths_s11[i].second == start)
		{
			rec_partial_paths_s11[i].second = 105;
			if (i != rec_partial_paths_s11.size() - 1)
			{
				swap(rec_partial_paths_s11[i], rec_partial_paths_s11[rec_partial_paths_s11.size() - 1]);
			}
			break;
		}
	}
	//修改拓扑
	for (int i = 1; i < res_path.size(); ++i)
	{
		int node = res_path[i], prenode = res_path[i - 1];
		rec_in_coming[node] = prenode;
		rec_out_going[prenode] = node;
		rec_is_in_path[node] = 11;
	}
	delay_11 += res_mean;
	var_11 += res_var;
	/*cout << "after 0 var  " << var_0 << "  mean:  " << delay_0 << endl;
	cout << "after 11 var  " << var_11 << "  mean:  " << delay_11 << endl;*/

	while (rec_partial_paths_s0.size() > 2 || rec_partial_paths_s11.size() > 2)
	{
		//每次选两条，头尾相连
		//todo
		bool flag = false;
		if (rec_partial_paths_s0.size() > 2)
		{
			int pre_index = -1, next_index = -1;
			ld best_var = 10, best_delay = 0;
			ld rec_var = 10;
			vector<int> best_path;

			for (int pre = 0; pre < rec_partial_paths_s0.size() - 1; ++pre)
			{
				for (int next = 1; next < rec_partial_paths_s0.size(); ++next)
				{
					if (pre == 0 && next == rec_partial_paths_s0.size() - 1 || pre == next)
						continue;
					vector<int> is_in_path = rec_is_in_path;
					connect_nodes(0, rec_partial_paths_s0[pre].second, rec_partial_paths_s0[next].first, childs, means, vars, is_in_path, 5, best_path, best_var, best_delay);
					if (rec_var > best_var)
					{
						rec_var = best_var;
						pre_index = pre;
						next_index = next;
					}
				}
			}

			if (best_path.size() == 0)
			{
				cout << "no connection" << endl;

			}
			else
				flag = true;

			if (best_path.size() > 0)
			{
				rec_partial_paths_s0[pre_index].second = rec_partial_paths_s0[next_index].second;
				//先改拓扑
				for (int i = 1; i < best_path.size(); ++i)
				{
					int node = best_path[i], prenode = best_path[i - 1];
					rec_in_coming[node] = prenode;
					rec_out_going[prenode] = node;
					rec_is_in_path[node] = 0;
				}

				if (rec_partial_paths_s0[next_index].second == 105)
				{
					swap(rec_partial_paths_s0[pre_index], rec_partial_paths_s0[next_index]);
					swap(pre_index, next_index);
				}

				rec_partial_paths_s0.erase(rec_partial_paths_s0.begin() + next_index);
				//cout << "increase var: " << best_var << endl;

				var_0 += best_var;
				delay_0 += best_delay;
				//cout << "current var: " << var << endl;
			}
		}


		if (rec_partial_paths_s11.size() > 2)
		{
			int pre_index = -1, next_index = -1;
			ld best_var = 10, best_delay = 0;
			ld rec_var = 10;
			vector<int> best_path;

			for (int pre = 0; pre < rec_partial_paths_s11.size() - 1; ++pre)
			{
				for (int next = 1; next < rec_partial_paths_s11.size(); ++next)
				{
					if (pre == 0 && next == rec_partial_paths_s11.size() - 1 || pre == next)
						continue;
					vector<int> is_in_path = rec_is_in_path;
					connect_nodes(11, rec_partial_paths_s11[pre].second, rec_partial_paths_s11[next].first, childs, means, vars, is_in_path, 5, best_path, best_var, best_delay);
					if (rec_var > best_var)
					{
						rec_var = best_var;
						pre_index = pre;
						next_index = next;
					}
				}
			}

			if (best_path.size() == 0)
			{
				cout << "no connection" << endl;

			}
			else
				flag = true;

			if (best_path.size() > 0)
			{
				rec_partial_paths_s11[pre_index].second = rec_partial_paths_s11[next_index].second;
				//先改拓扑
				for (int i = 1; i < best_path.size(); ++i)
				{
					int node = best_path[i], prenode = best_path[i - 1];
					rec_in_coming[node] = prenode;
					rec_out_going[prenode] = node;
					rec_is_in_path[node] = 11;
				}

				if (rec_partial_paths_s11[next_index].second == 105)
				{
					swap(rec_partial_paths_s11[pre_index], rec_partial_paths_s11[next_index]);
					swap(pre_index, next_index);
				}

				rec_partial_paths_s11.erase(rec_partial_paths_s11.begin() + next_index);
				//cout << "increase var: " << best_var << endl;

				var_11 += best_var;
				delay_11 += best_delay;
				//cout << "current var: " << var << endl;
			}
		}

		if (!flag)
			break;
	}

	//最后把头尾两个partial_path相连
	is_in_path = rec_is_in_path;
	res_var = 10, res_mean = 0;
	res_path.clear();


	connect_nodes(0, rec_partial_paths_s0[0].second, rec_partial_paths_s0[1].first, childs, means, vars, is_in_path, 5, res_path, res_var, res_mean);
	rec_partial_paths_s0[0].second = 105;

	for (int i = 1; i < res_path.size(); ++i)
	{
		int node = res_path[i], prenode = res_path[i - 1];
		rec_in_coming[node] = prenode;
		rec_out_going[prenode] = node;
		rec_is_in_path[node] = 0;
	}
	var_0 += res_var;
	delay_0 += res_mean;




	cout << "ship 0" << endl;
	cout << "final var: " << var_0 << endl;
	cout << "final delay: " << delay_0 << endl;

	solution[0].clear();
	
	int node = rec_partial_paths_s0[0].first;
	while (node != rec_partial_paths_s0[0].second)
	{
		solution[0].emplace_back(node);
		node = rec_out_going[node];
	}
	solution[0].emplace_back(node);



	//ship11
	is_in_path = rec_is_in_path;
	res_var = 10, res_mean = 0;
	res_path.clear();


	connect_nodes(11, rec_partial_paths_s11[0].second, rec_partial_paths_s11[1].first, childs, means, vars, is_in_path, 5, res_path, res_var, res_mean);
	rec_partial_paths_s11[0].second = 105;

	for (int i = 1; i < res_path.size(); ++i)
	{
		int node = res_path[i], prenode = res_path[i - 1];
		rec_in_coming[node] = prenode;
		rec_out_going[prenode] = node;
		rec_is_in_path[node] = 11;
	}
	var_11 += res_var;
	delay_11 += res_mean;




	cout << "ship 11" << endl;
	cout << "final var: " << var_11 << endl;
	cout << "final delay: " << delay_11 << endl;

	solution[11].clear();

	node = rec_partial_paths_s11[0].first;
	while (node != rec_partial_paths_s11[0].second)
	{
		solution[11].emplace_back(node);
		node = rec_out_going[node];
	}
	solution[11].emplace_back(node);
}




void sort_edges(bool posi, vector<vector<int>>& childs, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<pair<int, int>>& sorted_edges)
{
	sorted_edges.reserve(1000000);
	for (int node = 1; node <= 10000; ++node)
	{
		if (node == 105)
			continue;

		for (auto child : childs[node])
		{
			if (posi && means[node][child] < 0 || !posi && means[node][child] > 0)
				continue;
			sorted_edges.emplace_back(node, child);
		}
	}
	if(posi)
		sort(sorted_edges.begin(), sorted_edges.end(), [&](const pair<int, int>& pa, const pair<int, int>& pb) { return means[pa.first][pa.second]/vars[pa.first][pa.second] > means[pb.first][pb.second]/vars[pb.first][pb.second]; });
	else
		sort(sorted_edges.begin(), sorted_edges.end(), [&](const pair<int, int>& pa, const pair<int, int>& pb) { return means[pa.first][pa.second]/vars[pa.first][pa.second] < means[pb.first][pb.second]/vars[pb.first][pb.second]; });
}


void select_edges(ld& delay_0, ld& var_0, ld& delay_11, ld& var_11, ld threshold_0, ld threshold_11, vector<pair<int, int>>& partial_paths_s0, vector<pair<int, int>>& partial_paths_s11, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<pair<int, int>>& sorted_edges_posi, vector<pair<int, int>>& sorted_edges_nege, vector<int>& is_in_path, vector<int>& out_going, vector<int>& in_coming)
{
	int pos_0 = 0, pos_11 = 0;
	int cnt = 0;

	while (delay_11 > threshold_11  && pos_11 < sorted_edges_nege.size() || delay_0 < threshold_0 && pos_0 < sorted_edges_posi.size())
	{
		if (delay_0 < threshold_0)
		{
			while (pos_0 < sorted_edges_posi.size())
			{
				int node = sorted_edges_posi[pos_0].first, child = sorted_edges_posi[pos_0].second;
				if (is_in_path[node] != 11 && is_in_path[child] != 11 && out_going[node] == 0 && in_coming[child] == 0)
				{
					if (out_going[child] > 0)
					{
						int check_node = child;
						bool loop = false;
						//检查是否会构成环
						while (check_node != 0)
						{
							check_node = out_going[check_node];
							if (check_node == node)
							{
								loop = true;
								break;
							}
						}
						if (loop)
						{
							++pos_0;
							continue;
						}
					}

					out_going[node] = child;
					in_coming[child] = node;

					is_in_path[node] = 0;
					is_in_path[child] = 0;
			
					delay_0 += means[node][child];
					var_0 += vars[node][child];


					++pos_0;
					break;
				}
				++pos_0;
			}
		}
		if (delay_11 > threshold_11)
		{
			while (pos_11 < sorted_edges_nege.size())
			{
				int node = sorted_edges_nege[pos_11].first, child = sorted_edges_nege[pos_11].second;
				if (is_in_path[node] != 0 && is_in_path[child] != 0 && out_going[node] == 0 && in_coming[child] == 0)
				{
					if (out_going[child] > 0)
					{
						int check_node = child;
						bool loop = false;
						//检查是否会构成环
						while (check_node != 0)
						{
							check_node = out_going[check_node];
							if (check_node == node)
							{
								loop = true;
								break;
							}
						}
						if (loop)
						{
							++pos_11;
							continue;
						}
					}

					out_going[node] = child;
					in_coming[child] = node;

					is_in_path[node] = 11;
					is_in_path[child] = 11;

					delay_11 += means[node][child];
					var_11 += vars[node][child];


					++pos_11;
					break;
				}
				++pos_11;
			}
		}
	}
	//cout << pos << endl;
	//cout << cnt << endl;
	//cout << var << endl;
	vector<bool> checked(10001, false);

	for (int node = 1; node <= 10000; ++node)
	{
		if (checked[node] || in_coming[node] != 0)
			continue;

		if (out_going[node] != 0)
		{
			int check_node = out_going[node];
			checked[node] = true;
			int tail = node;
			int len = 1;
			while (check_node != 0)
			{
				checked[check_node] = true;
				tail = check_node;
				++len;
				check_node = out_going[check_node];
			}
			//cout << "len: " << len << endl;
			if (is_in_path[node] == 0)
				partial_paths_s0.emplace_back(node, tail);
			else
				partial_paths_s11.emplace_back(node, tail);
		}
		
	}
	cout << "ship0" << endl;
	cout << "delay " << delay_0 << "  var " << var_0 << endl;

	cout << "ship11" << endl;
	cout << "delay " << delay_11 << "  var " << var_11 << endl;
}

void connect_nodes(int ship, int start, int destination, vector<vector<int>>& childs, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<int>& is_in_path, int hops_limit, vector<int>& res_path, ld& res_var, ld& res_mean)
{
	vector<int> now_in_path = is_in_path;

	vector<int> path;
	path.reserve(15);

	path.emplace_back(start);

	vector<int> best_path;
	ld best_var = 10, best_mean = 0;
	now_in_path[start] = true;
	dfs_connect(ship, destination, childs, means, vars, now_in_path, path, 0, start, 0, best_path, best_var, best_mean, 0, hops_limit);
	if (best_var < res_var)
	{
		res_path = best_path;
		res_var = best_var;
		res_mean = best_mean;
	}

	if (res_var > 1)
	{
		path.clear();
		bfs_connect(ship, start, destination, childs, means, vars, is_in_path, path);
		if (path.size() != 0)
		{
			best_path = path;
			best_var = 0, best_mean = 0;
			for (int i = 1; i < path.size(); ++i)
			{
				int node = path[i], prenode = path[i - 1];
				best_var += vars[prenode][node];
				best_mean += means[prenode][node];
			}
			if (best_var < res_var)
			{
				res_path = best_path;
				res_var = best_var;
				res_mean = best_mean;
			}
		}
	}
}

void dfs_connect(int ship, int destination, vector<vector<int>>& childs, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<int>& is_in_path, vector<int>& path, ld current_delay, int node, ld current_var, vector<int>& best_path, ld& best_var, ld& best_mean, int hops, int hops_limit)
{
	if (node == destination)
	{
		if (current_var < best_var)
		{
			best_path = path;
			best_var = current_var;
			best_mean = current_delay;
			//cout << "found best: " << best_var << endl;
		}
		return;
	}
	if (hops >= hops_limit || current_var > best_var)
	{
		return;
	}

	if (destination == 105 && vars[node][105] > 0)
	{
		path.emplace_back(105);
		is_in_path[105] = ship;
		dfs_connect(ship, destination, childs, means, vars, is_in_path, path, current_delay + means[node][105], 105, current_var + vars[node][105], best_path, best_var, best_mean, hops + 1, hops_limit);
		path.erase(path.end() - 1);
		is_in_path[105] = -1;

	}

	int cnt = 0;
	for (auto child : childs[node])
	{
		if (cnt >= 20)
			break;
		
		if (is_in_path[child] >= 0 && child != destination)
			continue;

		path.emplace_back(child);
		is_in_path[child] = ship;
		dfs_connect(ship, destination, childs, means, vars, is_in_path, path, current_delay + means[node][child], child, current_var + vars[node][child], best_path, best_var, best_mean, hops + 1, hops_limit);
		path.erase(path.end() - 1);
		is_in_path[child] = -1;
		++cnt;
	}

	return;
}

void bfs_connect(int ship, int start, int destination, vector<vector<int>>& childs, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<int>& is_in_path, vector<int>& path)
{
	vector<int> current_layer, next_layer;
	vector<int> pres(10001, 0);
	vector<int> bfs_in_path = is_in_path;
	next_layer.emplace_back(start);
	bfs_in_path[start] = ship;
	int cnt = 0;
	bool flag = false;
	while (!next_layer.empty())
	{
		++cnt;

		current_layer = move(next_layer);
		next_layer.clear();
		next_layer.reserve(10000);
		//cout << "current size: " << current_layer.size() << endl;
		for (auto node : current_layer)
		{
			if (node == destination)
			{
				next_layer.clear();
				flag = true;
				//cout << "found" << endl;
				break;
			}
			for (auto child : childs[node])
			{
				if (bfs_in_path[child] >= 0 && child != destination)
					continue;

				next_layer.emplace_back(child);
				pres[child] = node;
				bfs_in_path[child] = ship;
			}
		}
	}
	if (flag)
	{
		int node = destination;
		while (node != start)
		{
			path.emplace_back(node);
			node = pres[node];
		}
		path.emplace_back(start);
		reverse(path.begin(), path.end());
	}
	

	return;

}

void modify(vector<vector<int>>& solution, vector<vector<int>>& childs, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<ld>& ini_delays, vector<bool>& is_visited)
{
	//273.941最大在ship11,  237.884最大在ship0
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	int turns = 20;
	int max_gap = 10;
	//for 273.941
	ld modi_value = 0;
	int length = solution[11].size();
	std::uniform_int_distribution<int> distribution(1, length - 2);
	for (int turn = 0; turn < turns; ++turn)
	{
		//cout << "a turn" << endl;
		//如果var变小了,break
		int pos1 = distribution(generator);
		ld old_var = vars[solution[11][pos1]][solution[11][pos1 + 1]];
		ld old_mean = means[solution[11][pos1]][solution[11][pos1 + 1]];
		bool improved = false;

		for (int gap = 1; gap <= max_gap; ++gap)
		{
			int pos2 = pos1 + gap + 1;

			if (pos2 >= length)
				break;

			old_var += vars[solution[11][pos2 - 1]][solution[11][pos2]];
			old_mean += means[solution[11][pos2 - 1]][solution[11][pos2]];
			//重新规划pos1到pos2的
			vector<bool> search_visited = is_visited;
			
			for (int pos = pos1 + 1; pos < pos2; ++pos)
			{
				int node = solution[11][pos];
				search_visited[node] = false;
			}

			ld new_var = old_var, new_delay = 0;
			vector<int> partial_path;
			vector<int> best_path;
			partial_path.emplace_back(solution[11][pos1]);

			//以solution[11][pos2]为destination

			int destination = solution[11][pos2];

			search_partial(solution[11][pos1], destination, childs, means, vars, search_visited, partial_path, 0, 0, old_mean, best_path, new_var, new_delay, 0, gap + 1);

			if (best_path.size() > 0)
			{
				//替换
				modi_value = old_mean - new_delay;
				//solution[11]从pos1到pos2全删掉

				for (int pos = pos1; pos <= pos2; ++pos)
				{
					int node = solution[11][pos];

					is_visited[node] = false;
				}
				solution[11].erase(solution[11].begin() + pos1, solution[11].begin() + pos2 + 1);
				solution[11].insert(solution[11].begin() + pos1, best_path.begin(), best_path.end());
				for (auto node : best_path)
				{
					is_visited[node] = true;
				}
				improved = true;
				break;
			}
			
		}



		if (improved && modi_value > 1.8e-3)
			break;

	}


	//for 273.884
	//int length = solution[0].size();
	//std::uniform_int_distribution<int> distribution(1, length - 2);

	//for (int turn = 0; turn < turns; ++turn)
	//{
	//	int pos1 = distribution(generator);
	//	ld old_var = vars[solution[0][pos1]][solution[0][pos1 + 1]];
	//	ld old_mean = means[solution[0][pos1]][solution[0][pos1 + 1]];
	//	bool improved = false;

	//	for (int gap = 1; gap <= max_gap; ++gap)
	//	{
	//		int pos2 = pos1 + gap + 1;

	//		if (pos2 >= length)
	//			break;

	//		old_var += vars[solution[0][pos2 - 1]][solution[0][pos2]];
	//		old_mean += means[solution[0][pos2 - 1]][solution[0][pos2]];
	//		//重新规划pos1到pos2的
	//		vector<bool> search_visited = is_visited;

	//		for (int pos = pos1 + 1; pos < pos2; ++pos)
	//		{
	//			int node = solution[0][pos];
	//			search_visited[node] = false;
	//		}

	//		ld new_var = old_var, new_delay = 0;
	//		vector<int> partial_path;
	//		vector<int> best_path;
	//		partial_path.emplace_back(solution[0][pos1]);

	//		//以solution[11][pos2]为destination

	//		int destination = solution[0][pos2];

	//		search_partial(solution[0][pos1], destination, childs, means, vars, search_visited, partial_path, 0, 0, old_mean, best_path, new_var, new_delay, 0, gap + 1);

	//		if (best_path.size() > 0)
	//		{
	//			//替换

	//			//solution[0]从pos1到pos2全删掉
	//			solution[0].erase(solution[0].begin() + pos1, solution[0].begin() + pos2 + 1);
	//			solution[0].insert(solution[0].begin() + pos1, best_path.begin(), best_path.end());

	//			improved = true;
	//			break;
	//		}
	//	}
	//	if (improved)
	//		break;
	//}

}

void search_partial(int node, int destination, vector<vector<int>>& childs, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<bool>& is_visited, vector<int>& path, ld current_delay, ld current_var, ld old_mean, vector<int>& best_path, ld& best_var, ld& best_mean, int hops, int hops_limit)
{
	if (node == destination)
	{
		if (current_var < best_var + 2e-6 && current_delay < old_mean)
		{
			best_path = path;
			best_var = current_var;
			best_mean = current_delay;
		}
		return;
	}
	if (hops > hops_limit || current_var > best_var + 2e-6)
	{
		return;
	}
	
	for (auto child : childs[node])
	{

		if (is_visited[child]  && child != destination)
			continue;

		path.emplace_back(child);
		is_visited[child] = true;
		search_partial(child, destination, childs, means, vars, is_visited, path, current_delay + means[node][child], current_var + vars[node][child], old_mean, best_path, best_var, best_mean, hops + 1, hops_limit);
		path.erase(path.end() - 1);
		is_visited[child] = false;

	}


}

void re_build_11_path(vector<vector<int>>& solution, vector<vector<int>>& childs, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<ld>& ini_delays, vector<bool>& is_visited, vector<vector<int>>& origins)
{
	//挑选一些性价比高的，然后连起来
	for (auto node : solution[11])
	{
		is_visited[node] = false;
	}

	solution[11].clear();
	vector<pair<int, int>> sorted_edges;

	sorted_edges.reserve(1000000);
	for (int node = 1; node <= 10000; ++node)
	{
		if (node == 105 || is_visited[node])
			continue;

		for (auto child : childs[node])
		{
			if (means[node][child] > 0 || is_visited[child])
				continue;
			sorted_edges.emplace_back(node, child);
		}
	}
	sort(sorted_edges.begin(), sorted_edges.end(), [&](const pair<int, int>& pa, const pair<int, int>& pb) { return means[pa.first][pa.second] / vars[pa.first][pa.second] < means[pb.first][pb.second] / vars[pb.first][pb.second]; });

	ld ship_0_delay = ini_delays[0];
	for (int pos = 1; pos < solution[0].size(); ++pos)
	{
		int prenode = solution[0][pos - 1], node = solution[0][pos];
		ship_0_delay += means[prenode][node];
	}

	ld target_delay = 1 + ship_0_delay;

	vector<int> rec_is_in_path(10001, -1);
	for (int node = 1; node <= 10000; ++node)
	{
		if (is_visited[node])
			rec_is_in_path[node] = 1;
	}

	//ship11要小于target_delay

	ld ship_11_delay = ini_delays[11];

	vector<int> rec_outgoing(10001, 0), rec_incoming(10001, 0);	
	vector<pair<int, int>> rec_partial_paths;
	ld ship_11_var = 0;
	int pos = 0;
	int hops = 0;
	while (ship_11_delay > target_delay && pos < sorted_edges.size() && hops < 170)
	{
		int node = sorted_edges[pos].first, child = sorted_edges[pos].second;
		if (rec_is_in_path[node] != 1 && rec_is_in_path[child] != 1 && rec_outgoing[node] == 0 && rec_incoming[child] == 0)
		{
			ship_11_delay += means[node][child];
			rec_outgoing[node] = child;
			rec_incoming[child] = node;
			++hops;
			ship_11_var += vars[node][child];
			rec_partial_paths.emplace_back(pair<int, int>{node, child});
			rec_is_in_path[node] = 11;
			rec_is_in_path[child] = 11;
		}
		++pos;
	}
	cout << "ship11 delay: " << ship_11_delay <<"  "<<ship_11_delay - ship_0_delay << endl;
	cout << "var: " << ship_11_var << endl;
	cout << "hops: " << hops << endl;

	

	ld best_start_var = 10, best_start_mean = 0;
	vector<int> best_start_path;
	for (auto origin : origins[11])
	{
		auto partial_paths = rec_partial_paths;

		auto out_going = rec_outgoing, in_coming = rec_incoming;
		auto is_in_path = rec_is_in_path;

		ld res_var = 10, res_mean = 0;
		vector<int> res_path;
		for (auto p_p : partial_paths)
		{
			//cout << "to find " << origin << "  to  " << p_p.first << endl;
			connect_nodes(11, origin, p_p.first, childs, means, vars, is_in_path, 5, res_path, res_var, res_mean);
		}
		//cout << "res var: " << res_var << endl;
		if (res_var < best_start_var)
		{
			best_start_var = res_var;
			best_start_path = res_path;
			best_start_mean = res_mean;
		}
	}

	int start_des = best_start_path.back();
	for (int i = 0; i < rec_partial_paths.size(); ++i)
	{
		if (rec_partial_paths[i].first == start_des)
		{
			rec_partial_paths[i].first = best_start_path[0];
			if (i > 0)
			{
				swap(rec_partial_paths[i], rec_partial_paths[0]);
			}
			break;
		}
	}
	//修改拓扑
	for (int i = 1; i < best_start_path.size(); ++i)
	{
		int node = best_start_path[i], prenode = best_start_path[i - 1];
		rec_incoming[node] = prenode;
		rec_outgoing[prenode] = node;
		rec_is_in_path[prenode] = 11;
	}

	ship_11_delay += best_start_mean;
	ship_11_var += best_start_var;

	auto is_in_path = rec_is_in_path;
	ld res_var = 10, res_mean = 0;
	vector<int> res_path;
	for (int i = 1; i < rec_partial_paths.size(); ++i)
	{
		//cout << "to find " << origin << "  to  " << p_p.first << endl;
		auto& p_p = rec_partial_paths[i];
		connect_nodes(11, p_p.second, 105, childs, means, vars, is_in_path, 5, res_path, res_var, res_mean);
	}

	int start = res_path[0];
	for (int i = 1; i < rec_partial_paths.size(); ++i)
	{
		if (rec_partial_paths[i].second == start)
		{
			rec_partial_paths[i].second = 105;
			if (i != rec_partial_paths.size() - 1)
			{
				swap(rec_partial_paths[i], rec_partial_paths[rec_partial_paths.size() - 1]);
			}
			break;
		}
	}
	//修改拓扑
	for (int i = 1; i < res_path.size(); ++i)
	{
		int node = res_path[i], prenode = res_path[i - 1];
		rec_incoming[node] = prenode;
		rec_outgoing[prenode] = node;
		rec_is_in_path[node] = 11;
	}
	ship_11_delay += res_mean;
	ship_11_var += res_var;

	cout << "after delay: " << ship_11_delay << endl;
	cout << "var: " << ship_11_var << endl;




}
