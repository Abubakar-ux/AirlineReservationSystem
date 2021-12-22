#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "Date.h"
#include "Query.h"
#include "Time.h"
#include "Vector.h"
#include "Queue.h"
using namespace std;
struct Node {//Used to store data of the specific flight.
	string source, dest, airline;
	Time startTime, endTime; //This is just a temporary measure. We have to make classes of date, time to handle the different Scenarios.
	Date date;
	int hotelCharges = 0;
	int cost;
	Node* next;
	Time calculateTime();
	bool visited;
	Node()
	{
		source = "\0";
		visited = false;
		next = NULL;
		cost = 0;
	}
	void Intialize(const Node obj) {
		this->source = obj.source;
		this->dest = obj.dest;
		this->airline = obj.airline;
		this->date = obj.date;
		this->hotelCharges = obj.hotelCharges;
		this->startTime = obj.startTime;
		this->endTime = obj.endTime;
		this->cost = obj.cost;
		next = NULL;
	}


	friend ostream& operator<<(ostream& os, const Node& city);
};

ostream& operator<<(ostream& os, const Node& city)//Overload accordingly later....
{
	os << city.dest;
	return os;
}

Time Node::calculateTime()//Calculates travel time based on starting time and endtime.
{
	Time getTime;
	if (this->startTime.getMinute() == 0 && this->endTime.getMinute() == 0)//For the case of just hours.
	{
		int time = startTime.getHour();
		int counter = 0;
		while (time != endTime.getHour())
		{
			if (time % 24 == 0)
			{
				time = 0;
			}
			counter++;
			time++;
		}
		getTime.setHour(counter);
		getTime.setMinute(0);
		return getTime;
	}
	else//If minutes are not equal to 0.
	{
		int minutes = startTime.getMinute();
		int timeHours = startTime.getHour();
		int counterHours = 0;
		int counterMinutes = 0;
		while (minutes != endTime.getMinute())
		{
			if (minutes % 60 == 0)
			{
				minutes = 0;
				timeHours = timeHours + 1;//Increasing hours.
				if (minutes == endTime.getMinute())
				{
					break;
				}
			}
			minutes++;
			counterMinutes++;
		}
		while (timeHours != endTime.getHour())
		{
			if (timeHours % 24 == 0)
			{
				timeHours = 0;
			}
			counterHours++;
			timeHours++;
		}
		getTime.setHour(counterHours);
		getTime.setMinute(counterMinutes);
		return getTime;
	}
}



struct List {//For our linked list of flights.
	Node* head;

	//Constructor
	List() {
		head = NULL;
	}

	void push(const Node obj) {
		Node* newNode = new Node;
		newNode->Intialize(obj);
		newNode->next = NULL;
		if (head == NULL) {
			head = newNode;
		}
		else {
			Node* temp = head;
			while (temp->next != NULL) {
				temp = temp->next;
			}
			temp->next = newNode;
		}
	}

	void remove() {
		Node* prev = NULL;
		if (!head)
			return;
		else {
			Node* temp = head;
			while (temp->next) {
				prev = temp;
				temp = temp->next;
			}
			delete temp;
			temp = NULL;
		}
	}
	string operator [] (const int index)//Finding specific node in vector and outputting its data.
	{
		Node* cursor = this->head;
		int current = 0;
		while (cursor != NULL)
		{
			if (current == index && cursor != NULL)
			{
				return cursor->source;
			}
			else
			{
				cursor = cursor->next;
				current++;
			}
		}
	}


};

struct AdjNode {//These are basically our vertices.
	string origin; // This is the origin of the Flight. 
	List destinations; // This holds the list of the destinations for each adjNode. 
	AdjNode* next;
	bool visited;

	AdjNode()
	{
		origin = "\0";
		destinations.head = NULL;
		next = NULL;
		visited = false;
	}
};

class Flight {
	AdjNode* head;

public:

	Flight();//Default constructor, reads our file and makes graph.
	Flight(int a);//So that default constructor doesn't call. Used for subgraph.
	void DrawGraph();//Displays whole graph.
	void DrawGraphDay(int d);//Display whole graph for specific day.
	void menu();//Displaying menu for passenger.
	List DirectFlights(const Query& passenger);//Scenario 5.
	int minTime(Vector <Node> v1, int day);
	int transit_time(Node, Node);//Calculates transit time between two flights in minutes.

	void createSubgraph(Flight& subgraph, int date, string airline);//For scenario 1.
	void createSubgraph(Flight& subgraph, int date);//For scenario 2 and ahead.
	Node* getDestinations(AdjNode* vertex, string city);//Returns Node pointer of head of specific city.
	int calculate_flightcost(Vector<Node> flight);//Calculates total cost of flight including transit charges etc.
	Time calculate_flighttime(Vector<Node> flight);//Calculates total time of flight including transit time etc.

	void scenario1(const Query& passenger);
	void scenario2(const Query& passenger);
	void scenario3(const Query& passenger);
	void scenario4(const Query& passenger);
	void scenario5(const Query& passenger);

