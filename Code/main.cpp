#include<iostream>
#include<fstream>
#include<stdlib.h>
#include <string>
#include <sstream>

#include "Generic_connection_parser.hpp"
#include "Chemin.hpp"
using namespace travel;

//======= EVALUATE PAR Grade =========

//#include"Grade.hpp"
//using travel::evaluate_small;
//using travel::evaluate;
//
//void Test_Grade(Chemin& _myclass) {
//	evaluate_small.stations(_myclass);
//	evaluate_small.connections(_myclass);
//	evaluate.dijkstra(_myclass, false);
//}


// ==== Declaration les fonctions pour tester chaque partie =====
void Test_stationETconnection(Chemin& _myclass);
void Test_Chemin_ID(Chemin& _myclass);
void Test_Chemin_NAME(Chemin& _myclass);


int main() {

	Chemin Map;
	//Test_stationETconnection(Map);
	Test_Chemin_ID(Map);
	//Test_Chemin_NAME(Map);

	//Test_Grade(Map);

	return 0;
}

// ======= Les fonctions de tester chaque partie=========

//-------------Test les Stations et les Connections-----------
void Test_stationETconnection(Chemin& _myclass) {
	std::unordered_map<uint64_t, Station> station_map;
    std::unordered_map<uint64_t, std::unordered_map<uint64_t, uint64_t>> connection_map;
	// lire les donées de ratp; les deux fonctions doivent être publiques pour pouvoir être utilisées en dehors de la classe
	_myclass.read_stations("data/stations.csv");
	_myclass.read_connections("data/connections.csv");
	station_map = _myclass.get_stations_hashmap();
	connection_map = _myclass.get_connections_hashmap();

	//afficher quelques exemples
	std::cout << "Afficher toutes les stations avec les données simples" << std::endl;
	for (auto i = station_map.begin(); i != station_map.end(); ++i) {
		std::cout <<"id: "<<i->first<<"  " << i->second << std::endl;
	}
	std::cout << std::endl;
	std::cout << "Afficher la connection de la station avec id 1" << std::endl;
	for (auto i = connection_map.at(1).begin(); i != connection_map.at(1).end(); ++i) {
		std::cout <<"id: "<< i->first <<", cout: "<< i->second << std::endl;
	}
}


//-------------Plus court chemin par Station ID -------------------
void Test_Chemin_ID(Chemin& _myclass) {
	std::unordered_map<uint64_t, Station> station_map;
	std::unordered_map<uint64_t, std::unordered_map<uint64_t, uint64_t>> connection_map;
	_myclass.read_stations("data/s.csv");    // utiliser la fonction pour lire les donnees
	_myclass.read_connections("data/c.csv");
	station_map = _myclass.get_stations_hashmap();     // enregistrer les donnees dans les deux maps
	connection_map = _myclass.get_connections_hashmap();
	uint64_t start;
	uint64_t end;
	std::cout << "Entrer start station id: ";
	std::cin >> start;
	// En cas de mauvaise entree, réessayer
	while (station_map.count(start) == 0) { 
		std::cin.clear();
		while (std::cin.get() != '\n') continue;
		std::cout << "L'identifiant de station n'exsit pas, re-saisir s'il vous plait : ";
		std::cin >> start;
	}
	std::cout << "Entrer end station id: ";
	std::cin >> end;
	// En cas de mauvaise entree, réessayer
	while (station_map.count(end) == 0) {
		std::cin.clear();
		while (std::cin.get() != '\n') continue;
		std::cout << "L'identifiant de station n'exsit pas, re-saisir s'il vous plait : ";
		std::cin >> end;
	}
	// appel la fonction pour calculer le chemin et l'afficher
	_myclass.compute_and_display_travel(start, end);

}


//---------Ameliorer: le plus court chemin par Station NAME--------- (Pas sensible à la casse)
void Test_Chemin_NAME(Chemin& _myclass) {
	std::string start_name = "0";
	std::string end_name = "0";

	std::unordered_map<uint64_t, Station> station_map;
	std::unordered_map<uint64_t, std::unordered_map<uint64_t, uint64_t>> connection_map;
	_myclass.read_stations("data/s.csv");
	_myclass.read_connections("data/c.csv");
	station_map = _myclass.get_stations_hashmap();
	connection_map = _myclass.get_connections_hashmap();
	std::cout << "Entrer start station name: ";
	std::getline(std::cin, start_name);   // entrer inclus l'espace 
	std::cout << "Entrer end station name: ";
	std::getline(std::cin, end_name);
	// 2 valeur booleen pour vérifier les 2 noms de station existent
	bool error1 = true; 
	bool error2 = true;
	while (error1 or error2) {  // s'il y a une erreur, reessayer
		try {
			_myclass.compute_and_display_travel(start_name, end_name);  // si le nom existe pas, cette fonction va throw l'erreur
			break; // si les 2 noms sont corrects, sortir de la boucle
		}
		catch (int16_t const& err) {  // catch les erreurs throw dans la fonction precedente
			if (err == 1) {   // le nom de start station est incorrect
				std::cout << "\nLe nom de start station n'exsit pas, re-saisir s'il vous plait : ";
				std::getline(std::cin, start_name);
				error1 = true;
			}
			else { error1 == false; }  // le nom de start station est correct
			if (err== 2) { // le nom de end station est incorrect
				std::cout << "\nLe nom de end station n'exsit pas, re-saisir s'il vous plait : ";
				std::getline(std::cin, end_name);
				error2 = true;
			}
			else { error2 = false; }  // le nom de end station est correct
		}
	}
}



