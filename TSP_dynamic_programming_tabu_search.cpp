// Wojciech Moruñ 


#include <stdio.h>
#include "stdafx.h"
#include <iostream>
#include <chrono>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <map>
using namespace std;
using  ns = chrono::nanoseconds;
using get_time = chrono::steady_clock;
int optimum = INT_MAX;
int rows = 0; //zmienna pomocnicza przy przepisywaniu wektorów
vector< vector<int> > distances; // dwuwymiarowa dynamiczna tablica, do przechowywania informacji o odleglosciach
vector<int> row; // 1-wym. dynamiczna tablica pomocnicza, do wczytania danych z pliku
string temp; // pomocniczy string do wczytania info z pliku
int step = 0;
int current_cost;
int myrandom(int i) { return std::rand() % i; }


class Stopwatch {
private:
	clock_t tstart;
	clock_t tstop;
public:
	double time;
	Stopwatch::Stopwatch() {

	}
	void Stopwatch::start() {
		tstart = clock();
		time = 0;
	}
	void Stopwatch::stop() {
		tstop = clock();
		time = ((double)(tstop - tstart)) / CLOCKS_PER_SEC;
	}
	double Stopwatch::read() {
		return time;
	}
};

void readDataSym(string name){
	row.clear();
	distances.clear();
	fstream plik; // deklaracja pliku
	plik.open(name, ios::in); // plik, póki co tylko symetryczne macierzowe

	while (!plik.eof()){		// trzeba while, dla fora trzeba by by³o zrobiæ sztuczny warunek stopu, nie fajnie
		getline(plik, temp, ' ');	// po kolei ka¿dy 'wyraz' do stringa, wyrazy z pliku oddzielone spacj¹, w pliku wyrzucony wczesniej opis
		row.push_back(atoi(temp.c_str())); // konwersja na int, wrzucenie do tablicy
	}
	distances.resize(distances.size() + 1);  // deklaracja rozmiaru dwuwymiarowej, odocelowej tablicy
	for (int i = 0; i < row.size(); i++){		//po kolei ka¿dy wyraz przepisujemy
		distances[distances.size() - 1].resize(rows + 1);		// nie wiadomo jaki jest rozmiar docelowy, wiêc trzeba za kazdym razem ustawiæ - kolumny
		distances[distances.size() - 1][rows] = row[i];	//	copy-paste
		rows++;		//	przygotowanie do zwiêkszenia rozmiaru
		if (row[i] == 0) {	// je¿eli 0 to przejœcie do nastepnego wiersza
			rows = 0;//wyzerowanie licznnika wierszy
			if ((i + 1<row.size())){		// je¿eli jest po co rozszerzaæ to rozrzesz
				distances.resize(distances.size() + 1);	// nie wiadomo jaki jest rozmiar docelowy, wiêc trzeba za kazdym razem ustawiæ - wiersze
			}
		}
	}
	row.~vector<int>(); // pomocnicza tablica ju¿ nie potrzebna, papa
	plik.close();

	for (int i = 0; i < distances.size(); i++){		// uzupe³neinie do prostok¹tnej tablicy
		distances[i].resize(distances.size());			// zmien rozmiar wiersza
		for (int j = 0; j < distances.size(); j++){
			distances[i][j] = distances[j][i];		// przepisanie
		}
	}
	//cout << "ok";
}

void readDataASym(string name,int number){
	row.clear();
	distances.clear();
	fstream plik; // deklaracja pliku
	plik.open(name, ios::in); // plik, póki co tylko symetryczne macierzowe

	while (!plik.eof()){		// trzeba while, dla fora trzeba by by³o zrobiæ sztuczny warunek stopu, nie fajnie
		getline(plik, temp, ' ');	// po kolei ka¿dy 'wyraz' do stringa, wyrazy z pliku oddzielone spacj¹, w pliku wyrzucony wczesniej opis
		row.push_back(atoi(temp.c_str())); // konwersja na int, wrzucenie do tablicy
	}

	distances.resize(number);
	int position=0;
	for (int i = 0; i < row.size(); i++){
		distances[position].push_back(row[i]);
		if (distances[position].size() == (number)){
			position++;
		}
	}
	row.~vector<int>(); // pomocnicza tablica ju¿ nie potrzebna, papa
	plik.close();

	cout << "ok";
}

