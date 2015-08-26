/*
	@Author: Pranay Yadav, 12CS30025
	@Description: To check whether the given N Planar points (in CCW) form a simple Polygon or not.
*/

#include <string>
#include <queue>
#include <iostream>
#include <stdlib.h>
#include <set>
#include <utility>
#include <vector>
#include <algorithm>
#define LEFT 0
#define RIGHT 1
using namespace std;
int N=0;
class point {
public:	
	double x;
	double y;
};
point* listPoints=NULL;
class segment{
public:
	int edgeID;
	point lp;
	point rp;
};
class Event{
public:
	int edgeID;
	int whichEnd;
	point vertex;
};
class EventQueue {
public:
    int      size;                // total number of events in array
    Event* sortedv;
    EventQueue(point* P);
};
class CompareEventPoint {
public:
	bool operator()(point& t1, point& t2) // Returns true if t1 is earlier than t2
	{
		if(t1.x < t2.x)
			return true;
		if(t1.x > t2.x)
			return false;
		if(t1.y < t2.y)
			return true;
		if(t1.y > t2.y)
			return false;
		return false;
	}
};

struct classComparator {
	bool operator() (const segment& lhs,const segment& rhs) 
	{
		if(rhs.lp.x==lhs.rp.x && rhs.lp.y==lhs.rp.y)
	   		return ((rhs.lp.x - lhs.rp.x) * (rhs.rp.y - lhs.rp.y) - (rhs.lp.y - lhs.rp.y) * (rhs.rp.x - lhs.rp.x))<0;
	 	else
	   		return ((rhs.lp.x - lhs.lp.x) * (rhs.rp.y - lhs.lp.y) - (rhs.lp.y - lhs.lp.y) * (rhs.rp.x - lhs.lp.x))<0;
	}
};
int lexOrdering(point& t1, point& t2){
	if (t1.x < t2.x)
		return -1;
	if(t1.x > t2.x)
		return 1;
	if(t1.y < t2.y)
		return -1;
	if(t1.y > t2.y)
		return 1;
	return 0;
}
int CompareEventPoint(const void* v1, const void* v2){
	Event* e1= (Event*) v1;
	Event* e2= (Event*) v2;
	return lexOrdering(e1->vertex,e2->vertex);
}

set<segment,classComparator>::iterator add(set<segment,classComparator>& tree, Event& E)
{
    pair<set<segment,classComparator>::iterator,bool> ret;
    segment s;
    s.edgeID = E.edgeID;
    point v1 = listPoints[s.edgeID]; 
    point v2 = listPoints[s.edgeID+1]; 
    if (lexOrdering(v1,v2) < 0) 
    {
        s.lp = v1; 
        s.rp = v2; 
    } 
    else { 
        s.rp = v1; 
        s.lp = v2;
    }
    ret= tree.insert(s);
    if(ret.second == true)
    	return ret.first;
}

set<segment,classComparator>::iterator find(set<segment,classComparator>& tree, Event& E)
{
    set<segment,classComparator>::iterator it;
	segment s;
    s.edgeID = E.edgeID;
    it = tree.find(s);
    return it;
}

bool intersect(segment a,segment b)
{
    int e1 = a.edgeID;
    int e2 = b.edgeID;
    if (((e1+1)%N == e2) || (e1 == (e2+1)%N))
        return false;
    double num=(a.lp.y-b.lp.y)*(b.rp.x-b.lp.x)-(a.lp.x-b.lp.x)*(b.rp.y-b.lp.y);
	double den=(a.lp.y-a.rp.y)*(b.rp.x-b.lp.x)-(a.lp.x-a.rp.x)*(b.rp.y-b.lp.y);
	double s=num/den;
	if(s>0 && s<1)
	{
		double t=((1-s)*a.lp.x+s*a.rp.x-b.lp.x)/(b.rp.x-b.lp.x);
	  	if(t>0 && t<1)
	  		return true;
	}
	return false;
}


EventQueue::EventQueue(point* P)
{
    int i = 0;
    size  = 2 * N; 
    sortedv = new Event[size];
    Event e1,e2;
    for (int i=0; i < N; i++) {        
        sortedv[2*i].edgeID=i;
        sortedv[2*i+1].edgeID=i;
        sortedv[2*i].vertex=P[i];
        sortedv[2*i+1].vertex=P[i+1];
        if (lexOrdering(P[i],P[i+1]) < 0)  { // determine type
            sortedv[2*i].whichEnd = LEFT;
            sortedv[2*i+1].whichEnd = RIGHT;
        }
        else {
            sortedv[2*i].whichEnd = RIGHT;
            sortedv[2*i+1].whichEnd = LEFT;
        }
    }
    qsort(sortedv,size,sizeof(Event),CompareEventPoint);
}

bool isSimplePolygon(point* Pn)
{
    EventQueue Eq(Pn);
    set<segment,classComparator> tree;
    set<segment,classComparator>::iterator it,above,below;
    for(int i=0; i < Eq.size ; ++i)
    {
        if (Eq.sortedv[i].whichEnd == LEFT) {      
            it = add(tree,Eq.sortedv[i]);          
			above = tree.upper_bound(*it);
			below = tree.lower_bound(*it);
			if (intersect(*it,*above)) 
                 return false;      // Pn is NOT simple
            if (intersect(*it,*below)) 
                 return false;      // Pn is NOT simple
        }
        else {                      
            it = find(tree,Eq.sortedv[i]);
            above = tree.upper_bound(*it);
			below = tree.lower_bound(*it);
		    if (intersect(*above,*below)) 
                 return false;      // Pn is NOT simple
            tree.erase(*it);           
        }
    }
    return true;
}

int main()
{
	int i=0;
	double x,y;
	cin>>N;
	listPoints = new point[N];
	for(i=0;i<N;++i){
		cin>>listPoints[i].x>>listPoints[i].y;
	}
	bool isSimple =  isSimplePolygon(listPoints);
	if(isSimple)
		cout<<"Yes!! It is a simple Polygon!!!";
	else
		cout<<"No!! It is not simple Polygon!!!";
	delete [] listPoints;
	cout<<endl;
	return 0;
}

