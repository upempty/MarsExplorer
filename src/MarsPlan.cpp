#include <iostream>

#include "MarsPlan.h"

MarsPlan::MarsPlan()
{
    _paths.clear();
    _commands.clear(); 
}

void MarsPlan::dispatch_command(const std::string &instruction_str)
{
    _parser.load_str(instruction_str);
    _parser.get_commands(_commands);
    _parser.get_edge(_edge);
    
    _grid = new StateGrid(_edge.x, _edge.y);
    for (const auto &com : _commands)
    {
       Explorer explorer(com);
       _explorers.push_back(explorer);
    }
}

#ifndef __UT__
void MarsPlan::dispatch_command_from_file(const std::string &filename) 
{
    _parser.load(filename);
    //std::string contents = std::string("5 5\n1 2 N\nLMLMLMLMM");
    //_parser.load_str(contents);
    _parser.get_commands(_commands);
    _parser.get_edge(_edge);
    
    _grid = new StateGrid(_edge.x, _edge.y);
    for (const auto &com : _commands)
    {
       Explorer *explorer =  new Explorer(com);
       _explorers.push_back(*explorer);
    }
}
#endif

void MarsPlan::exec()
{
    int i = 0;
    for (auto &mar : _explorers)
    {
        mar.visit(*_grid);
        mar.walk_path(_paths[i++]);
    }
}

void MarsPlan::print_paths()
{
   for (size_t i = 0; i < _paths.size(); i++)
   {
       for (auto &p : _paths[i])
       {
           print_position(p);
       }
       std::cout << std::endl;
   }  

}
//options: commands, edge as the params
void MarsPlan::run(const std::string &instruction_str)
{
    dispatch_command(instruction_str);
    exec();
}

void MarsPlan::print()
{
    std::cout <<"\nOutput:"<<std::endl;
    for (auto &mar : _explorers)
    {
        mar.print_pos();
    }
    std::cout <<"\npaths: \n";
    print_paths();
}    
