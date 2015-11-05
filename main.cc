/*
Ryan Lee
CS  170 Project 1
November 2015 
		8 puzzle project

		- uniform cost search
		- A* 
		- Manhatten A* 

Notes:

	I use a vector to represent state. For example, 

	v[0] v[1] v[2]

	v[3] v[4] v[5]

	v[6] v[7] v[8]

	Since I use a vector of ints, * cannot be represented as an int, 
	so i simply treat it as 0. 
*/

#include <iostream>
#include <vector> 
#include <algorithm>
#include <map>
#include <queue> 
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

using namespace std; 

struct node 
{
	vector <int> X; 
	int h; 
	int g; 
	int f; 
	int operator< (const node&other) const
		{ return f > other.f; }
}; 

vector <int> puzzle; 
vector <int> goal_state; 
vector <int> empty;  
vector <node> UFC_Tree; 
priority_queue<node> pq;

vector < vector<int> > visited; 

int depth; 
int num_nodes; 
int nodes_in_queue; 

void init_goal_state()
{
	for(unsigned i = 1; i <=8; i++)
		goal_state.push_back(i); 
	goal_state.push_back(0); 
}

void print_puzzle(vector<int> v)
{
	cout << endl; 

	cout << "\t";
	for(unsigned i = 1; i < v.size()+1; i++)
	{
		cout << v[i-1] << " "; 
		if(i % 3 == 0) {
			cout << endl << "\t"; 
		}
	}
	cout << endl; 
}

//return location of 0, which is just *
int star_position(vector <int> v)
{
	for(unsigned i = 0; i < v.size(); i++)
	{
		if(v[i] == 0) return i; 
	}
	return -1; 
}

/*  
	operators
	
	legal operators: 
		swap left
		swap right
		swap down
		swap up
*/

/*
	swap the value position i with the left one. but check to make sure it's legal first... 
	If it is, swap. 
*/
int swap_left(vector <int> & v, int i)
{
	if( (i > 0) && (i <= 8))
	{
		if( (i < 2) || (i > 3 && i < 5) || (i >6))
		{
			swap(v[i], v[i-1]); 
			return 1; 
		}
	}
	return -1; 
}

int swap_right(vector<int> & v, int i)
{
	if( (i != 2) && (i != 5) && (i != 8))
	{
		swap(v[i], v[i+1]);
		return 1; 
	}
	return -1; 
}

int swap_up(vector <int> & v, int i)
{
	if(i>2)
	{
		if(i == 3){
			swap(v[3], v[0]);
			return 1; 
		}
		else if(i == 4){
			swap(v[4], v[1]); 
			return 1; 
		}
		else if(i == 5){
			swap(v[5], v[2]); 
			return 1; 
		}
		else if(i == 6){
			swap(v[6], v[3]); 
			return 1; 
		}
		else if(i == 7) { 
			swap(v[7], v[4]); 
			return 1; 
		}
		else if(i == 8){
			swap(v[8], v[5]); 
			return 1; 
		}
	}
	return -1; 
}

int swap_down(vector<int> & v, int i)
{
	if(i == 0) {
		swap(v[0], v[3]);
		return 1; 
	}
	else if(i == 1){
		swap(v[1], v[4]);
		return 1; 
	}
	else if(i == 2){
		swap(v[2], v[5]);
		return 1; 
	}
	else if(i == 3){
		swap(v[3], v[6]);
		return 1; 
	}
	else if(i == 4){
		swap(v[4], v[7]);
		return 1; 
	}
	else if(i == 5) {
		swap(v[5], v[8]);
		return 1; 
	}
	return -1; 
}

bool check_goal_state(vector <int> & v)
{
	if(v[0] == 1 && v[1] == 2 && v[2] == 3 && v[3] == 4 &&
	   v[4] == 5 && v[5] == 6 && v[6] == 7 && v[7] == 8 && v[8] == 0 )
		return true; 
	return false; 
} 