	void savepath(Vector<string>& path, Vector<Vector<string>>& allpaths);
	bool isNotVisited(string city, Queue<Vector<string>>& paths, Vector<string> path);
	void findpaths(Flight subGraph, Query passenger, AdjNode* vertex, Vector<Vector<string>>& allpaths);

};
Flight::Flight() {
	//Opening Flights file...
	head = NULL;
	ifstream fileOpener("Flights.txt");//Opening file.
	string output;//Taking input.
	int day = 0, year = 0, month = 0;
	int hour = 0, minute = 0;

	while (!fileOpener.eof()) {
		int counter = 0;
		Node* newNode = new Node;
		newNode->next = NULL;
		AdjNode* newAdjNode = new AdjNode;
		newAdjNode->next = NULL;

		while (counter < 7) {
			fileOpener >> output;
			if (counter == 0) { //Case for source.
				newNode->source = output;
				newAdjNode->origin = output; //Source and origin are the same things.
			}
			else if (counter == 1) { //Case for destination.
				newNode->dest = output;
			}
			else if (counter == 2) { //Case for date.
				int i = 0, k = 0, checker = 0;
				char* date = new char[5];
				string val;
				while (output[i] != '\0') {
					if (output[i] == '/') {
						checker++;
						date[k] = '\0';
						val = date;
						if (checker == 1) {
							day = stoi(val);
							newNode->date.setDay(day);
						}
						else if (checker == 2) {
							month = stoi(val);
							newNode->date.setMonth(month);
						}
						k = 0;
						delete[] date;
						date = new char[5];
					}
					else {
						date[k] = output[i];
						k++;
					}
					i++;
				}
				date[k] = '\0';
				val = date;
				year = stoi(val);
				newNode->date.setYear(year);
			}
			else if (counter == 3) { //Case for startTime.
				int i = 0, k = 0;
				char* time = new char[3];
				string val;
				while (output[i] != '\0') {
					if (output[i] == ':') {
						time[k] = '\0';
						val = time;
						hour = stoi(val);
						newNode->startTime.setHour(hour);
						k = 0;
						delete[] time;
						time = new char[3];
					}
					else {
						time[k] = output[i];
						k++;
					}
					i++;
				}
				time[k] = '\0';
				val = time;
				minute = stoi(val);
				newNode->startTime.setMinute(minute);
				//cout << newNode->startTime.getHour() << ":" << newNode->startTime.getMinute() << "\t";

			}
			else if (counter == 4) { //Case for endTime.
				int i = 0, k = 0;
				char* time = new char[3];
				string val;
				while (output[i] != '\0') {
					if (output[i] == ':') {
						time[k] = '\0';
						val = time;
						hour = stoi(val);
						newNode->endTime.setHour(hour);
						k = 0;
						delete[] time;
						time = new char[3];
					}
					else {
						time[k] = output[i];
						k++;
					}
					i++;
				}
				time[k] = '\0';
				val = time;
				minute = stoi(val);
				newNode->endTime.setMinute(minute);
				//cout << newNode->endTime.getHour() << ":" << newNode->endTime.getMinute() << endl;
			}
			else if (counter == 5) { //Case for cost or expenses.
				int cost = stoi(output);
				newNode->cost = cost;
			}
			else { //Case for airline.
				newNode->airline = output;
			}
			counter++;
		}

		if (!head) {
			head = newAdjNode; //Case for head=NULL
		}

		AdjNode* temp = head;

		while (temp) {
			if (temp->origin == newNode->source) { //If adjNode origin matches the newNode origin, we will put the newNode in the linked list of the AdjNode destinations object.
				if (!(temp->destinations.head)) {
					temp->destinations.head = newNode; //Case for head = NULL.
					break;
				}
				else {
					Node* nodePtr = temp->destinations.head; // Case for head != NULL, in this case we will put all the nodes at the end of the previous node.
					while (nodePtr->next) {
						nodePtr = nodePtr->next;
					}
					nodePtr->next = newNode;
					break;
				}
			}
			else if (temp->next == NULL) {
				temp->next = newAdjNode;
			}
			temp = temp->next;
		}
	}

	//Closing file...
	fileOpener.close();

	//Opening the HotelCharges file...
	fileOpener.open("HotelCharges_perday.txt");
	int expense;

	while (!fileOpener.eof()) {
		fileOpener >> output; // City
		fileOpener >> expense; // hotelCharges
		AdjNode* temp = head;
		while (temp) {
			Node* nodePtr = temp->destinations.head;
			while (nodePtr) {
				if (nodePtr->dest == output) {
					nodePtr->hotelCharges = expense;
				}
				nodePtr = nodePtr->next;
			}
			temp = temp->next;
		}
	}

	//Closing file...
	fileOpener.close();
}
Flight::Flight(int a)//For subgraph.
{
	head = NULL;
}


void Flight::DrawGraph()
{
	cout << "Vertexes \t        Destinations" << endl;
	AdjNode* tempVertexes = head;
	while (tempVertexes != NULL)
	{
		Node* tempDest = tempVertexes->destinations.head;//Taking head of destinations.
		cout << tempVertexes->origin << "                 ";
		while (tempDest != NULL)
		{
			cout << "(" << tempDest->dest << " " << tempDest->date.getDay() << " " << tempDest->airline << " )" << " -> ";
			tempDest = tempDest->next;
		}
		cout << endl;
		cout << endl;
		tempVertexes = tempVertexes->next;
	}
}

void Flight::DrawGraphDay(int d)
{
	cout << "FLIGHTS FOR THE DATE : " << d << "/12/2019" << endl;
	cout << "Vertexes \t        Destinations" << endl;
	AdjNode* tempVertexes = head;
	while (tempVertexes != NULL)
	{
		Node* tempDest = tempVertexes->destinations.head;
		cout << tempVertexes->origin << "                 ";
		while (tempDest != NULL)
		{
			if (d == tempDest->date.getDay())
			{
				cout << tempDest->dest << " -> ";
			}
			tempDest = tempDest->next;
		}
		cout << endl;
		cout << endl;
		tempVertexes = tempVertexes->next;
	}
}

List Flight::DirectFlights(const Query& passenger) {
	List Flights;
	bool available = false; //Used for the availability of fligts.
	AdjNode* temp = head;
	while (temp) {
		if (temp->origin == passenger.getOrigin()) {
			Node* nodePtr = temp->destinations.head;
			while (nodePtr) {
				if (nodePtr->dest == passenger.getDestination()) { //If it matches Query's date.
					if ((nodePtr->date.getDay() == passenger.getDay())) {
						Node* newNode = new Node;
						newNode->Intialize(*nodePtr);//Is like a copy constructor.
						newNode->next = NULL;
						if (!(Flights.head)) {
							Flights.head = newNode;
						}
						else {
							Node* curr = Flights.head;
							while (curr->next) {
								curr = curr->next;
							}
							curr->next = newNode;
						}
						available = true;
					}
				}
				nodePtr = nodePtr->next;
			}
		}
		temp = temp->next;
	}

	if (!available) { //If there is no flight available on the specified date.
		temp = head;
		while (temp) {
			if (temp->origin == passenger.getOrigin()) {
				Node* nodePtr = temp->destinations.head;
				while (nodePtr) {
					if (nodePtr->dest == passenger.getDestination()) {
						if ((nodePtr->date.getDay() == passenger.getDay() + 1) || (nodePtr->date.getDay() == passenger.getDay() - 1)) {
							Node* newNode = new Node;
							newNode->Intialize(*nodePtr);
							newNode->next = NULL;
							if (!(Flights.head)) {
								Flights.head = newNode;
							}
							else {
								Node* curr = Flights.head;
								while (curr->next) {
									curr = curr->next;
								}
								curr->next = newNode;
							}
						}
					}
					nodePtr = nodePtr->next;
				}
			}
			temp = temp->next;
		}
	}
	return Flights;
}