void showData(){
	for (int i = 0; i < distances.size(); i++){						//wyswietlenie docelowej talbicy, ¿eby sprawdziæ czy so far so gut
		for (int j = 0; j < distances[i].size(); j++){
			cout << distances[i][j] << " ";
		}
		cout << endl;
	}
}

//¿eby ³atwiej podgl¹daæ co siê dzieje, do testów
//-------																						
template <class T1, class T2>																	
std::ostream& operator<<(std::ostream& strm, const std::vector<T1, T2>& vec)					
{																								
	strm << "[ ";																				
	for (int i = 0; i < vec.size(); i++) {												
		strm << vec[i] << "; ";																	
	}																							
	strm << "]";																				
	return strm;																				
}																								
																								
template <class T1, class T2>																	
std::ostream& operator<<(std::ostream& strm, const std::pair<T1, T2>& kvPair)					
{																								
	strm << "(" << kvPair.first << ", " << kvPair.second << ")";								
	return strm;																				
}																								
																								
template <class T1, class T2>																	
std::ostream& operator<<(std::ostream& strm, const std::map<T1, T2>& map) {						
	strm << "{\n";																				
	for (const auto &p : map) {																	
		strm << "  " << p.first << " => " << p.second << endl;
	}																							
	strm << "}\n";																				
	return strm;																				
}																								
//-------

vector<vector<int>> powerset(int n){								// double vector, na wszystkie kombinacje mo¿liwych dróg
	if (n == 0){
		return vector<vector<int>> { vector<int> {} };
	}
	else {
		auto tail = powerset(n - 1);

		vector<vector<int>> newPowerSet(tail);
		for (int i = 0; i < tail.size(); ++i) {
			auto set = vector<int>(tail[i]);
			cout << tail.size();
			set.push_back(n);
			newPowerSet.push_back(set);
		}
		return newPowerSet;
	}
}

// w³aœciwa metoda DYNAMIC PROGRAMMING
void find(){
	
	int n = distances.size(); // wielkoœc tablicy
	map<pair<vector<int>, int>, int> C;		// deklaracja mapy, w ktorej beda przechowywane kombinacje z odpowiedznia odlegloscia
	for (int k = 1; k < n; k++){ // wype³nienie mapy
		C[make_pair(vector<int> {k}, k)] = distances[k][0]; 
	}
	auto p = powerset(n - 1); // stworzenie kombinacji
	sort(p.begin(), p.end(), [](vector<int> x, vector<int> y) { return x.size() < y.size(); }); //posortowanie, od najmniejszej liczby elementów do najwiekszej
	for (int i = n; i < p.size(); i++) { // w³aœciwa fukcja ktora liczy odleglosci
		auto S = p[i]; 
		for (int k : S) {		
			vector<int> u; // 
			for (auto s : S) {
				if (s != k) u.push_back(s); 
			}
			int minimum = INT_MAX >> 1;
			for (int m : S){
				cout << ".";
				if (m == k) continue; 
				auto current = C[make_pair(u, m)] + distances[m][k]; // dynamiczne programowanie, liczenie drogi z 1 do n + z n do 1, dla wszystkich elementów S, czyl istworzonych wczesniej kombinacji

				minimum = minimum > current ? current : minimum;  // wybór najkrotszej jak do tej pory
			}
			C[make_pair(S, k)] = minimum;  // zapisanie do mapy
		}
	}
	vector<int> stuff;	// pomocnicza tablica, ¿eby wybraæ optimum
	for (int k = 1; k < n; k++){
		stuff.push_back(k);
	}
	
	for (int k = 1; k < n; k++){
		optimum = min(optimum, C[make_pair(stuff, k)] + distances[k][0]); // wybranie minimalnej drogi od miasta 0 przez wszystkie do miasta 0
	}
	
	cout << "opt " << optimum;
}



// ----------
// TABU SEARCH
vector<int> currentPath;
vector< vector<int> > tabuList;
vector<int> newBestSolution;
int bestCost, newbestCost;
vector<int> bestSolution;
int numberOfIterations = distances.size()*1000;

int getPathCost(vector<int> &a){ // sumuje koszty, zwraca ca³kowity kosz drogi path
	int tmp_cost=0;
	for (int i = 0; i < a.size()-1; i++){
		tmp_cost += distances[a[i]][a[i + 1]];
	}
	return tmp_cost;
}