bool is_visited(vector<int> v)
{
	for(unsigned i = 0; i < visited.size(); i++)
	{
		if(v[0] == visited[i][0] && v[1] == visited[i][1] && v[2] == visited[i][2] && v[3] == visited[i][3] && v[4] == visited[i][4] && v[5] == visited[i][5] && v[6] == visited[i][6] && v[7] == visited[i][7] && v[8] == visited[i][8] )
			return true; 
	}
	return false; 
}
// uniform cost search
void UCS()
{
	node init_node;
	init_node.X = puzzle; 
	init_node.h = 0; 
	init_node.g = 0; 
	init_node.f = 0; 
	pq.push(init_node); 
	visited.push_back(puzzle); 
        bool skip_iter = true; 
	int g_value = 1; // calculates depth
	nodes_in_queue = pq.size();// used to find the max queue size 
	while(1)
	{
		node current = pq.top(); 
		pq.pop(); 
		if( check_goal_state(current.X) == true)
			return; 
		node temp = current; 
		
		vector<node> level; // stores the valid nodes and pushes them later.

		if( swap_down(temp.X ,star_position(current.X)) == 1)
		{
		  if(is_visited(temp.X) == false) {
		  	skip_iter = false; 
			temp.h = 0; 
			temp.f = temp.g + temp.h; 
		        
			level.push_back(temp); 
		  }
		}
		
		temp = current; 
		if( swap_up(temp.X ,star_position(current.X)) == 1)
		{
		  if(is_visited(temp.X) == false) {
		  	skip_iter = false; 
			temp.h = 0; 
			temp.f = temp.g + temp.h; 
		
			level.push_back(temp); 
		  }
		}

		temp = current; 
		if( swap_right(temp.X ,star_position(current.X)) == 1)
		{
		  if(is_visited(temp.X) == false) {
		  	skip_iter = false; 
			temp.g = g_value;
			temp.h = 0; 
			temp.f = temp.g; 

			level.push_back(temp);
		  }
		}

		temp = current; 
		if( swap_left(temp.X ,star_position(current.X)) == 1)
		{
		  if(is_visited(temp.X) == false) {
		  	skip_iter = false; 
			temp.g = g_value; 
			temp.h = 0; 
			temp.f = temp.g;

			level.push_back(temp); 
		  }
		}
		
		// if no operations are invoked, I need to make sure that my depth value doesn't over increment
		if(skip_iter == true) { 
			g_value--; 
			skip_iter = true;
		}
		// set my g values
		for(unsigned i = 0 ;i < level.size(); i++)
		{
			level[i].g = g_value; 
		}
			
		for(unsigned i = 0; i < level.size(); i++) {
			pq.push(level[i]); 
			visited.push_back(level[i].X); 
			num_nodes++; 

			cout << "The best state to expand with g(n) = " << level[i].g << " and h(n) = " << level[i].h << " is..."; 
			print_puzzle(level[i].X); 
			if(check_goal_state(level[i].X) == true )
			{
				depth = level[i].g; 
				return; 
			}
		}
		skip_iter = true; 
		g_value++; 
		if(nodes_in_queue < pq.size() )
			nodes_in_queue = pq.size(); 
	}
}

int get_h(vector<int> v)
{
	int i = 0; 
	if(v[0] != 1)
		i++; 
	if(v[1] != 2)
		i++; 
	if(v[2] != 3)
		i++; 
	if(v[3] != 4) 
		i++; 
	if(v[4] != 5) 
		i++; 
	if(v[5] != 6) 
		i++; 
	if(v[6] != 7)
		i++; 
	if(v[7] != 8) 
		i++; 
	if(v[8] != 0)
		i++; 
	return i; 
}

void Misplaced_Tile()
{
	node init_node;
	init_node.X = puzzle; 
	init_node.h = 0; 
	init_node.g = 0; 
	init_node.f = 0; 
	pq.push(init_node); 
	visited.push_back(puzzle); 
        bool skip_iter = true; 
	int g_value = 1; 
	nodes_in_queue = pq.size(); 
	while(1)
	{
		node current = pq.top(); 
		pq.pop(); 
		if( check_goal_state(current.X) == true)
			return; 
		node temp = current; 
		
		vector<node> level; 

		if( swap_down(temp.X ,star_position(current.X)) == 1)
		{
		  if(is_visited(temp.X) == false) {
		  	skip_iter = false; 
			temp.h = get_h(temp.X); 
			temp.f = temp.g + temp.h; 
		        
			level.push_back(temp); 
		  }
		}
		
		temp = current; 
		if( swap_up(temp.X ,star_position(current.X)) == 1)
		{
		  if(is_visited(temp.X) == false) {
		  	skip_iter = false; 
			temp.h = get_h(temp.X); 
			temp.f = temp.g + temp.h; 
		
			level.push_back(temp); 
		  }
		}

		temp = current; 
		if( swap_right(temp.X ,star_position(current.X)) == 1)
		{
		  if(is_visited(temp.X) == false) {
		  	skip_iter = false; 
			temp.g = g_value;
			temp.h = get_h(temp.X); 
			temp.f = temp.g + temp.h; 

			level.push_back(temp);
		  }
		}

		temp = current; 
		if( swap_left(temp.X ,star_position(current.X)) == 1)
		{
		  if(is_visited(temp.X) == false) {
		  	skip_iter = false; 
			temp.g = g_value; 
			temp.h = get_h(temp.X); 
			temp.f = temp.g + temp.h;

			level.push_back(temp); 
		  }
		}

		if(skip_iter == true) {
			g_value--; 
			skip_iter = true;
		}
		for(unsigned i = 0 ;i < level.size(); i++)
		{
			level[i].g = g_value; 
		}
	
		for(unsigned i = 0; i < level.size(); i++) {
			pq.push(level[i]); 
			visited.push_back(level[i].X); 
			num_nodes++; 

			cout << "The best state to expand with g(n) = " << level[i].g << " and h(n) = " << level[i].h << " is..."; 
			print_puzzle(level[i].X); 
			if(check_goal_state(level[i].X) == true ) {
				depth = level[i].g; 
				return; 
			}
		}
		skip_iter = true; 
		g_value++; 
		if(nodes_in_queue < pq.size() )
			nodes_in_queue = pq.size(); 

	}
}

