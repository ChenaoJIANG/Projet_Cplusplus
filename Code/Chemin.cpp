#include<iostream>
#include<fstream>
#include<stdlib.h>
#include <string>
#include <sstream>
#include<algorithm>
#include<math.h>
#include "Chemin.hpp"
using namespace travel;

// l'algorithme Dijktra
std::vector<std::pair<uint64_t, uint64_t>> travel::Chemin::compute_travel(uint64_t _start, uint64_t _end)
{

	std::unordered_map<uint64_t, uint64_t> station_yes;  // l'ensemble de stations calculees le plus court cout  [ {station1: cout1}, {station2: cout2}... ]
	std::unordered_map<uint64_t, uint64_t> station_no;  // l'ensemble de stations no calculees

	std::unordered_map<uint64_t, std::vector<std::pair<uint64_t, uint64_t>>> chemins;  // map key: chaque station; value: chemin(vector) // chaque station associe un chemin plus court
	std::vector<std::pair<uint64_t, uint64_t>> chemin;            //vector inclus plusieur pairs; represente le chemin plus court dans la map chemins pour chaque station 
	//std::pair<uint64_t, uint64_t> station_at(_start, 0);
	chemin.push_back({_start, 0});
	
	// Initialisation chemin a (start, 0) pour chaque station
	for (auto iter = this->stations_hashmap.begin(); iter != this->stations_hashmap.end(); ++iter) {
		chemins.insert({ iter->first,chemin });
	}

	// Initialisation les 2 ensembles station_no et station_yes
	station_yes.insert({ _start,0 });
	station_no = this->connections_hashmap[_start];    // les stations connecte avec la station start
	for (auto iter = this->stations_hashmap.begin(); iter != this->stations_hashmap.end(); ++iter) {
		uint64_t cout = UINT64_MAX;
		uint64_t s_id = iter->first;
		if (s_id != _start) {
			station_no.insert({ s_id,cout });     // les autres station initialise le cout a infini
		} 
	}

	// repeter (nombre station-1) fois, tirer la station de plus petit cout du map station_no, la mettre dans le map station_yes 
	for (int i = 0; i < (this->stations_hashmap.size() - 1); i++) {                        
		std::pair<uint64_t, uint64_t> min;
		min.second = UINT64_MAX;
		// trouver la station avec le cout minimal (pair: min)
		for (auto iter = station_no.begin(); iter != station_no.end(); ++iter) {
			if (iter->second < min.second) {
				min.first = iter->first;
				min.second = iter->second;
			}
		}
		// mettre min dans l'ensemble station_yes
		station_yes.insert(min);
		// mise a jour l'ensemble station_no
		station_no.erase(min.first);  // supprimer min

		// mise a jour le chemin de cette station min dans la map chemins
		chemin = chemins.at(min.first);    // trouver le chemin pour la station min
		//station_at = min;
		chemin.push_back(min);        // ajouter la station min dans le chemin 
		// mise a jour le chemin de cette station dans le map chemins
		chemins.erase(min.first);
		chemins.insert({ min.first,chemin });
		//chemins.insert_or_assign(min.first, chemin);


		// mise a jour les stations connectes avec min
		std::unordered_map<uint64_t, uint64_t> station_connect;
		station_connect = this->connections_hashmap[min.first];  // les stations connectees avec min
		for (auto iter1 = station_no.begin(); iter1 != station_no.end(); ++iter1) {
			for (auto iter = station_connect.begin(); iter != station_connect.end(); ++iter) {
				if (iter->first == iter1->first) {                                                  // si la station dans le map station_connect
					if (iter1->second > (iter->second + min.second)) {        // mise a jour le cout si le cout precedent > le cout du min+ le cout de station connecte
						iter1->second = iter->second + min.second;
						// et mise a jour le chemin des stations connectees aussi
						chemin = chemins.at(min.first);                          
						chemins.erase(iter->first);
						chemins.insert({ iter->first, chemin });
						//chemins.insert_or_assign(iter->first, chemin);
					}
				}
			}
		}
	}
	// dans la map chemins, trouver le chemin plus court entre la station start et end et le return
	chemin = chemins.at(_end);
	return chemin;
}