void TabuList(int a){
	tabuList.clear();
	for(int i = 0; i<a-1; i++){
		tabuList.resize(i + 1);
		for (int j = 0; j<a-1; j++){
			tabuList[i].push_back(0);
		}
	}
}

void tabuMove(int city1, int city2) { //tabus the swap operation
	tabuList[city1][city2] += 5;
	tabuList[city2][city1] += 5;
}

void decrementTabu() {
	for (int i = 0; i < distances.size(); i++) {
		for (int j = 0; j < distances.size(); j++) {
			tabuList[i][j] -= tabuList[i][j] <= 0 ? 0 : 1;
		}
	}
}

void copy(vector<int> &a, vector<int> &b){
	b.clear();
	for (int i = 0; i < a.size(); i++){
		b.push_back(a[i]);
	}
}

void swap(int city1, int city2, vector<int> &solution) {
	int temp = solution[city1];
	solution[city1] = solution[city2];
	solution[city2] = temp;
}

void getBestNeighbour(){
	copy(currentPath, bestSolution);
	bestCost = getPathCost(bestSolution);
	int city1 = 0;
	int city2 = 0;
	bool firstNeighbor = true;
	for (int i = 1; i < bestSolution.size() - 1; i++) {
		for (int j = 2; j < bestSolution.size() - 1; j++) {
			if (i == j) {
				continue;
			}
			copy(bestSolution, newBestSolution);
			swap(i, j, newBestSolution);
			newbestCost = getPathCost(newBestSolution);

			if (((newbestCost < bestCost) || firstNeighbor) && tabuList[i][j] == 0) { //if better move found, store it
				firstNeighbor = false;
				city1 = i;
				city2 = j;
				bestSolution = newBestSolution;
				//copy(newBestSolution, bestSolution);
				bestCost = newbestCost;
			} 
		}
	}
	if (city1 != 0) {
		decrementTabu();
		tabuMove(city1, city2);
	}
}

void tabu(){
	currentPath.clear();
	for (int i = 0; i < distances.size(); i++){
		currentPath.push_back(i);
	}
	currentPath.push_back(0); // path teraz to tablica z 0,1,2....n,0 < defaultowa œciezka startowa
	
	TabuList(currentPath.size());

	copy(currentPath, bestSolution);
	optimum = getPathCost(bestSolution);


	for (int i = 0; i < distances.size() * 100; i++) { // perform iterations here

		getBestNeighbour();
		bestCost = getPathCost(bestSolution);
		random_shuffle(bestSolution.begin()+1, bestSolution.end()-1);
	
		if (bestCost < optimum) {
			copy(bestSolution, currentPath);
			optimum = bestCost;
		}
	}
	cout << endl << "najlepsze rozwiazanie: "<< optimum;
}