int calculate_hn_Manhattan(vector<int> v)
{
	int h = 0; 
	if(v[0] != 1)
	{
		if(v[1] == 1)
			h += 1; 
		if(v[2] == 1)
			h += 2; 
		if(v[3] == 1)
			h += 1; 
		if(v[4] == 1)
			h+= 2; 
		if(v[5] == 1)
			h += 3; 
		if(v[6] == 1)
			h += 2; 
		if(v[7] == 1)
			h += 3; 
		if(v[8] == 1)
			h += 4; 
	}
	if(v[1] != 2)
	{
		if(v[0] == 2)
			h += 1; 
		if(v[2] == 2)
			h += 1; 
		if(v[3] == 2)
			h += 2; 
		if(v[4] == 2)
			h+= 1; 
		if(v[5] == 2)
			h += 2; 
		if(v[6] == 2)
			h += 3; 
		if(v[7] == 2)
			h += 2; 
		if(v[8] == 2)
			h += 3; 
	}
	if(v[2] != 3)
	{
		if(v[0] == 3)
			h += 2; 
		if(v[1] == 3)
			h += 1; 
		if(v[3] == 3)
			h += 3; 
		if(v[4] == 3)
			h+= 2; 
		if(v[5] == 3)
			h += 1; 
		if(v[6] == 3)
			h += 4; 
		if(v[7] == 3)
			h += 3; 
		if(v[8] == 3)
			h += 2; 
	}
	if(v[3] != 4)
	{
		if(v[0] == 4)
			h += 1; 
		if(v[1] == 4)
			h += 2; 
		if(v[2] == 4)
			h += 3; 
		if(v[4] == 4)
			h+= 1; 
		if(v[5] == 4)
			h += 2; 
		if(v[6] == 4)
			h += 1; 
		if(v[7] == 4)
			h += 2; 
		if(v[8] == 4)
			h += 3; 
	}
	if(v[4] != 5)
	{
		if(v[0] == 5)
			h += 2; 
		if(v[1] == 5)
			h += 1; 
		if(v[2] == 5)
			h += 2; 
		if(v[3] == 5)
			h+= 1; 
		if(v[5] == 5)
			h += 1; 
		if(v[6] == 5)
			h += 2; 
		if(v[7] == 5)
			h += 1; 
		if(v[8] == 5)
			h += 2; 
	}
	if(v[5] != 6)
	{
		if(v[0] == 6)
			h += 3; 
		if(v[1] == 6)
			h += 2; 
		if(v[2] == 6)
			h += 1; 
		if(v[3] == 6)
			h+= 2; 
		if(v[4] == 6)
			h += 1; 
		if(v[6] == 6)
			h += 3; 
		if(v[7] == 6)
			h += 2; 
		if(v[8] == 6)
			h += 1; 
	}
	if(v[6] != 7)
	{
		if(v[0] == 7)
			h += 2; 
		if(v[1] == 7)
			h += 3; 
		if(v[2] == 7)
			h += 4; 
		if(v[3] == 7)
			h+= 1; 
		if(v[4] == 7)
			h += 2; 
		if(v[5] == 7)
			h += 3; 
		if(v[7] == 7)
			h += 1; 
		if(v[8] == 7)
			h += 2; 
	}
	if(v[7] != 8)
	{
		if(v[0] == 8)
			h += 3; 
		if(v[1] == 8)
			h += 2; 
		if(v[2] == 8)
			h += 3; 
		if(v[3] == 8)
			h+= 2; 
		if(v[4] == 8)
			h += 1; 
		if(v[5] == 8)
			h += 2; 
		if(v[6] == 8)
			h += 1; 
		if(v[8] == 8)
			h += 1; 
	}
	return h; 
}