//Breadth First Traversal.
//We have subgraph, we have passenger's query, 
void Flight::findpaths(Flight subGraph, Query passenger, AdjNode* vertex, Vector<Vector<string>>& allpaths)
{
	Queue <Vector<string>> queue;//Stores combinations of path in queue.
	Vector<string> path;//Puts path we have found in queue.
	path.push(passenger.getOrigin());//Pushes origin of passenger in our path.
	queue.enqueue(path);
	while (queue.size != 0)
	{
		//cout << "\nQueue is\n";
		//queue.display();
		//cout << "\n\nvalue of queue front\n";
		//queue.front().display();
		//cout << "\n\nPath before copying\n";
		//path.display();
		path = queue.front();//Returns whole path stored in queue. Can be multiple cities.
		//cout << "\n\nPath after copying\n";
		//path.display();
		queue.dequeue();//Removes the path from queue.
		//cout << "\n\nQueue after dequeue\n";
		//queue.display();
		string last = path[path.Size() - 1];

		if (last == passenger.getDestination())//If that path leads to our destination, we store that path in our allpaths.
			savepath(path, allpaths);

		if (last == passenger.getDestination())
			continue;

		Node* city = nullptr;
		vertex = subGraph.head;
		bool not_found = false;//For basically sydney.
		while (vertex->origin != last)//Reaching that city from vertices.
		{
			vertex = vertex->next;
			if (vertex == nullptr)
			{
				not_found = true;
				break;
			}
		}
		if (not_found)
			continue;
		city = vertex->destinations.head;
		while (city != nullptr)//Now checking all destinations of that city.
		{
			//cout << "\n\nCheck " << city->dest << " in \n";
			//queue.display();
			bool isnotvisited = isNotVisited(city->dest, queue, path); // might have to change this
			if (isnotvisited)
			{
				Vector<string> newpath = path;//We make copy of existing path.
				//cout << "\n\nNew Path before\n";
				//newpath.display();
				newpath.push(city->dest);//We push that city which wasn't visited into this new path.
				//cout << "\n\nNew Path after\n";
				//newpath.display();
				queue.enqueue(newpath);//Then we enqueue this new path into our queue.
				//cout << "\n\nQueue right now \n";
				//queue.display();
			}
			city = city->next;
		}
	}
}



void Flight::savepath(Vector<string>& path, Vector<Vector<string>>& allpaths)
{
	allpaths.push(path);
}


bool Flight::isNotVisited(string city, Queue<Vector<string>>& all_paths, Vector<string> path)
{
	for (int i = 0; i < path.Size(); i++)
	{
		if (path[i] == city)//If that city is found already in path.
			return false;
	}
	int last_idx = 0;
	for (int i = 0; i < all_paths.size; i++)//Duplicate paths check.
	{
		if (all_paths[i].Size() <= path.Size())
			continue;
		Vector<string> path_in_queue = all_paths[i];
		bool match = true;
		for (int j = 0; j < path.Size(); j++)
		{
			if (path_in_queue[j] != path[j])
			{
				match = false;
				break;
			}
		}
		if (match == true)
		{
			for (int x = 0; x < path_in_queue.Size(); x++)
				if (path_in_queue[x] == city)
					return false;
		}
		else
			continue;
	}
	return true;
}

void Flight::createSubgraph(Flight& subGraph, int date, string airline)
{
	AdjNode* tempVertexes = head;
	Node* nodePtr = head->destinations.head;
	while (tempVertexes != NULL)
	{
		AdjNode* newAdjNode = new AdjNode();
		newAdjNode->next = NULL;
		newAdjNode->origin = tempVertexes->origin;
		if (subGraph.head == NULL)
		{
			subGraph.head = newAdjNode;
			while (nodePtr != NULL)
			{
				if (nodePtr->date.getDay() >= date && nodePtr->airline == airline)//AFFAN BROTER ONLY CHANGE DIS.... PUT RANGE LIKE 3-4 DAYS MAX.
				{
					Node* newNode = new Node;
					newNode->Intialize(*nodePtr);
					subGraph.head->destinations.push(*newNode);
				}
				nodePtr = nodePtr->next;
			}
		}
		else
		{
			AdjNode* cursor = subGraph.head;
			while (cursor->next != NULL)
			{
				cursor = cursor->next;
			}
			cursor->next = newAdjNode;
			while (nodePtr != NULL)
			{
				if (nodePtr->date.getDay() >= date && nodePtr->airline == airline)//AFFAN BROTHER ONLY CHANGE DIS TOO. PUT RANGE LIKE 3-4 DAYS.
				{
					Node* newNode = new Node;
					newNode->Intialize(*nodePtr);
					cursor->next->destinations.push(*newNode);
				}
				nodePtr = nodePtr->next;
			}
		}
		tempVertexes = tempVertexes->next;
		if (tempVertexes != NULL)
		{
			nodePtr = tempVertexes->destinations.head;
		}
	}
}


Node* Flight::getDestinations(AdjNode* vertex, string city)//Goes to city,  and returns it's destinations head.
{
	while (vertex != nullptr)
	{
		if (vertex->origin == city)
			return vertex->destinations.head;
		vertex = vertex->next;
	}
	return nullptr;
}

