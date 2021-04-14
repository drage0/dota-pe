#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <map>
#include <sstream>
#include <iomanip>
#include <string.h>
#include "dota/7_29/npc_heroes.h"

#define MAX_HERO_COUNT 256

struct hero
{
	std::string name;
	std::string nameid;
	std::map<std::string, std::string> parameters;
};

static void
trim(std::string &s, const std::string &characters)
{
	s.erase(0, s.find_first_not_of(characters));
}

static void
die(const char *message)
{
	std::cerr << message << std::endl;
	std::exit(1);
}

int
main(int argc, char **argv)
{
	std::string line(256, '\0');
	int hero_index         = -1;
	int depth              = 0;
	hero *heroes           = new hero[MAX_HERO_COUNT];
	hero *heroes_default   = new hero[MAX_HERO_COUNT];
	const size_t linecount = sizeof(npc_heroes7_29)/sizeof(npc_heroes7_29[0]);
	
	for (size_t i = 0; i < linecount; i++)
	{
		line = npc_heroes7_29[i];
		
		trim(line, "\t ");
		if (line.rfind("\"npc_dota_hero_") == 0)
		{
			hero_index++;
			heroes[hero_index].nameid = heroes_default[hero_index].nameid = line.substr(1, line.size()-2);
		}
		else if (line.rfind("//") == 0)
		{
		}
		else if (line.rfind("{") == 0)
		{
			depth++;
		}
		else if (line.rfind("}") == 0)
		{
			depth--;
		}
		else if (hero_index > 0 && depth == 2)
		{
			std::stringstream ss(line);
			std::string key, value;
			
			/*
			 * Read key-value pair
			 * check for empty to be sure it's key-value line.
			 */
			ss >> key;
			std::getline(ss, value); /* everything left after reading the key... */
			trim(key, " \t");
			trim(value, " \t");
			if (!key.empty() && !value.empty())
			{
				key.erase(0, 1);
				key.erase(key.size() - 1);
				value.erase(0, 1);
				value.erase(value.size() - 1);
				heroes[hero_index].parameters[key] = heroes_default[hero_index].parameters[key] = value;
			}
		}
	}
	
	/* Parse arguments. */
	hero_index = -1;
	std::ofstream ofile;
	
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-h") == 0)
		{
			if (argc > i+1)
			{
				const int hero_id = atoi(argv[i+1]);
				if (hero_id <= 0)
				{
					die("invalid hero id.");
				}
				
				for (size_t i = 0; i < MAX_HERO_COUNT; i++)
				{
					try
					{
						if (heroes[i].parameters.at("HeroID") == std::to_string(hero_id))
						{
							hero_index = i;
							break;
						}
					}
					catch (std::out_of_range e)
					{
					}
				}
			}
			else
			{
				die("-h parameter is malformed.");
			}
		}
		else if (strcmp(argv[i], "-p") == 0)
		{
			if (argc > i+1)
			{
				const std::string a(argv[i+1]);
				const size_t d          = a.find(":");
				const std::string key   = a.substr(0,   d);
				const std::string value = a.substr(d+1, a.size());
				
				heroes[hero_index].parameters[key] = value;
				std::cout << std::setw(32) <<  std::left << heroes[hero_index].nameid << std::setw(28) << key << "*: " << heroes[hero_index].parameters[key] << std::endl;
			}
			else if (hero_index == -1)
			{
				die("-p used before -h.");
			}
			else
			{
				die("-p parameter is malformed.");
			}
		}
		else if (strcmp(argv[i], "-l") == 0)
		{
			if (hero_index != -1)
			{
				std::cout << heroes[hero_index].nameid << ':' << std::endl;
				for (auto it = heroes[hero_index].parameters.begin(); it != heroes[hero_index].parameters.end(); it++)
				{
					std::cout << std::setw(28) << it->first << " : " << it->second << std::endl;
				}
			}
			else
			{
				die("-l used before -h.");
			}
		}
		else if (strcmp(argv[i], "-o") == 0)
		{
			if (argc > i+1)
			{
				ofile.open(argv[i+1], std::ios::out | std::ios::trunc);
				if (!ofile.is_open())
				{
					die("cannot open file for writing.");
				}
			}
			else
			{
				die("-o parameter is malformed.");
			}
		}
	}
	if (ofile.is_open())
	{
		bool write_depth2 = false;
		
		hero_index = -1;
		
		for (size_t i = 0; i < linecount; i++)
		{
			line = npc_heroes7_29[i];
			std::string	linetrim(line);
			
			trim(linetrim, "\t ");
			if (linetrim.rfind("\"npc_dota_hero_") == 0)
			{
				std::string nameid = linetrim.substr(1, linetrim.size()-2);
				
				for (int i = 0; i < MAX_HERO_COUNT; i++)
				{
					if (heroes[i].nameid == nameid)
					{
						hero_index   = i;
						write_depth2 = true;
					}
				}
			}
			else if (linetrim.rfind("//") == 0)
			{
			}
			else if (linetrim.rfind("{") == 0)
			{
				depth++;
			}
			else if (linetrim.rfind("}") == 0)
			{
				depth--;
			}
			else if (hero_index > 0 && depth == 2)
			{
				std::stringstream ss(line);
				std::string key, value;
				
				ss >> key;
				std::getline(ss, value);
				trim(key, " \t");
				trim(value, " \t");
				if (!key.empty() && value.empty())
				{
					line = "\t\t"+key+"\n";
				}
				else if (write_depth2)
				{
					line = "";
					write_depth2 = false;
					for (auto it = heroes[hero_index].parameters.begin(); it != heroes[hero_index].parameters.end(); it++)
					{
						line += "\t\t\"" + it->first + "\" \"" + it->second + "\"\n";
					}
				}
				else
				{
					line = "";
				}
			}
			
			if (!line.empty())
			{
				ofile << line << std::endl; /* TODO only use cr+lf */
			}
		}
		ofile.close();
	}
	
	return 0;
}
