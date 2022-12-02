
#include "Generic_station_parser.hpp"
#include"Generic_connection_parser.hpp"
#include"Generic_mapper.hpp"

namespace travel {
	class Chemin :public Generic_mapper {
	public:
		void afficher() { std::cout << "my station" << std::endl; }

		virtual std::vector<std::pair<uint64_t, uint64_t> > compute_travel(uint64_t _start, uint64_t _end) override; // mapper
	    virtual std::vector<std::pair<uint64_t, uint64_t> > compute_and_display_travel(uint64_t _start, uint64_t _end) override; //mapper

		//surcharge des fonctions qui estiment le trajet minimal pour relier les stations via leurs noms, et non plus leurs identifiants
		std::vector<std::pair<uint64_t, uint64_t>> compute_and_display_travel(std::string start_name, std::string end_name);

	//protected:
		virtual void read_stations(const std::string& _filename) override;  // station parser
		virtual void read_connections(const std::string& _filename) override;  // connection parser

		
		
		 
		
	};
}

std::ostream& operator<<(std::ostream& o, const std::vector<std::pair<uint64_t, uint64_t>>& v);