void Manhattan()
{
	node init_node;
	init_node.X = puzzle; 
	init_node.h = 0; 
	init_node.g = 0; 
	init_node.f = 0; 
	pq.push(init_node); 
	visited.push_back(puzzle); 
        bool skip_iter = true; 
	int g_value = 1; 
	nodes_in_queue = pq.size(); 
	while(1)
	{
		node current = pq.top(); 
		pq.pop(); 
		if( check_goal_state(current.X) == true)
			return; 
		node temp = current; 
		
		vector<node> level; 

		if( swap_down(temp.X ,star_position(current.X)) == 1)
		{
		  if(is_visited(temp.X) == false) {
		  	skip_iter = false; 
			temp.h = calculate_hn_Manhattan(temp.X); 
			temp.f = temp.g + temp.h; 
		        
			level.push_back(temp); 
		  }
		}
		
		temp = current; 
		if( swap_up(temp.X ,star_position(current.X)) == 1)
		{
		  if(is_visited(temp.X) == false) {
		  	skip_iter = false; 
			temp.h = calculate_hn_Manhattan(temp.X); 
			temp.f = temp.g + temp.h; 
		
			level.push_back(temp); 
		  }
		}

		temp = current; 
		if( swap_right(temp.X ,star_position(current.X)) == 1)
		{
		  if(is_visited(temp.X) == false) {
		  	skip_iter = false; 
			temp.g = g_value;
			temp.h = calculate_hn_Manhattan(temp.X); 
			temp.f = temp.g + temp.h; 

			level.push_back(temp);
		  }
		}

		temp = current; 
		if( swap_left(temp.X ,star_position(current.X)) == 1)
		{
		  if(is_visited(temp.X) == false) {
		  	skip_iter = false; 
			temp.g = g_value; 
			temp.h = calculate_hn_Manhattan(temp.X); 
			temp.f = temp.g + temp.h;

			level.push_back(temp); 
		  }
		}

		if(skip_iter == true) {
			g_value--; 
			skip_iter = true;
		}
		for(unsigned i = 0 ;i < level.size(); i++)
		{
			level[i].g = g_value; 
		}
	
		for(unsigned i = 0; i < level.size(); i++) {
			pq.push(level[i]); 
			visited.push_back(level[i].X); 
			num_nodes++; 
			cout << "The best state to expand with g(n) = " << level[i].g << " and h(n) = " << level[i].h << " is..."; 
			print_puzzle(level[i].X); 
			if(check_goal_state(level[i].X) == true ) {
				depth = level[i].g;
				return; 
			}
		}
		skip_iter = true; 
		g_value++; 
		if(nodes_in_queue < pq.size() )
			nodes_in_queue = pq.size(); 

	}
}

int main()
{
	num_nodes = 0; 
	nodes_in_queue = 0; 
	string in = ""; 
	cout << "Welcome to 8-puzzle solver.\nType \"1\" to use a default puzzle, or \"2\" to enter your own puzzle\n"; 
	cin >> in; 

	if(in == "2") {
		cout << "Enter your puzzle, use a zero to represent the blank\nEnter puzzle, like 123456780.\n"; 
		cin >> in; 
		puzzle.push_back( (in[0])-48 ); 
		puzzle.push_back( (in[1])-48 ); 
		puzzle.push_back( (in[2])-48 ); 
		puzzle.push_back( (in[3])-48 ); 
		puzzle.push_back( (in[4])-48 ); 
		puzzle.push_back( (in[5])-48 );
		puzzle.push_back( (in[6])-48 ); 
		puzzle.push_back( (in[7])-48 ); 
		puzzle.push_back( (in[8])-48 );
	}
	else if(in == "1")
	{
		puzzle.push_back(1); 
		puzzle.push_back(2); 
		puzzle.push_back(3);
		puzzle.push_back(4); 
		puzzle.push_back(0); 
		puzzle.push_back(6); 
		puzzle.push_back(7); 
		puzzle.push_back(5); 
		puzzle.push_back(8); 

		cout << "Expanding state "; 
		print_puzzle(puzzle); 
	}
	
	
	cout << "Enter your choice of algorithm\n1.Uniform Cost Search\n2.A* with the Misplaced Tile heuristic.\n3.A* with the Manhattan distance heuristic\n"; 
	cin >> in; 

	cout << "Expanding state "; 
		print_puzzle(puzzle); 

	if(in == "1") {
		UCS();
	}
	else if(in == "2")
	{
		Misplaced_Tile();
	}
	else if(in == "3")
	{
		Manhattan(); 		 
	}

	cout << "goal!! \n\nTo solve this problem the search algorithm expanded a total of " << num_nodes << " nodes. The maximum number of nodes in the queue at on time was " << nodes_in_queue << ". The depth of the goal node was " << depth << endl; 
	return 0;  
}