int Flight::calculate_flightcost(Vector<Node> flight)
{
	int cost = 0;
	int transit_cost;
	int tr_time;
	for (int i = 0; i < flight.Size(); i++)
	{
		cost += flight[i].cost;
		if (i < flight.Size() - 1)//If not last flight.
		{
			tr_time = transit_time(flight[i], flight[i + 1]);
			if (tr_time > 720)//12 hours. (minutes)
			{
				transit_cost = flight[i].hotelCharges;
				int days = (tr_time / (24 * 60));
				if (tr_time % (24 * 60) != 0)
					days++;
				cost += (transit_cost * days);
			}
		}
	}
	return cost;
}

Time Flight::calculate_flighttime(Vector<Node> flight)
{
	Time total_time;
	Time flight_time;
	int time = 0;
	int tr_time = 0;
	for (int i = 0; i < flight.Size(); i++)
	{
		flight_time = flight[i].calculateTime();//Travel time.
		time += (flight_time.getHour() * 60) + flight_time.getMinute();//Minutes.
		if (i < flight.Size() - 1)
		{
			tr_time = transit_time(flight[i], flight[i + 1]);
			time += tr_time;
		}
	}
	total_time.setHour(time / 60);
	total_time.setMinute(time % 60);

	return total_time;
}







void Flight::scenario1(const Query& passenger) {
	//Creating subgraph.
	Flight subGraph(5); // this 5 means nothing.
	createSubgraph(subGraph, passenger.getDay(), passenger.getAirline());
	cout << "Our Date and airline subgraph is : " << endl;
	subGraph.DrawGraph();

	//  N E W   I M P L E M E N T A T I O N

	AdjNode* vertex = subGraph.head;
	Vector<Vector<string>> allpaths;
	findpaths(subGraph, passenger, vertex, allpaths);

	// THESE PATHS ARE FILTERED ACCORDING TO DATE AND AIRLINE.
	if (allpaths.Size() != 0)
	{
		"\nDisplaying possible paths\n";
		for (int i = 0; i < allpaths.Size(); i++)
		{
			cout << "\nPath no: " << i + 1 << endl;
			allpaths[i].display();
			cout << endl;
		}
	}
	else
	{
		cout << "\nNo possible flights\n";
		return;
	}

	// STORING POSSIBLE FLIGHTS
	Vector<Vector<Node>> flights; // to save possible flights --- not sorted.
	for (int i = 0; i < allpaths.Size(); i++)
	{
		Vector<string> curr_path = allpaths[i];//A row containing path is put in current path.
		//cout << "\n\nCurrent Path is\n";// Delete comment later.
		curr_path.display();

		// ASSISTANT VARIABLES TO MAKE FLIGHTS
		Vector<Vector<Node>> temp_flights;
		string city;
		string nextcity;
		Node* nodeptr = nullptr;
		AdjNode* vertex = subGraph.head;

		for (int j = 0; j < curr_path.Size() - 1; j++)
		{
			Vector<Node> temp_cities; // this will be pushed in temp_flights
			city = curr_path[j];//The flight is put.
			nextcity = curr_path[j + 1];//The next flight is put.

			nodeptr = getDestinations(vertex, city);
			while (nodeptr != nullptr)
			{
				if (nodeptr->dest == nextcity)
					temp_cities.push(*nodeptr);//We look for the next flight... and store it.
				nodeptr = nodeptr->next;
			}
			//cout << "\n\nTemp_cities being pushed\n";//Delete comment later.
			temp_cities.display();
			temp_flights.push(temp_cities);
			//cout << "\nTemp flights now has\n";//Delete comment later.
			for (int i = 0; i < temp_flights.Size(); i++)
			{
				temp_flights[i].display();
				cout << endl;
			}
		}

		// F I N A L   P A T H S


		int n = temp_flights.Size();


		int* indices = new int[n];


		for (int i = 0; i < n; i++)
			indices[i] = 0;


		while (1) {
			Vector<Node> temp_path;//Each path is a unique path.

			for (int i = 0; i < n; i++)
				temp_path.push(temp_flights[i][indices[i]]);
			//cout << arr[i][indices[i]] << " ";
			//cout << "\nTemp path right now: ";
			//temp_path.display();
			bool push = true;
			int city1_date = 0, city2_date = 0;
			int city1_time = 0, city2_time = 0;
			for (int i = 0; i < temp_path.Size() - 1; i++)
			{
				city1_date = temp_path[i].date.getDay();
				city2_date = temp_path[i + 1].date.getDay();
				city1_time = (temp_path[i].endTime.getHour() * 60) + temp_path[i].endTime.getMinute();
				city2_time = (temp_path[i + 1].startTime.getHour() * 60) + temp_path[i + 1].startTime.getMinute();
				if (city2_date < city1_date)//Cannot store flight of previous days... Day 4 to 6 is possible but not day 6 to 4...
				{
					push = false;
					break;
				}
				else
				{
					if (city1_date == city2_date)
					{
						if (city2_time < city1_time)//Time must be lesser than initial time.
						{
							push = false;
							break;
						}
					}
				}

			}
			if (push == true)
				flights.push(temp_path);//We push that unique path into our flights variable.
			//cout << endl;


			int next = n - 1;
			while (next >= 0 &&
				(indices[next] + 1 >= temp_flights[next].Size()))
				next--;


			if (next < 0)
				break;


			indices[next]++;


			for (int i = next + 1; i < n; i++)
				indices[i] = 0;
		}
		if (flights.Size() == 0)
		{
			cout << "No possible flights!" << endl;
		}
		else
		{
			cout << "\n\nDisplaying possible flights from " << passenger.getOrigin() << endl;
			for (int i = 0; i < flights.Size(); i++)
			{
				cout << "\nFlight no: " << i + 1 << endl;
				flights[i].display();
				cout << endl;
			}
		}
	}

	//===================== W H A T   A L I    N E E D S   T O   D O ====================================================
	// the variable "flight" has all the possible flights according to date and airline									=
	// You need to sort the flights below these comments																=
	// To access a single flight use "flight[index]"																	=
	// you can save it like this Vector<Node> cost_flight = flight[index]												=
	// you can access every city present in cost_flight by writing  cost_flight[idx].dest or cost_flight[idx].time etc	=
	// if flights are on same day put a check that next flight time is greater than prev flight arrival time before disp=
	// JUST SORT IT LIKE AN ARRAY AND DISPLAY ACCORDINGLY																=
	//===================================================================================================================
	cout << endl;
	cout << "Flights size is : " << flights.Size();
	cout << endl;
	if (flights.Size() > 1) //Flights will be sorted if they are greater than 1 atleast.
	{

		for (int i = 1; i < flights.Size(); i++)//Bubble sort. Complexity is : Comparison(n^2), Swaps (n^2).
		{
			for (int j = 0; j < flights.Size() - i; j++)
			{
				cout << "Checking flights " << j + 1 << " and " << j + 2 << endl;
				Time totaltime1 = calculate_flighttime(flights[j]);
				Time totaltime2 = calculate_flighttime(flights[j + 1]);

				cout << "Total time of flight " << j + 1 << " is : ";
				totaltime1.displayTime();
				cout << "  ";
				cout << endl;

				cout << "Total time of flight " << j + 2 << " is : ";
				totaltime2.displayTime();
				cout << "  ";
				cout << endl;

				int cost1 = calculate_flightcost(flights[j]);
				int cost2 = calculate_flightcost(flights[j + 1]);

				cout << "Total cost of flight " << j + 1 << " is : " << cost1 << endl;
				cout << "Total cost of flights " << j + 2 << " is : " << cost2 << endl;

				if (cost1 > cost2)//Swapping.
				{
					cout << "Now swapping because of cost ... " << endl;
					Vector<Node> temp = flights[j];
					flights[j] = flights[j + 1];
					flights[j + 1] = temp;
				}
				cout << endl;
				cout << endl;
				cout << endl;
			}
		}

	}
	if (flights.Size() >= 1)
	{
		cout << "Flight paths according to travel cost are : " << endl;
		for (int i = 0; i < flights.Size(); i++)
		{
			cout << "\nFlight no: " << i + 1 << endl;
			flights[i].display();
			cout << endl;
		}
	}

}