// afficher le chemin clairement
std::vector<std::pair<uint64_t, uint64_t>> travel::Chemin::compute_and_display_travel(uint64_t _start, uint64_t _end)
{
	std::cout << std::endl;
	std::cout << "Best way from " << this->stations_hashmap.at(_start) << " to " << this->stations_hashmap.at(_end) << " is: " << std::endl << std::endl;
	std::vector<std::pair<uint64_t, uint64_t>> chemin;   
	chemin = compute_travel(_start, _end);   // obtenir le chemin plus court par la fonction compute_travel 

	// la meme station de differente ligne, il faut indiquer aller a pied; les stations dans la meme ligne sont affiche ensemble
	//utiliser deux iterateurs et les initialiser
	Station s1;
	Station s2;
	auto iter1 = chemin.begin();          
	auto iter2 = chemin.begin()+1;

	while (iter2!= chemin.end()) { 
		s1 = this->stations_hashmap.at(iter1->first);   // get les 2 stations pointés par les 2 iterateurs
		s2 = this->stations_hashmap.at(iter2->first);
		if (s1.name == s2.name) {
			if (s1.line_id != s2.line_id) {  // same name, different line
				std::cout << "- Walk to " << s2 << " ( " << (iter2->second - iter1->second) << " secs )" << std::endl << std::endl;; 
			}
			// les 2 iterator +1, meme que le cas: same name, same line
			iter1++;
			iter2++;
		}

		else {
			if (s1.line_id == s2.line_id) {     // different name, same line
				//if (iter2 + 1 == chemin.end() - 1) {
				if (iter2  == chemin.end() - 1) {
					std::cout << "- Take line " << s1.line_id << s1.line_name << std::endl;
					if ((iter2->second - (iter2 - 1)->second) == 0) {  // le cout de derniere etape est 0 
						std::cout << "  From " << s1.name << " to " << this->stations_hashmap.at(iter2->first).name << " ( " << (iter2->second - iter1->second) << " secs )  " << (iter2 - iter1 -1 ) << " stations" << std::endl << std::endl;
					}
					else {
						std::cout << "  From " << s1.name << " to " << this->stations_hashmap.at(iter2->first).name << " ( " << (iter2->second - iter1->second) << " secs )  " << (iter2 - iter1) << " stations" << std::endl << std::endl;
					}
				}
				iter2++;
			}
			else {   // different name, different line
				std::cout << "- Take line " << s1.line_id << s1.line_name << std::endl;
				std::cout << "  From " << s1.name << " to " << this->stations_hashmap.at((iter2 - 1)->first).name << " ( " << ((iter2 - 1)->second - iter1->second) << " secs )  " << (iter2 - iter1-1) << " stations"<<std::endl << std::endl;
				iter1 = iter2 - 1;
			}
		}

	}

	std::cout << "After " << (chemin.end() - 1)->second << " secs ( about " << round(((chemin.end() - 1)->second) / 60)+1 << " min ), you have reached your destination!" << std::endl;

	return std::vector<std::pair<uint64_t, uint64_t>>();
}


// associer le fichier et mettre les donnees dans this->station_hashmap
void travel::Chemin::read_stations(const std::string& _filename) 
{
	std::string line;          // pour stocker chaque ligne de donnees 
	std::ifstream file;      // definit un flot d'entree pour input file stream
	file.open(_filename);    // utilise la fonction open pour associer le stream entree avec le fichier physique _filename(nom de fichier)

	// tester le fichier exist ou pas
	if (!file.is_open()) {
		std::cout << "File " << _filename << " not found." << std::endl;
		exit(-1);
	}
	// lecture des donnees ligne par ligne
	std::getline(file, line);   // no read the first line
	while (std::getline(file, line)) {
		Station s;                    // structure Station
		std::uint32_t station_id;
		std::istringstream readstr(line);  // Lire les caracteres readstr de l'objet chaine line

		std::string info;  // stocker l'information temporaire

		std::getline(readstr, info, ',');    // chaque line, separer par le virgule, 
		s.name = info;                            // la premiere information est le nom de station
		std::getline(readstr, info, ',');
		station_id = std::atoi(info.c_str());     // convertir une chaine en une entier  (2nd info est l'identifiant de station)
		std::getline(readstr, info, ',');
		s.line_id = info;                                    // 3eme information est l'identifiant de ligne de cette station
		std::getline(readstr, info, ',');
		s.address = info;                                  // 4eme information est l'address de station
		std::getline(readstr, info, ',');
		s.line_name = info;                               // 5eme information est le nom de ligne de cette station

		this->stations_hashmap.insert({ station_id,s });       // ajouter dans la map de station: unordered_map<uint64_t, Station>  ==>  <uint64_t station_id, Station s>
	}
}


// associer le fichier et mettre les donnees dans this->connections_hashmap (la meme que read_stations)
void travel::Chemin::read_connections(const std::string& _filename)
{
	std::string line;
	std::ifstream file;
	file.open(_filename);
	if (!file.is_open()) {
		std::cout << "File " << _filename << " not found." << std::endl;
		exit(-1);
	}
	std::getline(file, line); // not read the first line

	std::unordered_map<uint64_t, uint64_t> map_to_id_time = {};  // une map pour chaque start station, key: id de station connecte; value: le cout

	while (std::getline(file, line)) {
		
		std::uint64_t from_stop_id =0;         // id de start-station
		std::uint64_t to_stop_id=0;             // id de end-station
		std::uint64_t time=0;                       // le cout entre les 2 stations


		std::istringstream readstr(line);  

		std::string info;

		std::getline(readstr, info, ',');   
		from_stop_id = std::atoi(info.c_str());   // convertir une chaine en une entier (1ere information est l'identifiant de start station)

		std::getline(readstr, info, ',');
		to_stop_id = std::atoi(info.c_str());      // convertir une chaine en une entier  (2nd information est l'identifiant de end station)

		std::getline(readstr, info, ',');
		time = std::atoi(info.c_str());                    // convertir une chaine en une entier (3ere information est le cout entre les 2 stations)

		
		if (this->connections_hashmap.count(from_stop_id) == 0) {    // cette start station n'existe pas encore
			map_to_id_time = {};
			map_to_id_time.insert({ to_stop_id, time });  // ajouter la value a cette key
		}
		else {   // si cette start station existe deja
			map_to_id_time = this->connections_hashmap.find(from_stop_id)->second; // trouver le map correspondane a la start station
			map_to_id_time.insert({ to_stop_id, time });      // ajouter ce couple {station connecte, cout} dans le map  
		}
		
		//ajouter dans la map de connections: 
		//unordered_map<uint64_t, std::unordered_map<uint64_t,uint64_t> >  ==>  <uint64_t from_stop_id,std::unordered_map map_to_id_time >
		this->connections_hashmap.erase(from_stop_id);            //il faut supprimer le couple de start station et le mise a jour 
		this->connections_hashmap.insert({ from_stop_id, map_to_id_time });     //car la fonction insert ne peut pas remplacer le couple si cette key existe deja 
	    //this->connections_hashmap.insert_or_assign(from_stop_id, map_to_id_time);  // insert_or_assign peut remplacer le couple si cette key existe, mais elle est pour std=c++17
	}

}




