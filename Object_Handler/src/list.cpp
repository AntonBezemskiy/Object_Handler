#include "list.h"

const static int TODAY = 86400;
const static int YESTERDAY = 172800;
const static int THISWEEK = 604800;
const static int THISMONTH= 2629743;
const static int THISYEAR= 31556926;

const static std::set<std::string> Сyrillic = {"А","а","Б","б","В","в","Г","г","Д","д","Е","е","Ё","ё","Ж","ж","З","з","И","и"
                                              ,"Й","й","К","к","Л","л","М","м","Н","н","О","о","П","п","Р","р","С","с","Т","т","У","у"
                                              ,"Ф","ф","Х","х","Ц","ц","Ч","ч","Ш","ш","Щ","щ","Ъ","ъ","Ы","ы","Ь","ь","Э","э"
                                              ,"Ю","ю","Я","я"};

bool IsСyrillic(std::string letter){
    return Сyrillic.count(letter);
}

double GetDitance(double x, double y){
    double x_sqw = x * x;
    double y_sqw = y * y;
    return std::sqrt(x_sqw + y_sqw);
}

void Group::EraseEmptyElements(){
    std::vector<std::string> to_insert;
    for(const auto& [group, objects]: groups){
        if(groups[group].size() == 0){
            to_insert.push_back(group);
        }
    }
    for(const auto& name_group: to_insert){
        groups.erase(name_group);
    }
}

bool CompareByName(const std::weak_ptr<Object> l, const std::weak_ptr<Object> r){
    if(auto sh_l = l.lock(), sh_r = r.lock(); sh_l && sh_r){
        return  std::lexicographical_compare(sh_l->name.begin(), sh_l->name.end(), sh_r->name.begin(), sh_r->name.end());
    }
    else {
        throw std::invalid_argument("CompareByName: weak_ptr is expired");
    }
}

bool CompareByDistance(const std::weak_ptr<Object> l, const std::weak_ptr<Object> r){
    if(auto sh_l = l.lock(), sh_r = r.lock(); sh_l && sh_r){
        double l_dist = GetDitance(sh_l->x, sh_l->y);
        double r_dist = GetDitance(sh_r->x, sh_r->y);
        return l_dist < r_dist;
    }
    else {
        throw std::invalid_argument("CompareByDistance: weak_ptr is expired");
    }
}

bool CompareByTime(const std::weak_ptr<Object> l, const std::weak_ptr<Object> r){
    if(auto sh_l = l.lock(), sh_r = r.lock(); sh_l && sh_r){
        return sh_l->time > sh_r->time;
    }
    else {
        throw std::invalid_argument("CompareByDistance: weak_ptr is expired");
    }
}



template <typename It, typename Comp>
void SortObjectInGroup(It begin, It end, Comp comp){
    std::sort(begin, end, comp);
}


void Group::AddObject(const Object& object){
    objects.insert(std::make_shared<Object>(object));
}

void Group::MakeGroupByName(){
    groups.clear();
    groups["#"];

    for(const auto& object: objects){
        std::string first_later = object->name.substr(0,2);
        if(!IsСyrillic(first_later)){
            groups["#"].push_back(object);
            continue;
        }
        groups[first_later].push_back(object);
    }

    if(groups["#"].size() == 0){
        groups.erase("#");
    }

    for(auto& [group, objects]: groups){
        SortObjectInGroup(objects.begin(), objects.end(), CompareByName);
    }
}

void Group::MakeGroupByDistance(){
    groups.clear();

    groups["UP_TO_100"];
    groups["UP_TO_1000"];
    groups["UP_TO_10000"];
    groups["TOO_FAR"];

    for(const auto& object: objects){
        assert(object);
        double distance = GetDitance(object->x, object->y);
        if(distance < 100){
            groups.at("UP_TO_100").push_back(object);
        }else if(IsEqual(distance, 100) || (distance > 100 && distance < 1000)){
            groups.at("UP_TO_1000").push_back(object);
        }else if(IsEqual(distance, 1000) || (distance > 1000 && distance < 10000)){
            groups.at("UP_TO_10000").push_back(object);
        }else if(distance > 10000 || IsEqual(distance, 10000)){
            groups.at("TOO_FAR").push_back(object);
        }
    }
    Group::EraseEmptyElements();

    for(auto& [group, objects]: groups){
        SortObjectInGroup(objects.begin(), objects.end(), CompareByDistance);
    }
}

