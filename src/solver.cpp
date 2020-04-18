#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <map>
#include <unistd.h>
#include "pukoban.hpp"
using namespace std;
char move_ahead[12] = {'U','D','L','R','^','v','<','>','u','d','l','r' };
long long total = 0LL;
long long right_place = 0LL;
int a = 0;
int b = 0;
long long wall=0LL;
long long left_boundary=0LL,right_boundary=0LL;
long long me=0LL;
string answerlist;

vector<long long> memory[2][10000000];
vector<int> history[2][10000000];
vector<int> last[2];
vector<int> todo;
int counter[2];
int step=0;

double start,the_end;
map<long long,int> mymap;
map<long long,int>::iterator iter;
int total_node;
int meet_map_0,meet_me_0,meet_map_1,meet_me_1;

map<long long,int> mymap_c;

bool valid(long long wall,long long total,long long me,int a, int b,int i)
{
	if (i % 4 == 0)
	{
		if ((me >> b) == 0LL)
			return false;
		if (i == 8)
		{
			if ((((me >> b) ^ wall) < wall) || (((me >> b) ^ total) < total))
			return false;
		}
		else if (i == 0)
		{
			if ((((me >> b) ^ total) > total) || (((me >> (2 * b)) ^ wall) < wall) || (((me >> (2 * b)) ^ total) < total)|| ((me >>(2* b)) == 0LL))
				return false;
		}
		else if(i==4)
		{
			if ((((me << b) ^ total) > total) || (((me >> b) ^ wall) < wall) || (((me >> b) ^ total) < total))
				return false;
		}
		return true;
	}
	else if (i % 4 == 1)
	{
		if ((me << b) >= (1LL << (a*b)))
			return false;
		if (i == 9)
		{
			if ((((me << b) ^ wall) < wall) || (((me << b) ^ total) < total))
				return false;
		}
		else if (i == 1)
		{
			if ((((me << b) ^ total) > total) || (((me << (2 * b)) ^ wall) < wall) || (((me << (2 * b)) ^ total) < total)||((me<<(2*b))>= (1LL << (a*b))))
				return false;
		}
		else if(i==5)
		{
			if ((((me >> b) ^ total) > total) || (((me << b) ^ wall) < wall) || (((me << b) ^ total) < total)||((me>>b)==0LL))
				return false;
		}
		return true;
	}
	else if (i % 4 == 2)
	{
		if((me&left_boundary)>0)
			return false;
		if (i == 10)
		{
			if ((((me >> 1) ^ wall) < wall) || (((me >> 1) ^ total) < total))
			return false;
		}
		else if (i == 2)
		{
			if ((((me>>1)&left_boundary)>0) || (((me >> 1) ^ total) > total) || (((me >> 2) ^ wall) < wall) || (((me >> 2) ^ total) < total))
				return false;
		}
		else if (i==6)
		{
			if (((me&right_boundary)>0) || (((me << 1) ^ total) > total) || (((me >> 1) ^ wall) < wall) || (((me >> 1) ^ total) < total))
				return false;
		}
	}
	else
	{
		if((me&right_boundary)>0)
                        return false;
		if (i == 11)
		{
			if ((((me << 1) ^ wall) < wall) || (((me << 1) ^ total) < total))
			return false;
		}
		else if (i == 3)
		{
			if ((((me<<1)&right_boundary)>0) || (((me << 1) ^ total) > total) || (((me << 2) ^ wall) < wall) || (((me << 2) ^ total) < total))
				return false;
		}
		else if (i==7)
		{
			if (((me&left_boundary)>0) || (((me >> 1) ^ total) > total) || (((me << 1) ^ wall) < wall) || (((me << 1) ^ total) < total))
				return false;
		}
	}
	return true;
}
bool check_memory(long long total,long long me,int last_step,int direction,bool ht)
{
	int map_condition = last_step / 100;
	if (direction > 7)
	{
		vector<long long>::iterator iter = memory[ht][map_condition].begin();
		int termi = 0;
		for (int ix = 0; iter != memory[ht][map_condition].end(); ++iter, ++ix)
		{
			if (memory[ht][map_condition][ix] == me)
				return false;
			else
				termi += 1;
		}
		memory[ht][map_condition].push_back(me);
		total_node+=1;
		history[ht][map_condition].push_back(last_step);
		last[ht].push_back(map_condition * 100 + termi);
		return true;
	}
	else
	{
		if(!ht)
			iter=mymap.find(total);
		else
			iter=mymap_c.find(total);
		if(iter!=mymap.end() and iter!=mymap_c.end())
		{
			int i=iter->second;
			vector<long long>::iterator iter = memory[ht][i].begin();
			int termi = 0;
			for (int ix = 0; iter != memory[ht][i].end(); ++iter, ++ix)
			{
				if (memory[ht][i][ix] == me)
					return false;
				else
					termi += 1;
			}
			memory[ht][i].push_back(me);
			total_node+=1;
			history[ht][i].push_back(last_step);
			last[ht].push_back(i * 100 + termi);
			return true;
		}
		else
		{
			if(!ht)
				mymap[total]=counter[ht];
			else
				mymap_c[total]=counter[ht];
			memory[ht][counter[ht]].push_back(total);
			memory[ht][counter[ht]].push_back(me);
			total_node+=1;
			history[ht][counter[ht]].push_back(0);
			history[ht][counter[ht]].push_back(last_step);
			last[ht].push_back(counter[ht] * 100 + 1);
			counter[ht] += 1;
			return true;
		}
	}
}
long long place(long long object,int i,int b)
{
	if (i == 0)
		object = object >> b;
	else if (i == 1)
		object = object << b;
	else if (i == 2)
		object = object >> 1;
	else if (i == 3)
		object = object << 1;
	return object;
}
bool solve_pukoban(bool ht)
{
	step++;
	todo.assign(last[ht].begin(), last[ht].end());
	last[ht].clear();
	vector<int>::iterator iter1 = todo.begin();
	for (int i = 0; iter1 != todo.end(); ++i,++iter1)
	{
		int total = todo[i] / 100;
		long long me = memory[ht][total][todo[i] % 100];
		long long total_box = memory[ht][total][0];
		for (int j = 0; j < 12; j++)
		{
			if (valid(wall, total_box, me, a, b, j))
			{
				long long box=total_box;
				if (j < 4)
				{
					long long new_box = place(place(me, j % 4, b), j % 4, b);
					box = (new_box | place(me, j % 4, b)) ^ total_box;
				}
				else if ((j < 8) && (j > 3))
				{
					long long new_box = place(me, (j % 4) ^ 1, b);
					box = (new_box | me) ^ total_box;
				}
				long long new_me=place(me,j%4,b);
				if(check_memory(box, new_me, todo[i], j,ht))
				{

					if(ht)
						iter=mymap.find(box);
					else
						iter=mymap_c.find(box);
					if (iter!=mymap.end() and iter!=mymap_c.end())//box in memory
					{
						int m=iter->second;
						vector<long long>::iterator iter = memory[!ht][m].begin();
						for (int ix = 0; iter != memory[!ht][m].end(); ++iter, ++ix)
						{
							if (memory[!ht][m][ix] == new_me)
							{
								if(ht)
								{
									meet_map_0=m;
									meet_me_0=ix;
									meet_map_1=total;
									meet_me_1=todo[i]%100;
								}
								else
								{
									meet_map_0=total;
									meet_me_0=todo[i]%100;
									meet_map_1=m;
									meet_me_1=ix;
								}
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}

int main(int argc, char *argv[]) {
	FILE *f = fopen(argv[1], "r");
	FILE *f1=fopen(argv[1],"r");
	FILE *fp = fopen(argv[2], "w");
	while (fscanf(f, "%d %d", &a, &b) != EOF) {
		start=clock();
		total = 0LL;
		right_place = 0LL;
		counter[0] = 0;
		counter[1] = 1;
		wall = 0LL;
		me = 0LL;
		step=0;
		right_boundary=0LL;
		left_boundary=0LL;
		total_node=0;
		char temp;
		mymap.erase(mymap.begin(),mymap.end());
		mymap_c.erase(mymap_c.begin(),mymap_c.end());
		for (int i = 0; i < a*b; i++)
		{
			if(i%b==0)
				left_boundary|=(1LL<<i);
			else if(i%b==b-1)
				right_boundary|=(1LL<<i);
			fscanf(f, "%c", &temp);
			if (temp == '\n')
				fscanf(f, "%c", &temp);
			if (temp == '@')
				me = 1LL << i;	
			else if (temp == '+')
			{
				me = 1LL << i;
				right_place = right_place | me;
			}
			else if (temp == '$')
				total = total | (1LL << i);
			else if (temp == '*')
			{
				total = total | (1LL << i);
				right_place = right_place | (1LL << i);	
			}
			else if (temp == '.')
				right_place = right_place | (1LL << i);
			else if (temp == '#')
				wall = wall | (1LL << i);
		}
		fscanf(f, "%c", &temp);
		memory[0][0].push_back(total);
		memory[0][0].push_back(me);
		mymap[total]=0;
		history[0][0].push_back(0);
		history[0][0].push_back(0);
		last[0].push_back(1);
		counter[0] += 1;

		long long tmp=1LL;
		memory[1][0].push_back(right_place);
		history[1][0].push_back(0);
		mymap_c[right_place]=0;
		int termi=1;
		long long me_history=0LL;
		for(int i=0;i<a*b;i++)
		{
			if((tmp^right_place)<right_place)
			{
				for(int j=0;j<4;j++)
				{
					if(valid(wall,right_place,tmp,a,b,j+8))
					{	
						me=place(tmp,j,b);
						if((me^me_history)<me_history)
							continue;
						me_history|=me;
						memory[1][0].push_back(me);
						history[1][0].push_back(0);
						last[1].push_back(termi);
						termi+=1;
						total_node+=1;
					}
				}
			}
			tmp=tmp<<1;
		}
		bool ht=0;
		while (!solve_pukoban(ht))
			ht=!ht;
		string str;
		for(int i=0;i<2;i++)
		{
			int map=meet_map_0;
			int me=meet_me_0;
			ht=i;
			while (history[ht][map][me] != 0)
			{
			    	long long now = memory[ht][map][me];
			    	long long last_one = memory[ht][history[ht][map][me] / 100][history[ht][map][me] % 100];
			    	int aa=log(now)/log(2);
			    	int bb=log(last_one)/log(2);
			    	int dire = aa-bb;
			    	int cc;
			    	if (dire == 1)
					cc = 3;
			    	else if (dire == -1)
					cc = 2;
			    	else if (dire > 1)
					cc = 1;
			    	else
					cc = 0;
			    	if (memory[ht][map][0] == memory[ht][history[ht][map][me] / 100][0])
					answerlist= move_ahead[8 + cc]+answerlist;
			    	else if ((memory[ht][map][0] - memory[ht][history[ht][map][me] / 100][0] )> last_one)
					answerlist=move_ahead[cc]+answerlist;
			    	else if ((memory[ht][map][0] - memory[ht][history[ht][map][me] / 100][0])==-1* last_one)
			 	{
					answerlist=move_ahead[4+cc]+answerlist;
			    	}			
			    	else
			    	{
					if ((cc == 0) || (cc == 2))
					{
				    		if (( memory[ht][history[ht][map][me] / 100][0]- memory[ht][map][0])>last_one)
							answerlist= move_ahead[4+cc] + answerlist;
				    		else
							answerlist =move_ahead[cc] + answerlist;
					}
					else
						answerlist =move_ahead[4+cc] + answerlist;
				}
				int temp_map = history[ht][map][me] / 100;
				int temp_me= history[ht][map][me] % 100;
				map = temp_map;
			    	me = temp_me;
			}
			if(!ht)
			{
				history[1][counter[1]].push_back(0);
				history[1][counter[1]].push_back(0);
				memory[1][counter[1]].push_back(memory[0][meet_map_0][0]);
				memory[1][counter[1]].push_back(memory[0][meet_map_0][meet_me_0]);
		                int last_map=counter[1];
                		int last_me=1;
				while(history[1][meet_map_1][meet_me_1]!=0)
				{
					int next=history[1][meet_map_1][meet_me_1];
					history[1][meet_map_1][meet_me_1]=last_map*100+last_me;
					last_map=meet_map_1;
					last_me=meet_me_1;
					meet_map_1=next/100;
					meet_me_1=next%100;
				}
				history[1][meet_map_1][meet_me_1]=last_map*100+last_me;
				meet_map_0=meet_map_1;
				meet_me_0=meet_me_1;
				counter[1]++;
			}
			str=str+answerlist;
			answerlist="";
		}
		int answer = str.length();
		fprintf(fp,"%d\n%s\n",answer,str.c_str());
		while (counter[0] != 0)
		{
			counter[0]--;
			memory[0][counter[0]].clear();
			history[0][counter[0]].clear();
		}
		while (counter[1] != 0)
		{
			counter[1]--;
			memory[1][counter[1]].clear();
			history[1][counter[1]].clear();
		}
		last[0].clear();
		last[1].clear();
		todo.clear();
		the_end=clock();
		BOARD b;
		#define CLEAR_SCREEN fprintf(stdout, "\033[H\033[J")
		//fprintf(stdout, "\nMove Count: \033[1m%3d\033[m\n", SZ(b.moves));
		//fprintf(stdout, "Moves: "); b.display_moves();
		//fprintf(stdout, "\n");
		bool aaa=get_board(f1,b);
		b.display_board();
		for(int i=0,n=str.length();i<n;i++)
		{
			bool moved = 0;
			bool drag = 0; // dragging- bit
			CLEAR_SCREEN;
			cout<<"time="<<(the_end-start)/CLOCKS_PER_SEC<<' '<<"nodes="<<total_node<<' '<<"step="<<step<<endl;
			fprintf(stdout,"\nmove count:%d\n",i+1);
			fprintf(stdout,"\nmoves:");
			for(int j=0;j<=i;j++)
				fprintf(stdout,"%c",str[j]);
			fprintf(stdout,"\n");
			MOVE m;
			switch ( str[i] ) {
				case 'U' : case 'u' : case '^': 
					m.dir = UP; break;
				case 'D' : case 'd' : case 'v':
					m.dir = DOWN; break;		
				case 'L' : case 'l' : case '<':
					m.dir = LEFT; break;
				case 'R' : case 'r' : case '>':
					m.dir = RIGHT; break;
			}
			if(str[i]=='^' or str[i]=='v' or str[i]=='<' or str[i]=='>')
				drag=!drag;
			m.drag = drag;
			moved = do_move(b, m);
			b.display_board();
			usleep(300000);
		}
	}
	fclose(fp);
	return 0;
}

