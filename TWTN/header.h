#pragma once
#include<vector>
#include<string>
using ld = long double;
extern int des;
extern int jump_limit;
extern ld window;
extern int maxnode;


using namespace std;

void read_edge_and_meanvar(vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<vector<int>>& neighbors);

void read_origin(vector<vector<int>>& origins);

void read_edge(vector<vector<int>>& neighbors);

void read_ini_delays(vector<ld>& ini_delays);

void read_shortest(vector<vector<int>>& shortest_path, vector<ld>& shortest_mean, vector<ld>& shortest_var);

void bfs(vector<vector<int>>& childs, vector<vector<int>>& origins);

void find_path(vector<vector<int>>& childs, int origin, vector<vector<ld>>& means, vector<vector<ld>>& vars, ld ini_delay, ld tw_start);

bool is_in_time_window(ld time);

void read_ini_solution(vector<vector<int>>& solution, vector<vector<ld>>& visited, vector<ld>& ini_delays, vector<vector<ld>>& means);

void read_0_11_path(vector<vector<int>>& solution, vector<vector<ld>>& visited, vector<ld>& ini_delays, vector<vector<ld>>& means, string filename);

void get_mean_andvar(vector<vector<int>>& childs, vector<vector<ld>>& means, vector<vector<ld>>& vars);

ld cal_path_var(vector<int>& path, vector<vector<ld>>& visited, vector<vector<ld>>& means, vector<vector<ld>>& vars, ld ini_delay);


void gene_solution(int ship, vector<vector<int>>& childs, vector<vector<int>>& solution, vector<vector<ld>>& visited, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<ld>& shortest_mean, vector<ld>& shortest_var, vector<ld>& ini_delays, vector<vector<int>>& shortest_path, vector<vector<int>>& origins, vector<vector<bool>>& dominated);


void check_solution(vector<vector<int>>& solution, vector<vector<ld>>& visited, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<ld>& ini_delays);

void write_solution(vector<vector<int>>& solution, string filename);

void delete_ship_path(int ship, vector<vector<int>>& solution, vector<vector<ld>>& visited, vector<vector<ld>>& means, vector<ld>& ini_delays);

void dfs(vector<vector<int>>& childs, vector<vector<ld>>& visited, ld left_time, ld right_time, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<vector<bool>>& dominated, vector<int>& path, ld current_delay, int node, ld current_var, vector<ld>& shortest_mean, vector<ld>& shortest_var, vector<vector<int>>& shortest_path, vector<int>& best_path, ld& best_var);

void adjust_solution_later(vector<vector<int>>& solution, ld left_time, ld right_time, vector<vector<ld>>& visited, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<ld>& ini_delays, vector<ld>& shortest_mean, vector<ld>& shortest_var, vector<vector<int>>& shortest_path, vector<vector<int>>& origins, vector<vector<int>>& childs, vector<vector<bool>>& dominated);

void adjust_solution_earlier(vector<vector<int>>& solution, ld left_time, ld right_time, vector<vector<ld>>& visited, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<ld>& ini_delays, vector<ld>& shortest_mean, vector<ld>& shortest_var, vector<vector<int>>& shortest_path, vector<vector<int>>& origins, vector<vector<int>>& childs);

void check_domi(vector<vector<int>>& childs, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<vector<bool>>& dominated);


void deal_with_conflict(vector<vector<int>>& solution, vector<vector<ld>>& visited, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<ld>& ini_delays, vector<ld>& shortest_mean, vector<ld>& shortest_var, vector<vector<int>>& shortest_path, vector<vector<int>>& origins, vector<vector<int>>& childs);

void dfs_partial(int ship, int destination, vector<vector<int>>& childs, vector<vector<int>>& partial_paths, vector<int>& other_path, ld old_mean, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<int>& path, ld current_delay, int node, ld current_var, vector<int>& best_path, ld& best_var, vector<ld>& shortest_var);


void refinement(vector<vector<int>>& solution, vector<vector<int>>& childs, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<ld>& ini_delays, ld max_var, ld left_time, ld right_time);

void dfs_refine(int ship, int destination, vector<vector<int>>& childs, vector<int>& ship_0_path, vector<int>& ship_11_path, ld old_mean, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<int>& path, ld current_delay, int node, ld current_var, vector<int>& best_path, ld& best_var, ld& best_path_mean, int search_depth);

void transfer_file(string filename);

void make_tree_var(vector<vector<int>>& childs, vector<ld>& result, vector<vector<ld>>& vars, vector<bool>& is_in_tree, vector<int>& path);

void make_tree_posi(vector<vector<int>>& childs, vector<ld>& result, vector<vector<ld>>& means, vector<bool>& is_in_tree, vector<int>& path);

void make_tree_nege(vector<vector<int>>& childs, vector<ld>& result, vector<vector<ld>>& means, vector<bool>& is_in_tree, vector<int>& path);

void read_bound(string bound_path, vector<ld>& var_bound, vector<ld>& mean_posi_bound, vector<ld>& mean_nege_bound);

void path_find(ld left_time, ld right_time, vector<vector<int>>& childs, vector<vector<int>>& solution, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<ld>& ini_delays, vector<vector<int>>& origins, ld var_limit);

void dfs_path_find(int ship, ld left_time, ld right_time, vector<vector<int>>& childs, vector<vector<ld>>& means, vector<vector<ld>>& vars, vector<int>& path, ld current_delay, int node, ld current_var, vector<ld>& tree_var, vector<ld>& tree_posi, vector<ld>& tree_nege, vector<bool>& is_in_tree, vector<int>& rec_path, ld& rec_var, int depth);