#include"header.h"
#include<fstream>
#include<string>
#include<sstream>
#include<iomanip>
#include<iostream>


void read_edge_and_meanvar(vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<vector<int>>& neighbors)
{
	ld mean, var;
	ifstream infile_meanvar("../data/meanvar.txt"), infile_edge("../data/edges.txt");
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
	ifstream infile("../data/origins.txt");
	string linestr;
	//每个飞船有14个候选起点
	for (int i = 0; i < 12; ++i)
	{
		getline(infile, linestr);

		int pos = 0, nextpos = -1;
		while ((nextpos = linestr.find(" ", pos)) != -1)
		{
			int node = stoi(linestr.substr(pos, nextpos - pos));
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
	ifstream infile("../data/delays.txt");

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
	ifstream infile("../data/shortest_path.txt");
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


void read_ini_solution(vector<vector<int>>& solution, vector<vector<ld>>& visited, vector<ld>& ini_delays, vector<vector<ld>>& means)
{
	ifstream infile("../data/best-ini.txt");
	for (int i = 0; i < 12; ++i)
	{
		int node = 0;
		ld delay = ini_delays[i];
		infile >> node;
		if (i == 0 || i == 11)
		{
			visited[node].emplace_back(delay);
			solution[i].emplace_back(node);
		}
		int prenode = node;
		while (node != 105)
		{
			infile >> node;

			delay += means[prenode][node];
			if (i == 0 || i == 11)
			{
				visited[node].emplace_back(delay);
				solution[i].emplace_back(node);
			}
			prenode = node;
		}
	}
	infile.close();
}

void read_0_11_path(vector<vector<int>>& solution, vector<vector<ld>>& visited, vector<ld>& ini_delays, vector<vector<ld>>& means)
{
	ifstream infile("../data/ship-0-11.txt");
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


void check_solution(vector<vector<int>>& solution, vector<vector<ld>>& visited, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<ld>& ini_delays)
{
	//看一下每个ship有多少个点是重复的，重新计算一次var
	int repeat_cnt = 0;
	for (int ship = 0; ship < 12; ship += 11)
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
			int k = 0;
			for (auto visit_time : visited[node])
			{
				if (visit_time < current_delay)
					++k;
			}
			if (visited[node].size() == 1)
				k = 0;
			if (vars[node][route[j]] > 1e-4)
				++cnt;
			if (visited[node].size() > 1 && ship == 0)
				++repeat_cnt;
			
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

	cout << "total repeat: " << repeat_cnt << endl;
}


void write_solution(vector<vector<int>>& solution)
{
	ofstream outfile("../data/ship-0-11.txt");
	for (int ship = 0; ship < 12; ship+=11)
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
	int search_width = 6;
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


			cout << "start search ship0  old var: "<<ship_0_old_var <<"  " << ship_0_left_node << " to " << ship_0_right_node << endl;
			dfs_partial(0, ship_0_right_node, childs, partial_paths, solution[11], ship_0_old_mean, means, vars, ship_0_path, 0, ship_0_left_node, 0, ship_0_best_path, ship_0_best_var, shortest_var);

			cout << ship_0_best_var << endl;


			//搜ship11
			partial_paths.clear();
			partial_paths[0].assign(solution[11].begin(), solution[11].begin() + ship_11_left_index);

			partial_paths[1].assign(solution[11].begin() + ship_11_right_index, solution[11].end());


			vector<int> ship_11_best_path, ship_11_path;


			ship_11_path.emplace_back(ship_11_left_node);


			cout << "start search ship11  old var: " << ship_11_old_var << "  " << ship_11_left_node << " to " << ship_11_right_node << endl;
			//if(!(ship_11_right_node == 105 && abs(ship_11_old_var - shortest_var[ship_11_left_node]) < 1e-6))
			dfs_partial(11, ship_11_right_node, childs, partial_paths, solution[0], ship_11_old_mean, means, vars, ship_11_path, 0, ship_11_left_node, 0, ship_11_best_path, ship_11_best_var, shortest_var);

			cout << ship_11_best_var << endl;


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

	int search_depth = 2;

	ld ship_0_mean = 0, ship_0_var = 0, ship_11_mean = 0, ship_11_var = 0;


	//统计一遍旧的
	ship_0_mean = ini_delays[0];
	for (int i = 1; i < ship_0_path.size(); ++i)
	{
		int node = ship_0_path[i], prenode = ship_0_path[i - 1];
		ship_0_mean += means[prenode][node];
		ship_0_var += vars[prenode][node];
	}

	ship_11_mean = ini_delays[1];
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


		for (int i = 1; ship_0_path[i] != 105; ++i)
		{
			int prenode = ship_0_path[i - 1], node = ship_0_path[i];

			ld old_mean = means[prenode][node];

			vector<int> path;
			ld best_var = 1, path_mean = 0;
			path.emplace_back(prenode);
			//做一个dfs，到达node时的深度不能为1，不超过search_depth,选择一个mean符合ship要求的，var最小的



			//检查是否替换new_path


		}

		//new_path有没有内容
		if (insert_pos > 0)
		{
			
		}
		else
		{
			//找不到路了
			
			break;
		}



	}

	//处理ship11
	while (ship_11_mean > right_time)
	{
		
	}

}