void Flight::createSubgraph(Flight& subGraph, int date)//Without airline.
{
	AdjNode* tempVertexes = head;
	Node* nodePtr = head->destinations.head;
	while (tempVertexes != NULL)
	{
		AdjNode* newAdjNode = new AdjNode();
		newAdjNode->next = NULL;
		newAdjNode->origin = tempVertexes->origin;
		if (subGraph.head == NULL)
		{
			subGraph.head = newAdjNode;
			while (nodePtr != NULL)
			{
				if (nodePtr->date.getDay() >= date)
				{
					Node* newNode = new Node;
					newNode->Intialize(*nodePtr);
					subGraph.head->destinations.push(*newNode);
				}
				nodePtr = nodePtr->next;
			}
		}
		else
		{
			AdjNode* cursor = subGraph.head;
			while (cursor->next != NULL)
			{
				cursor = cursor->next;
			}
			cursor->next = newAdjNode;
			while (nodePtr != NULL)
			{
				if (nodePtr->date.getDay() >= date)
				{
					Node* newNode = new Node;
					newNode->Intialize(*nodePtr);
					cursor->next->destinations.push(*newNode);
				}
				nodePtr = nodePtr->next;
			}
		}
		tempVertexes = tempVertexes->next;
		if (tempVertexes != NULL)
		{
			nodePtr = tempVertexes->destinations.head;
		}
	}
}