// Ameliorer: surcharger la fonction compute_and_display_travel
std::vector<std::pair<uint64_t, uint64_t>> travel::Chemin::compute_and_display_travel(std::string start_name, std::string end_name)
{
	std::uint64_t _start = 0;
	std::uint64_t _end = 0;
	// convertir les noms de station en minuscule
	transform(start_name.begin(), start_name.end(), start_name.begin(), ::tolower);
	transform(end_name.begin(), end_name.end(), end_name.begin(), ::tolower);
	
	for (auto i = stations_hashmap.begin(); i != stations_hashmap.end(); ++i) {
		std::string name = i->second.name;
		transform(i->second.name.begin(), i->second.name.end(),name.begin(), ::tolower);   // convertir le nom en minuscule et l'enregistrer dans string name
		//trouver le meme nom
		if (name == start_name) {
			_start = i->first;
		}
		if (name == end_name) {
			_end = i->first;
		}
	}
	// s'il ne trouve pas, throw error
	if (_start == 0) {
		throw std::int16_t(1);
	}
	if (_end == 0) {
		throw std::int16_t(2);
	}


	std::cout << "\nBest way from " << this->stations_hashmap.at(_start) << " to " << this->stations_hashmap.at(_end) << " is: " << std::endl << std::endl;
	std::vector<std::pair<uint64_t, uint64_t>> chemin;
	chemin = compute_travel(_start, _end);   // obtenir le chemin plus court par la fonction compute_travel 

	// la meme station de differente ligne, il faut indiquer aller a pied; les stations dans la meme ligne sont affiche ensemble
	Station s1;
	Station s2;
	auto iter1 = chemin.begin();
	auto iter2 = chemin.begin() + 1;

	while (iter2 != chemin.end()) {
		s1 = this->stations_hashmap.at(iter1->first);
		s2 = this->stations_hashmap.at(iter2->first);
		if (s1.name == s2.name) {
			if (s1.line_id != s2.line_id) {  // same name, different line
				std::cout << "- Walk to " << s2 << " ( " << (iter2->second - iter1->second) << " secs )" << std::endl << std::endl;;  // same name, different line
			}
			// les 2 iterator +1, meme que le cas: same name, same line
			iter1++;
			iter2++;
		}

		else {
			if (s1.line_id == s2.line_id) {     // different name, same line
				//if (iter2 + 1 == chemin.end() - 1) {
				if (iter2 == chemin.end() - 1) {
					std::cout << "- Take line " << s1.line_id << s1.line_name << std::endl;
					if ((iter2->second - (iter2 - 1)->second) == 0) {  // le cout de derniere etape est 0 
						std::cout << "  From " << s1.name << " to " << this->stations_hashmap.at(iter2->first).name << " ( " << (iter2->second - iter1->second) << " secs )  " << (iter2 - iter1 - 1) << " stations" << std::endl << std::endl;
					}
					else {
						std::cout << "  From " << s1.name << " to " << this->stations_hashmap.at(iter2->first).name << " ( " << (iter2->second - iter1->second) << " secs )  " << (iter2 - iter1) << " stations" << std::endl << std::endl;
					}
				}
				iter2++;
			}
			else {   // different name, different line
				std::cout << "- Take line " << s1.line_id << s1.line_name << std::endl;
				std::cout << "  From " << s1.name << " to " << this->stations_hashmap.at((iter2 - 1)->first).name << " ( " << ((iter2 - 1)->second - iter1->second) << " secs )  " << (iter2 - iter1 - 1) << " stations" << std::endl << std::endl;
				iter1 = iter2 - 1;
			}
		}

	}

	std::cout << "After " << (chemin.end() - 1)->second << " secs ( about " << round(((chemin.end() - 1)->second) / 60)+1 << " min ), you have reached your destination!" << std::endl;

	return std::vector<std::pair<uint64_t, uint64_t>>();
}