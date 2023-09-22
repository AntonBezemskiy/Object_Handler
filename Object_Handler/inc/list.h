#pragma once

#include <iostream>
#include <functional>
#include <map>
#include <set>
#include <memory>
#include <cmath>
#include <cassert>
#include <random>

#include "structures.h"

enum TypeOfGroups {Name, Distance, Type, Time};


struct cmp {
    bool operator () (const std::shared_ptr<Object>& l, const std::shared_ptr<Object>& r) const{
        return *l.get() < *r.get();
    }
};

class Group{
private:
    using ListOfGroups = std::map<std::string, std::vector<std::weak_ptr<Object>>>;
    ListOfGroups groups;
    using ListOfObjects = std::set<std::shared_ptr<Object>, cmp>;
    ListOfObjects objects;
    std::map<std::string, int> check_exist_group;


    void MakeGroupByDistance();
    void MakeGroupByTime();
    void MakeGroupByName();
    void MakeGroupByType(int size_group_by_type);

    void RelocateObject(const std::string& type);
    void EraseEmptyElements();
public:
    void AddObject(const Object& object);
    void MakeGroups(const int type_group, int size_group_by_type);
    const ListOfGroups& GetGroups() const{
         return groups;
    }
};

class List{
public:
    void AddObject(const Object& object);
    void MakeGroups(const int type_group, int size_group_by_type = 5);
    const std::map<std::string, std::vector<std::weak_ptr<Object>>>& GetGroups() const;
private:
    Group group;
};