void Flight::scenario2(const Query& passenger)
{
	//Creating subgraph.
	Flight subGraph(5); // this 5 means nothing 
	createSubgraph(subGraph, passenger.getDay());
	cout << " Our Date and airline subgraph is : " << endl;
	subGraph.DrawGraph();

	//  N E W   I M P L E M E N T A T I O N

	AdjNode* vertex = subGraph.head;
	Vector<Vector<string>> allpaths;
	findpaths(subGraph, passenger, vertex, allpaths);

	// THESE PATHS ARE FILTERED ACCORDING TO DATE AND AIRLINE
	if (allpaths.Size() != 0)
	{
		"\nDisplaying possible paths\n";
		for (int i = 0; i < allpaths.Size(); i++)
		{
			cout << "\nPath no: " << i + 1 << endl;
			allpaths[i].display();
			cout << endl;
		}
	}
	else
	{
		cout << "\nNo possible flights\n";
		return;
	}

	// STORING POSSIBLE FLIGHTS
	Vector<Vector<Node>> flights; // to save possible flights --- not sorted
	for (int i = 0; i < allpaths.Size(); i++)
	{
		Vector<string> curr_path = allpaths[i];
		//cout << "\n\nCurrent Path is\n";//Delete comment later.
		curr_path.display();

		// ASSISTANT VARIABLES TO MAKE FLIGHTS
		Vector<Vector<Node>> temp_flights;
		string city;
		string nextcity;
		Node* nodeptr = nullptr;
		AdjNode* vertex = subGraph.head;

		for (int j = 0; j < curr_path.Size() - 1; j++)
		{
			Vector<Node> temp_cities; // this will be pushed in temp_flights
			city = curr_path[j];
			nextcity = curr_path[j + 1];

			nodeptr = getDestinations(vertex, city);
			while (nodeptr != nullptr)
			{
				if (nodeptr->dest == nextcity)
					temp_cities.push(*nodeptr);
				nodeptr = nodeptr->next;
			}
			//cout << "\n\nTemp_cities being pushed\n";//Delete comment later.
			temp_cities.display();
			temp_flights.push(temp_cities);
			//cout << "\nTemp flights now has\n";
			for (int i = 0; i < temp_flights.Size(); i++)
			{
				temp_flights[i].display();
				cout << endl;
			}
		}

		// F I N A L   P A T H S


		int n = temp_flights.Size();


		int* indices = new int[n];


		for (int i = 0; i < n; i++)
			indices[i] = 0;


		while (1) {
			Vector<Node> temp_path;

			for (int i = 0; i < n; i++)
				temp_path.push(temp_flights[i][indices[i]]);
			//cout << arr[i][indices[i]] << " ";
			//cout << "\nTemp path right now: ";
			//temp_path.display();
			bool push = true;
			int city1_date = 0, city2_date = 0;
			int city1_time = 0, city2_time = 0;
			for (int i = 0; i < temp_path.Size() - 1; i++)
			{
				city1_date = temp_path[i].date.getDay();
				city2_date = temp_path[i + 1].date.getDay();
				city1_time = (temp_path[i].endTime.getHour() * 60) + temp_path[i].endTime.getMinute();
				city2_time = (temp_path[i + 1].startTime.getHour() * 60) + temp_path[i + 1].startTime.getMinute();
				if (city2_date < city1_date)
				{
					push = false;
					break;
				}
				else
				{
					if (city1_date == city2_date)
					{
						if (city2_time < city1_time)
						{
							push = false;
							break;
						}
					}
				}

			}
			if (push == true)
				flights.push(temp_path);
			//cout << endl;


			int next = n - 1;
			while (next >= 0 &&
				(indices[next] + 1 >= temp_flights[next].Size()))
				next--;


			if (next < 0)
				break;


			indices[next]++;


			for (int i = next + 1; i < n; i++)
				indices[i] = 0;
		}

		if (flights.Size() == 0)
		{
			cout << "No possible flights!" << endl;
		}
		else
		{
			cout << "\n\nDisplaying possible flights from " << passenger.getOrigin() << endl;
			for (int i = 0; i < flights.Size(); i++)
			{
				cout << "\nFlight no: " << i + 1 << endl;
				flights[i].display();
				cout << endl;
			}
		}

	}



	//S O R T I N G . . . .

	cout << endl;
	cout << "Flights size is : " << flights.Size();
	cout << endl;
	if (flights.Size() > 1) //Flights will be sorted if they are greater than 1 atleast.
	{
		for (int i = 1; i < flights.Size(); i++)//Bubble sort.
		{
			for (int j = 0; j < flights.Size() - i; j++)
			{
				cout << "Checking flights " << j + 1 << " and " << j + 2 << endl;
				Time totaltime1 = calculate_flighttime(flights[j]);
				Time totaltime2 = calculate_flighttime(flights[j + 1]);

				cout << "Total time of flight " << j + 1 << " is : ";
				totaltime1.displayTime();
				cout << "  ";
				cout << endl;

				cout << "Total time of flight " << j + 2 << " is : ";
				totaltime2.displayTime();
				cout << "  ";
				cout << endl;

				int cost1 = calculate_flightcost(flights[j]);
				int cost2 = calculate_flightcost(flights[j + 1]);

				cout << "Total cost of flight " << j + 1 << " is : " << cost1 << endl;
				cout << "Total cost of flights " << j + 2 << " is : " << cost2 << endl;
				if (totaltime1 > totaltime2)//Swapping.
				{
					cout << "Now swapping because of time ... " << endl;
					Vector<Node> temp = flights[j];
					flights[j] = flights[j + 1];
					flights[j + 1] = temp;
				}
				cout << endl;
				cout << endl;
				cout << endl;

			}
		}

	}
	if (flights.Size() >= 1)
	{
		cout << "Flight paths according to travel time are : " << endl;
		for (int i = 0; i < flights.Size(); i++)
		{
			cout << "\nFlight no: " << i + 1 << endl;
			flights[i].display();
			cout << endl;
		}
	}

}

void Flight::scenario3(const Query& passenger)
{
	// FIND FLIGHTS TILL DESTINATION
	Flight subGraph(5); // this 5 means nothing 
	createSubgraph(subGraph, passenger.getDay());
	cout << " Our Date and airline subgraph is : " << endl;
	subGraph.DrawGraph();

	//  N E W   I M P L E M E N T A T I O N

	AdjNode* vertex = subGraph.head;
	Vector<Vector<string>> allpaths;
	findpaths(subGraph, passenger, vertex, allpaths);

	if (allpaths.Size() != 0)
	{
		"\nDisplaying possible paths\n";
		for (int i = 0; i < allpaths.Size(); i++)
		{
			cout << "\nPath no: " << i + 1 << endl;
			allpaths[i].display();
			cout << endl;
		}
	}
	else
	{
		cout << "\nNo possible flights\n";
		return;
	}

	// STORING POSSIBLE FLIGHTS
	Vector<Vector<Node>> flights; // to save possible flights --- not sorted
	for (int i = 0; i < allpaths.Size(); i++)
	{
		Vector<string> curr_path = allpaths[i];
		//cout << "\n\nCurrent Path is\n";
		//curr_path.display();

		// ASSISTANT VARIABLES TO MAKE FLIGHTS
		Vector<Vector<Node>> temp_flights;
		string city;
		string nextcity;
		Node* nodeptr = nullptr;
		AdjNode* vertex = subGraph.head;

		for (int j = 0; j < curr_path.Size() - 1; j++)
		{
			Vector<Node> temp_cities; // this will be pushed in temp_flights
			city = curr_path[j];
			nextcity = curr_path[j + 1];

			nodeptr = getDestinations(vertex, city);
			while (nodeptr != nullptr)
			{
				if (nodeptr->dest == nextcity)
					temp_cities.push(*nodeptr);
				nodeptr = nodeptr->next;
			}
			
			temp_flights.push(temp_cities);
		}

		// F I N A L   P A T H S

		// number of arrays 
		int n = temp_flights.Size();


		int* indices = new int[n];


		for (int i = 0; i < n; i++)
			indices[i] = 0;


		while (1) {
			Vector<Node> temp_path;

			for (int i = 0; i < n; i++)
				temp_path.push(temp_flights[i][indices[i]]);
			bool push = true;
			int city1_date = 0, city2_date = 0;
			int city1_time = 0, city2_time = 0;
			for (int i = 0; i < temp_path.Size() - 1; i++)
			{
				city1_date = temp_path[i].date.getDay();
				city2_date = temp_path[i + 1].date.getDay();
				city1_time = (temp_path[i].endTime.getHour() * 60) + temp_path[i].endTime.getMinute();
				city2_time = (temp_path[i + 1].startTime.getHour() * 60) + temp_path[i + 1].startTime.getMinute();
				if (city2_date < city1_date)
				{
					push = false;
					break;
				}
				else
				{
					if (city1_date == city2_date)
					{
						if (city2_time < city1_time)
						{
							push = false;
							break;
						}
					}
				}

			}
			if (push == true)
				flights.push(temp_path);


			int next = n - 1;
			while (next >= 0 &&
				(indices[next] + 1 >= temp_flights[next].Size()))
				next--;


			if (next < 0)
				break;


			indices[next]++;


			for (int i = next + 1; i < n; i++)
				indices[i] = 0;
		}
	}

	if (flights.Size() == 0)
	{
		cout << "No possible flights!" << endl;
	}
	else
	{
		cout << "\n\nDisplaying possible flights from " << passenger.getOrigin() << endl;
		for (int i = 0; i < flights.Size(); i++)
		{
			cout << "\nFlight no: " << i + 1 << endl;
			flights[i].display();
			cout << endl;
		}
	}

	// CHECK FOR TRANSIT LOCATION
	string transit;
	cout << "\t\t Enter Transit Location: ";
	cin >> transit;

	if (flights.Size() == 0)
	{
		cout << "No possible flights!" << endl;
	}
	else
	{
		cout << "\n\nDisplaying possible flights from " << passenger.getOrigin() << " including " << transit << endl;
		cout << "Date of deprarture: " << passenger.getDay() << "/12/2019\n";
		int flight_no = 1;
		Time total_time;
		for (int i = 0; i < flights.Size(); i++)
		{
			for (int j = 0; j < flights[i].Size(); j++)
			{
				if (flights[i][j].dest == transit)
				{
					cout << "\nFlight no: " << flight_no << endl;
					flights[i].display();
					cout << "\nCost: " << calculate_flightcost(flights[i]);
					total_time = calculate_flighttime(flights[i]);
					cout << "\nTravel time: " << total_time.getHour() << " hrs " << total_time.getMinute() << " mins";
					flight_no++;
					break;
				}
			}
		}
		cout << endl;
	}
}

