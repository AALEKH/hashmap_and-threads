#include <fstream>
#include <iostream>
#include <vector>
#include <queue> 
#include <map>
#include <string>
#include <thread>
#include <array>
#include <mutex>
#include <algorithm>

#include <boost/crc.hpp>

// using namespace std;

struct jaaX_Open_Addressing_
{
    std::string key;
    std::string value;
    int number_of_collision_elements;
    struct jaaX_Open_Addressing_ *next;
    struct jaaX_Open_Addressing_ *lastIndex;
};

int GetCrc32(const std::string& my_string) {
    boost::crc_32_type result;
    result.process_bytes(my_string.data(), my_string.length());
    return result.checksum();
}

class jaax_MultiMap {
	
	private:
		std::map<int, jaaX_Open_Addressing_> jaaX_Map;
		std::mutex mutex_x;

	public:

		jaax_MultiMap(): jaaX_Map() {

		}

		~jaax_MultiMap() {

		}

		void insert_Element(int key, struct jaaX_Open_Addressing_ value) {
			std::lock_guard<std::mutex> guard(mutex_x);
			jaaX_Map[key] = value;
		}

		void insert_and_check(std::string key, std::string value) {

			std::lock_guard<std::mutex> guard(mutex_x);
			int i_key = GetCrc32(key);
			struct jaaX_Open_Addressing_ element = {key, value, 0, NULL, NULL};
			std::map<int, jaaX_Open_Addressing_>::iterator it = jaaX_Map.find(i_key);
			if(it != jaaX_Map.end())
			{
			   // Add colision element found code here

			} else {
				// Insert's element if collision does not occur
				insert_Element(i_key, element);	
			}
			
		}

		std::vector<std::string> get_Element(std::string key) {

			std::lock_guard<std::mutex> guard(mutex_x);
			int i_key = GetCrc32(key);
			std::map<int, jaaX_Open_Addressing_>::iterator it = jaaX_Map.find(i_key);
			if(it != jaaX_Map.end())
			{
			   // Currently just returns non colision element found add colision case code here.
				return {jaaX_Map[i_key].key, jaaX_Map[i_key].value};
			} else {
				// Element's not found
				return {"", ""};
			}
		}

		void erase_element(std::string key) {

			std::lock_guard<std::mutex> guard(mutex_x);
			int i_key = GetCrc32(key);
			jaaX_Map.erase(i_key);
		}

		void put_element(std::string key, std::string value) {

			std::lock_guard<std::mutex> guard(mutex_x);
			int i_key = GetCrc32(key);
			jaaX_Map[i_key].value = value;
		
		}
};


int main() {
	jaax_MultiMap *jm = new jaax_MultiMap();
	jm->insert_and_check("Aalekh", "Nigam");
	std::cout <<  jm->get_Element("Aalekh")[1] << std::endl;
	jm->put_element("Aalekh", "Niigam");
	jm->erase_element("Aalekh");
	return 0;
}