void Group::RelocateObject(const std::string& type){
    groups[type];

    for(auto begin = groups["Miscellaneous"].begin(); begin < groups["Miscellaneous"].end(); ++begin){
        if(auto sh_object = (*begin).lock(); sh_object){
            if(sh_object->type != type){
                continue;
            }
            groups[type].push_back(*begin);
            groups["Miscellaneous"].erase(begin);
            --begin;
        }
        else {
            throw std::invalid_argument("RelocateObject: weak_ptr is expired");
        }
    }
}

void Group::MakeGroupByType(int size_group_by_type){
    groups.clear();
    check_exist_group.clear();

    groups["Miscellaneous"];

    for(const auto& object: objects){
        if(groups.count(object->type)){
            groups.at(object->type).push_back(object);
            continue;
        }
        if(check_exist_group.count(object->type) == 0){
            check_exist_group[object->type] = 1;
            groups["Miscellaneous"].push_back(object);
            continue;
        }
        if(check_exist_group.count(object->type) && check_exist_group.at(object->type) >= size_group_by_type - 1){
            check_exist_group[object->type] += 1;
            groups["Miscellaneous"].push_back(object);
            RelocateObject(object->type);
            continue;
        }
        check_exist_group[object->type] += 1;
        groups["Miscellaneous"].push_back(object);
    }

    if(groups["Miscellaneous"].size() == 0){
        groups.erase("Miscellaneous");
    }

    for(auto& [group, objects]: groups){
        SortObjectInGroup(objects.begin(), objects.end(), CompareByName);
    }
}

void Group::MakeGroupByTime(){
    groups.clear();

    groups["Today"];
    groups["Yesterday"];
    groups["This_week"];
    groups["This_month"];
    groups["This_year"];
    groups["Previously"];

    time_t rawtime;
    int current_time = time( &rawtime );

    for(const auto& object: objects){
        assert(object);
        double time = current_time - object->time;
        if(time < TODAY){
            groups.at("Today").push_back(object);
        }else if(IsEqual(time,TODAY) || (time > TODAY && time < YESTERDAY) ){
            groups.at("Yesterday").push_back(object);
        }else if(IsEqual(time,YESTERDAY) || (time > YESTERDAY && time < THISWEEK)){
            groups.at("This_week").push_back(object);
        }else if(IsEqual(time,THISWEEK) || (time > THISWEEK && time < THISMONTH)){
            groups.at("This_month").push_back(object);
        }else if(IsEqual(time,THISMONTH) || (time > THISMONTH && time < THISYEAR)){
            groups.at("This_year").push_back(object);
        }else if(IsEqual(time,THISYEAR) || time > THISYEAR ){
            groups.at("Previously").push_back(object);
        }

    }
    Group::EraseEmptyElements();

    for(auto& [group, objects]: groups){
        if(groups[group].size() == 0){
            groups.erase(group);
            continue;
        }
        SortObjectInGroup(objects.begin(), objects.end(), CompareByTime);
    }
}

void Group::MakeGroups(const int type_group, int size_group_by_type){
    switch(type_group){
        case TypeOfGroups::Name:
            MakeGroupByName();
            break;
        case TypeOfGroups::Distance:
            MakeGroupByDistance();
            break;
        case TypeOfGroups::Type:
            MakeGroupByType(size_group_by_type);
            break;
        case TypeOfGroups::Time:
            MakeGroupByTime();
            break;

    }
}

void List::AddObject(const Object& object){
    group.AddObject(object);
}

void List::MakeGroups(const int type_group,int size_group_by_type){
    group.MakeGroups(type_group, size_group_by_type);
}

const std::map<std::string, std::vector<std::weak_ptr<Object>>>& List::GetGroups() const{
    return group.GetGroups();
}