void Flight::scenario4(const Query& passenger)
{
	// FIND FLIGHTS TILL DESTINATION
	Flight subGraph(5); // this 5 means nothing 
	createSubgraph(subGraph, passenger.getDay());
	cout << " Our Date and airline subgraph is : " << endl;
	subGraph.DrawGraph();

	//  N E W   I M P L E M E N T A T I O N

	AdjNode* vertex = subGraph.head;
	Vector<Vector<string>> allpaths;
	findpaths(subGraph, passenger, vertex, allpaths);

	if (allpaths.Size() != 0)
	{
		"\nDisplaying possible paths\n";
		for (int i = 0; i < allpaths.Size(); i++)
		{
			cout << "\nPath no: " << i + 1 << endl;
			allpaths[i].display();
			cout << endl;
		}
	}
	else
	{
		cout << "\nNo possible flights\n";
		return;
	}

	// STORING POSSIBLE FLIGHTS
	Vector<Vector<Node>> flights; // to save possible flights --- not sorted
	for (int i = 0; i < allpaths.Size(); i++)
	{
		Vector<string> curr_path = allpaths[i];

		// ASSISTANT VARIABLES TO MAKE FLIGHTS
		Vector<Vector<Node>> temp_flights;
		string city;
		string nextcity;
		Node* nodeptr = nullptr;
		AdjNode* vertex = subGraph.head;

		for (int j = 0; j < curr_path.Size() - 1; j++)
		{
			Vector<Node> temp_cities; // this will be pushed in temp_flights
			city = curr_path[j];
			nextcity = curr_path[j + 1];

			nodeptr = getDestinations(vertex, city);
			while (nodeptr != nullptr)
			{
				if (nodeptr->dest == nextcity)
					temp_cities.push(*nodeptr);
				nodeptr = nodeptr->next;
			}
			temp_flights.push(temp_cities);
		}

		// F I N A L   P A T H S

		// number of arrays 
		int n = temp_flights.Size();


		int* indices = new int[n];


		for (int i = 0; i < n; i++)
			indices[i] = 0;


		while (1) {
			Vector<Node> temp_path;

			for (int i = 0; i < n; i++)
				temp_path.push(temp_flights[i][indices[i]]);
			bool push = true;
			int city1_date = 0, city2_date = 0;
			int city1_time = 0, city2_time = 0;
			for (int i = 0; i < temp_path.Size() - 1; i++)
			{
				city1_date = temp_path[i].date.getDay();
				city2_date = temp_path[i + 1].date.getDay();
				city1_time = (temp_path[i].endTime.getHour() * 60) + temp_path[i].endTime.getMinute();
				city2_time = (temp_path[i + 1].startTime.getHour() * 60) + temp_path[i + 1].startTime.getMinute();
				if (city2_date < city1_date)
				{
					push = false;
					break;
				}
				else
				{
					if (city1_date == city2_date)
					{
						if (city2_time < city1_time)
						{
							push = false;
							break;
						}
					}
				}

			}
			if (push == true)
				flights.push(temp_path);


			int next = n - 1;
			while (next >= 0 &&
				(indices[next] + 1 >= temp_flights[next].Size()))
				next--;


			if (next < 0)
				break;


			indices[next]++;


			for (int i = next + 1; i < n; i++)
				indices[i] = 0;
		}
	}

	if (flights.Size() == 0)
	{
		cout << "No possible flights!" << endl;
	}
	else
	{
		cout << "\n\nDisplaying possible flights from " << passenger.getOrigin() << endl;
		for (int i = 0; i < flights.Size(); i++)
		{
			cout << "\nFlight no: " << i + 1 << endl;
			flights[i].display();
			cout << endl;
		}
	}

	// T R A N S I T


	string transit;
	int min_hrs = 0;
	int min_mins = 0;
	int max_hrs = 0;
	int max_mins = 0;

	int min_duration = 0;
	int max_duration = 0;

	cout << "\t\tEnter Transit Location: ";
	cin >> transit;

	cout << "\t\tPlease enter the minimum hours followed by miniutes you want to stay ";
	cin >> min_hrs;
	cin >> min_mins;
	min_duration = min_hrs * 60 + min_mins;

	cout << "\t\tPlease enter the maximum hours followed by miniutes you want to stay ";
	cin >> max_hrs;
	cin >> max_mins;
	max_duration = max_hrs * 60 + max_mins;



	if (flights.Size() == 0)
	{
		cout << "No possible flights!" << endl;
	}
	else
	{
		cout << "\n\nDisplaying possible flights from " << passenger.getOrigin() << " including " << transit << endl;
		cout << "Date of deprarture: " << passenger.getDay() << "/12/2019\n";
		int flight_no = 1;
		Time total_time;
		for (int i = 0; i < flights.Size(); i++)
		{
			for (int j = 0; j < flights[i].Size(); j++)
			{
				if (flights[i][j].dest == transit)//First see if transit location exists in path.
				{
					if (j < flights[i].Size() - 1)//Then see transit time.
					{
						int tr_time = transit_time(flights[i][j], flights[i][j + 1]);
						if (tr_time >= min_duration and tr_time <= max_duration)
						{
							cout << "\nFlight no: " << flight_no << endl;
							flights[i].display();
							cout << "\nCost: " << calculate_flightcost(flights[i]);
							total_time = calculate_flighttime(flights[i]);
							cout << "\nTravel time: " << total_time.getHour() << " hrs " << total_time.getMinute() << " mins";
							flight_no++;
							break;
						}
					}
				}
			}
		}
		cout << endl;
	}
}




