
#include "test.h"

#include <cassert>
#include <iostream>

namespace test{

std::map<std::string, std::vector<Object>> RemoveWeakPtr(const std::map<std::string, std::vector<std::weak_ptr<Object>>>& groups){
    std::map<std::string, std::vector<Object>> result;
    for(const auto& [type, objects]: groups){
        for(const auto& object: objects){
            if(auto sh_object = object.lock(); sh_object){
                result[type].push_back(*sh_object.get());
            }
            else {
                throw std::invalid_argument("CompareByDistance: weak_ptr is expired");
            }

        }
    }
    return result;
}

void AddObject(List& list, const std::vector<Object>& objects){
    for(const auto& object: objects){
        list.AddObject(object);
    }
}

void Test_Object(){
    {
        Object l = {"Кривой", -37.23, 13.44, "Человек", 1693235249.98678};
        Object r = {"Кривой", -37.23, 13.44, "Человек", 1693235249.98678};
        assert(l == r);
    }
    {
        Object l = {"Абвгдежз", -37.23, 13.44, "Человек", 1693235249.98678};
        Object r = {"Абвгдежи", -37.23, 13.44, "Человек", 1693235249.98678};
        assert(l < r);
    }
    {
        Object l = {"a", -37.23, 13.44, "Человек", 1693235249.98678};
        Object r = {"aaa", -37.23, 13.44, "Человек", 1693235249.98678};
        assert(l < r);
    }
}

void Test_Unique_Object(){
    {
        Object r1 = {"Кривой", -37.23, 13.44, "Человек", 1693235249.98678};
        Object r2 = {"Кривой", -37.23, 13.44, "Человек", 1693235249.98678};
        Object r3 = {"Кривой", -37.23, 13.44, "Человек", 1693235249.98678};
        std::set<Object> list = {r1,r2,r3};
        std::set<Object> result = {r1};
        assert(list == result);
    }
    {
        Object r1 = {"Кривой", -37.23, 13.44, "Человек", 1693235249.98678};
        Object r2 = {"Кривой", -37.23, 13.444, "Человек", 1693235249.98678};
        std::set<Object> list = {r1,r2};
        std::set<Object> result = {r1,r2};
        assert(list == result);
    }
    {
        List list;
        Object r1 = {"Кривой", -37.23, 13.44, "Человек", 1693235249.98678};
        Object r2 = {"Кривой", -37.23, 13.44, "Человек", 1693235249.98678};
        Object r3 = {"Кривой", -37.23, 13.44, "Человек", 1693235249.98678};
        list.AddObject(r1);
        list.AddObject(r2);
        list.AddObject(r3);
        list.MakeGroups(Name);
        const auto& get = list.GetGroups();
        std::map<std::string, std::vector<Object>> result;
        result["К"].push_back(r1);

        std::map<std::string, std::vector<Object>> get_ = RemoveWeakPtr(get);

        assert(get_ == result);
    }
    {
        List list;
        Object r1 = {"Кривой", -37.23, 13.44, "Человек", 1693235249.98678};
        Object r2 = {"Кривой", -37.23, 13.441, "Человек", 1693235249.98678};
        list.AddObject(r1);
        list.AddObject(r2);
        list.MakeGroups(Name);
        const auto& get = list.GetGroups();
        std::map<std::string, std::vector<Object>> result = {{"К", {r1,r2}}};

        std::map<std::string, std::vector<Object>> get_ = RemoveWeakPtr(get);

        assert(get_ == result);
    }
}

void Test_MakeGroupByDistance(){
    {
        List list;
        Object DD3 = {"Кривой", -0.34, -0.7, "Человек", 1693235249.98678};
        Object D1 = {"Кривой", 21.2, 21.2, "Человек", 1693235249.98678};
        Object D2 = {"Кривой", 21.2, 21.201, "Человек", 1693235249.98678};
        Object DD1 = {"Кривой", -46.17, 21.8, "Человек", 1693235249.98678};
        Object D3 = {"Кривой", 50.2, 21.2, "Человек", 1693235249.98678};
        Object D4 = {"Кривой", 21.2, 50.22, "Человек", 1693235249.98678};
        Object DD2 = {"Кривой", 54.53, -30, "Человек", 1693235249.98678};
        Object D5 = {"Кривой", 70.710679, 70.710679, "Человек", 1693235249.98678};
        Object D6 = {"Кривой", 72.15, 80.14, "Человек", 1693235249.98678};
        Object D7 = {"Кривой", 72.151, 80.14, "Человек", 1693235249.98678};
        Object D8 = {"Кривой", 90.2, 86.73, "Человек", 1693235249.98678};
        Object D9 = {"Кривой", 707.10679, 707.10679, "Человек", 1693235249.98678};
        Object D10 = {"Кривой", 821.14, 954.29, "Человек", 1693235249.98678};
        Object D11 = {"Кривой", 7071.06782, 7071.06782, "Человек", 1693235249.98678};
        Object D12 = {"Кривой", 7871.06781, 9071.06781, "Человек", 1693235249.98678};

        std::vector<Object> objects = {D4,D1,D2,DD1,DD2,D5,D6,D7,DD3,D8,D3,D9,D10,D12,D11};
        AddObject(list, objects);

        list.MakeGroups(Distance);
        const auto& get = list.GetGroups();
        std::map<std::string, std::vector<Object>> result = { {"UP_TO_100",{DD3,D1,D2,DD1,D3,D4,DD2}},
                                                              {"UP_TO_1000",{D5,D6,D7,D8}},
                                                              {"UP_TO_10000",{D9,D10}},
                                                              {"TOO_FAR", {D11,D12}}};

        std::map<std::string, std::vector<Object>> get_ = RemoveWeakPtr(get);

        assert(get_ == result);
    }
}

void Test_MakeGroupByTime(){
    {
        time_t rawtime;
        double current_time = static_cast<double>(time( &rawtime ));

        Object D1 = {"Кривой", 21.2, 21.2, "Человек", current_time - 3600.12};
        Object D2 = {"Кривой", 21.2, 21.2, "Человек", current_time - 36000.14};
        assert(D1 < D2);
    }
    {
        time_t rawtime;
        double current_time = static_cast<double>(time( &rawtime ));

        List list;
        Object D1 = {"Кривой", 21.2, 21.2, "Человек", current_time - 3600.12};
        Object D2 = {"Кривой", 21.2, 21.2, "Человек", current_time - 36000.14};
        Object D3 = {"Кривой", 21.2, 21.2, "Человек", current_time - 82800.16};

        Object D4 = {"Кривой", 21.2, 21.2, "Человек", current_time - 90000.18};
        Object D5 = {"Кривой", 21.2, 21.2, "Человек", current_time - 133200.567};

        Object D6 = {"Кривой", 21.2, 21.2, "Человек", current_time - 259200.22};
        Object D7 = {"Кривой", 21.2, 21.2, "Человек", current_time - 324000.26};
        Object D8 = {"Кривой", 21.2, 21.2, "Человек", current_time - 518400.689};

        Object D9 = {"Кривой", 21.2, 21.2, "Человек", current_time - 604801.28};
        Object D10 = {"Кривой", 21.2, 21.2, "Человек", current_time - 864000.144};
        Object D11 = {"Кривой", 21.2, 21.2, "Человек", current_time - 2505600.32};

        Object D12 = {"Кривой", 21.2, 21.2, "Человек", current_time - 2851200.593};
        Object D13 = {"Кривой", 21.2, 21.2, "Человек", current_time - 17280000.12};
        Object D14 = {"Кривой", 21.2, 21.2, "Человек", current_time - 31449600.36};

        Object D15 = {"Кривой", 21.2, 21.2, "Человек", current_time - 31622400.38};
        Object D16 = {"Кривой", 21.2, 21.2, "Человек", current_time - 60480000.42};
        Object D17 = {"Кривой", 21.2, 21.2, "Человек", current_time - 103680000.89};

        std::vector<Object> objects = {D8, D2,D3,D4,D15,D5,D6,D9,D10,D11,D12,D7,D13,D16,D1,D14,D17};
        AddObject(list, objects);

        list.MakeGroups(Time);
        const auto& get = list.GetGroups();
        std::map<std::string, std::vector<Object>> result = { {"Today",{D1,D2,D3}},
                                                              {"Yesterday",{D4,D5}},
                                                              {"This_week",{D6,D7,D8}},
                                                              {"This_month", {D9,D10,D11}},
                                                              {"This_year", {D12,D13,D14}},
                                                              {"Previously", {D15,D16,D17}}};

        std::map<std::string, std::vector<Object>> get_ = RemoveWeakPtr(get);

        assert(get_ == result);
    }
}

void Test_MakeGroupByName(){
    {
        List list;
        Object D1 = {"А", 0, 0, "", 0};
        Object D2 = {"Аа", 0, 0, "", 0};
        Object D3 = {"Аааб", 0, 0, "", 0};
        Object D4 = {"Аааба", 0, 0, "", 0};

        Object D5 = {"б", 0, 0, "", 0};

        Object D6 = {"рбст", 0, 0, "", 0};
        Object D7 = {"рпст", 0, 0, "", 0};
        Object D8 = {"рюст", 0, 0, "", 0};
        Object D9 = {"ряст", 0, 0, "", 0};

        Object D10 = {"с", 0, 0, "", 0};
        Object D11 = {"са", 0, 0, "", 0};
        Object D12 = {"ся", 0, 0, "", 0};
        Object D13 = {"сяя", 0, 0, "", 0};

        Object D14 = {"5Ааа", 0, 0, "", 0};
        Object D15 = {"!фыв", 0, 0, "", 0};
        Object D16 = {"Sцыв", 0, 0, "", 0};
        Object D17 = {"9", 0, 0, "", 0};

        std::vector<Object> objects = {D5,D13,D1,D3,D6,D2,D7,D8,D10,D11,D12,D16,D9,D14,D15,D17,D4};
        AddObject(list, objects);

        list.MakeGroups(Name);
        const auto& get = list.GetGroups();
        std::map<std::string, std::vector<Object>> result = { {"А",{D1,D2,D3,D4}},
                                                              {"б",{D5}},
                                                              {"р",{D6,D7,D8,D9}},
                                                              {"с", {D10,D11,D12,D13}},
                                                              {"#", {D15,D14,D17,D16}}};

        std::map<std::string, std::vector<Object>> get_ = RemoveWeakPtr(get);

        assert(get_ == result);
    }
}

void Test_MakeGroupByType(){
    {
        List list;
        Object D1 = {"А", 0, 0, "Машина", 0};
        Object D2 = {"Аа", 0, 0, "Машина", 0};
        Object D3 = {"Ааа", 0, 0, "Машина", 0};
        Object D4 = {"ББ", 0, 0, "Машина", 0};
        Object D5 = {"Бя", 0, 0, "Машина", 0};

        Object D6 = {"Больница", 0, 0, "Здание", 0};
        Object D7 = {"Дом", 0, 0, "Здание", 0};
        Object D8 = {"Таверна", 0, 0, "Здание", 0};
        Object D9 = {"Театр", 0, 0, "Здание", 0};
        Object D10 = {"Школа", 0, 0, "Здание", 0};

        Object D11 = {"Береза", 0, 0, "Дерево", 0};
        Object D12 = {"Дуб", 0, 0, "Дерево", 0};
        Object D13 = {"Ольха", 0, 0, "Дерево", 0};
        Object D14 = {"Ясень", 0, 0, "Дерево", 0};

        std::vector<Object> objects = {D5,D13,D1,D3,D6,D2,D7,D8,D10,D11,D12,D9,D14,D4};
        AddObject(list, objects);

        list.MakeGroups(Type, 5);
        const auto& get = list.GetGroups();
        std::map<std::string, std::vector<Object>> result = { {"Miscellaneous",{D11,D12,D13,D14}},
                                                              {"Здание",{D6,D7,D8,D9,D10}},
                                                              {"Машина",{D1,D2,D3,D4,D5}}
                                                              };

        std::map<std::string, std::vector<Object>> get_ = RemoveWeakPtr(get);

        assert(get_ == result);
    }
}

void Test(){
    Test_Object();
    Test_Unique_Object();
    Test_MakeGroupByDistance();
    Test_MakeGroupByTime();
    Test_MakeGroupByName();
    Test_MakeGroupByType();

    std::cout << ' ' << "Test is OK" << '\n';
}

} // end namespace test