int main()
{	
	
	
	srand(unsigned(time(0)));
	
	fstream out; // deklaracja pliku
	out.open("output.txt", ios::out); // plik
	Stopwatch clock;
	//showData();
	

		
		int x,q;
		



		cout  << "Implementacja metody Tabu Search dla problemu TSP";
		cout << endl << "Wybierz:" << endl;
		cout  << endl << "STSP: [1] - 8 miast, [2] gr17.txt, [3] gr21.txt, [4] gr24.txt, [5] gr48.txt" << endl;
		cout  << endl << "ATSP: [6] - dla danych z wikipedii, [7] - 10 miast, [8] br17.txt  [9] ft53.txt "<< endl;
		cout << "Podaj numer ....";
		cin >> x;



		switch (x)
		{
		case 1:{
		int q = 0;
		system("cls");
		cout << "Ile razy chcesz liczyc?";
		cin >> q;
		for (int i = 0; i < q; i++){
		distances = { { 0, 1, 5, 6, 2, 6, 3, 4 },
		{ 1, 0, 7, 3, 8, 2, 1, 3 },
		{ 5, 7, 0, 2, 1, 11, 3, 4 },
		{ 6, 3, 2, 0, 7, 9, 1, 9 },
		{ 2, 8, 1, 7, 0, 3, 5, 7 },
		{ 6, 2, 11, 9, 3, 0, 6, 1 },
		{ 3, 1, 3, 1, 5, 6, 0, 3 },
		{ 4, 3, 4, 9, 7, 1, 3, 0 },
		};
		auto start = chrono::steady_clock::now();
		find();
		auto end = chrono::steady_clock::now();
		//auto time_span = static_cast<chrono::duration<double>>(end - start);
		auto diff = end - start;
		out << optimum << ", " << chrono::duration_cast<ns>(diff).count()  << "ns" << endl;
		out << endl;
		}
		break;
		}


		case 2:
		{

		system("cls");
		cout << "Ile razy chcesz liczyc?";
		cin >> q;
		for (int i = 0; i < q; i++){
		readDataSym("gr17.txt");

		clock.start();
		tabu();
		clock.stop();
		out << optimum << ", " << clock.read() << "s" << endl;
		out << endl;
		}
		break;
		}

		case 3:
		{

		system("cls");
		cout << "Ile razy chcesz liczyc?";
		cin >> q;
		for (int i = 0; i < q; i++){
		readDataSym("gr21.txt");

		clock.start();
		tabu();
		clock.stop();
		out << optimum << ", " << clock.read() << "s" << endl;
		out << endl;
		}
		break;
		}
		case 4:
		{

		system("cls");
		cout << "Ile razy chcesz liczyc?";
		cin >> q;
		for (int i = 0; i < q; i++){
		readDataSym("gr24.txt");

		clock.start();
		tabu();
		clock.stop();
		out << optimum << ", " << clock.read() << "s" << endl;
		out << endl;
		}
		break;
		}
		case 5:
		{

				  system("cls");
				  cout << "Ile razy chcesz liczyc?";
				  cin >> q;
				  for (int i = 0; i < q; i++){
					  readDataSym("gr48.txt");

					  clock.start();
					  tabu();
					  clock.stop();
					  out << optimum << ", " << clock.read() << "s" << endl;
					  out << endl;
				  }
				  break;
		}
		case 6:
		{

		system("cls");
		cout << "Ile razy chcesz liczyc?";
		cin >> q;
		for (int i = 0; i < q; i++){
		{
		distances = { { 0, 2, 9, 10 },
		{ 1, 0, 6, 4 },
		{ 15, 7, 0, 8 },
		{ 6, 3, 12, 0 },
		};

		clock.start();
		tabu();
		clock.stop();
		out << optimum << ", " << clock.read() << "s" << endl;
		out << endl;
		}
			break;
		}
		}
		case 7:
		{
		system("cls");
		cout << "Ile razy chcesz liczyc?";
		cin >> q;
		for (int i = 0; i < q; i++){
		distances = { { 0, 2, 9, 10, 5, 2, 4, 7, 9, 10 },
		{ 1, 0, 6, 4, 2, 7, 6, 3, 5, 2 },
		{ 15, 7, 0, 8, 2, 1, 5, 7, 8, 2, 7 },
		{ 6, 3, 12, 0, 2, 4, 6, 3, 7, 8 },
		{ 5, 2, 7, 1, 5, 6, 3, 9, 2, 4, },
		{ 1, 0, 4, 0, 2, 9, 0, 2, 5, 7 },
		{ 3, 1, 7, 9, 5, 2, 8, 7, 1, 5 },
		{ 2, 1, 8, 6, 7, 3, 5, 4, 2, 9 },
		{ 5, 2, 8, 9, 3, 7, 4, 2, 3, 8 },
		{ 2, 7, 5, 9, 4, 2, 3, 6, 47, 0 }
		};

		clock.start();
		tabu();
		clock.stop();
		out << optimum << ", " << clock.read() << "s" << endl;
		out << endl;
		}
		break;
		}
		case 8:
		{
		system("cls");
		cout << "Ile razy chcesz liczyc?";
		cin >> q;
		for (int i = 0; i < q; i++){
		readDataASym("br17.txt", 17);

		clock.start();
		tabu();
		clock.stop();
		out << optimum << ", " << clock.read() << "s" << endl;
		out << endl;
		}
		break;
		}
		case 9:
		{

				  system("cls");
				  cout << "Ile razy chcesz liczyc?";
				  cin >> q;
				  for (int i = 0; i < q; i++){
					  readDataSym("ft53.txt");

					  clock.start();
					  tabu();
					  clock.stop();
					  out << optimum << ", " << clock.read() << "s" << endl;
					  out << endl;
				  }
				  break;
		}
		default:
		cout << "Cos poszlo nie tak...";
		break;
		}

		

	

	cout << " end " << endl;
	cin.get();
	system("PAUSE");		// tak ¿eby da³o sie zobaczyæ
	return 0;
}