void Flight::scenario5(const Query& passenger) {
	List direct;

	direct = DirectFlights(passenger);

	cout << "Available Flights: " << endl;
	Node* traversal = direct.head;
	while (traversal) {
		cout << traversal->source << " " << traversal->dest << " " << traversal->cost << " " << traversal->airline << " ";
		cout << traversal->date.getDay() << " " << traversal->date.getMonth() << " " << traversal->date.getYear() << endl;
		traversal = traversal->next;
	}

}


int Flight::minTime(Vector<Node> v1, int day)//Didnt use anywhere.
{
	int mintime = 99999;
	for (int i = 0; i < v1.Size(); i++)
	{
		if (v1[i].endTime.getHour() < mintime && v1[i].date.getDay() == day)
			mintime = v1[i].endTime.getHour();
	}
	return mintime;
}

int Flight::transit_time(Node city1, Node city2)//City one is the city where we stay. Returns time in minutes.
{
	// Will return time back in miniutes
	int city1_hrs = city1.endTime.getHour();
	int city1_min = city1.endTime.getMinute();
	int city1_date = city1.date.getDay();
	int city1_time = city1_hrs * 60 + city1_min;

	int city2_hrs = city2.startTime.getHour();
	int city2_min = city2.startTime.getMinute();
	int city2_date = city2.date.getDay();
	int city2_time = city2_hrs * 60 + city2_min;

	if (city1_date == city2_date) // if both flights are on same date
	{
		return city2_time - city1_time;
	}
	else // 2nd flight will be in upcoming days.
	{
		int days_between = city2_date - city1_date - 1;
		int time_to_add = 0;
		time_to_add = (24 * 60) - city1_time + city2_time;
		int total_time = (days_between * 24 * 60) + time_to_add;
		return total_time;
	}
	//start	 end		
// Day 4: 8:00   -   10:00
// Day 6: 10:00  -   12:00
}

void Flight::menu() {
	
	Query passenger;
	string input;
	int day;
	int choice = -1;
	cout << "\t\t AIRLINE RESERVATION SYSTEM \t " << endl << endl << endl;
	while (choice != 0)
	{
		cout << "\t\t 1- Travel Cost Flights \t " << endl;
		cout << "\t\t 2- Travel Time Flights \t " << endl;
		cout << "\t\t 3- Transit Stay Flights \t " << endl;
		cout << "\t\t 4- Transit Time Flights \t " << endl;
		cout << "\t\t 5- Direct Flights \t " << endl;
		cout << "\t\t 0- Exit \t " << endl;
		cout << "Enter your choice: ";
		cin >> choice;

		switch (choice) {
		case 1:
			cout << "\t\t Enter your Origin: ";
			cin >> input;
			passenger.setOrigin(input);
			cout << "\t\t Enter your Destination: ";
			cin >> input;
			passenger.setDestination(input);
			cout << "\t\t Enter Airline: ";
			cin >> input;
			passenger.setAirline(input);
			cout << "\t\t Enter Day of Travel: ";
			cin >> day;
			passenger.setDay(day);

			scenario1(passenger);
			break;
		case 2:
			cout << "\t\t Enter your Origin: ";
			cin >> input;
			passenger.setOrigin(input);
			cout << "\t\t Enter your Destination: ";
			cin >> input;
			passenger.setDestination(input);
			cout << "\t\t Enter Day of Travel: ";
			cin >> day;
			passenger.setDay(day);

			scenario2(passenger);
			break;
		case 3:
			cout << "\t\t Enter your Origin: ";
			cin >> input;
			passenger.setOrigin(input);
			cout << "\t\t Enter your Destination: ";
			cin >> input;
			passenger.setDestination(input);
			cout << "\t\t Enter Day of Travel: ";
			cin >> day;
			passenger.setDay(day);

			scenario3(passenger);
			break;
		case 4:
			cout << "\t\t Enter your Origin: ";
			cin >> input;
			passenger.setOrigin(input);
			cout << "\t\t Enter your Destination: ";
			cin >> input;
			passenger.setDestination(input);
			cout << "\t\t Enter Day of Travel: ";
			cin >> day;
			passenger.setDay(day);

			scenario4(passenger);
			break;
		case 5:
			cout << "\t\t Enter your Origin: ";
			cin >> input;
			passenger.setOrigin(input);
			cout << "\t\t Enter your Destination: ";
			cin >> input;
			passenger.setDestination(input);
			cout << "\t\t Enter Day of Travel: ";
			cin >> day;
			passenger.setDay(day);

			scenario5(passenger);
			break;
		case 0:
			cout << "Good bye!" << endl;
			break;
		default:
			cout << "Wrong choice." << endl;
		}
	}

}
