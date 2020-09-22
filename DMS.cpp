//
// Created by hassan on 2020-09-18.
//
#ifndef FINAL_PROJECT_DMS_H
#define FINAL_PROJECT_DMS_H

#include <string>
#include <sstream>
#include <stdexcept>

using namespace std;

enum DIRECTION{
    NORTH=2, SOUTH=0, EAST=3, WEST=1, NULL_ISLAND=4
};

struct DMS {
    private:
        DIRECTION direction;
        int degree;
        int minute;
        int second;
        std::string raw;
    public:
        explicit DMS(const std::string& dms) {
            if (dms == "Unknown") {
                direction = NULL_ISLAND;
                degree = 0;
                minute = 0;
                second = 0;
                return;
            }
            if (dms.length()!= 7 && dms.length() != 8){
                throw std::invalid_argument("DMS values are expected to be of sizes 7 or 8");
            }
            bool is_latitude(false);
            raw = dms;
            direction = EAST;
            if (dms[dms.length() - 1] == 'W'){
                direction = WEST;
            } else if (dms[dms.length() - 1] == 'S') {
                is_latitude = true;
                direction = SOUTH;
            } else if (dms[dms.length() - 1] == 'N') {
                is_latitude = true;
                direction = NORTH;
            } else if (dms[0] == '-') {
                throw std::invalid_argument("DMS values are expected to show the sign using the direction modifiers at the end");
            }
            degree = stoi(dms.substr(0, is_latitude? 2: 3));
            minute = stoi(dms.substr(is_latitude? 2: 3, 2));
            second = stoi(dms.substr(is_latitude? 4: 5, 2));
        }

        DMS() {
            direction = NULL_ISLAND;
            degree = 0;
            minute = 0;
            second = 0;
        }

        DIRECTION get_direction() const {
            return direction;
        }

        std::string get_direction_str() const {
            switch (direction) {
                case NORTH:
                    return "North";
                case SOUTH:
                    return "South";
                case WEST:
                    return "West";
                case EAST:
                    return "East";
                default:
                    return "NullIsland";
            }
        }

        bool operator==(const DMS& a) const{
            if(a.direction==direction && a.degree== degree && a.minute== minute && a.second== second)
                return true;
            else
                return false;
        }

        bool operator<(const DMS& a) const{
            if (direction % 2 == a.direction % 2){
                if (direction == a.direction) {
                    if(is_negative()){
                        return (degree > a.degree) ||(degree == a.degree && minute > a.minute)
                               || (degree == a.degree && minute == a.minute && second > a.second);
                    }else{
                        return (degree < a.degree) ||(degree == a.degree && minute < a.minute)
                               || (degree == a.degree && minute == a.minute && second < a.second);
                    }
                } else {
                    return direction < a.direction;
                }
            } else {
                throw std::invalid_argument("cannot compare DMS of type latitude with DMS of type longitude");
            }
        }

        bool operator>(const DMS& a) const{
            if (direction % 2 == a.direction % 2){
                if (direction == a.direction) {
                    if(is_negative()){
                        return (degree < a.degree) ||(degree == a.degree && minute < a.minute)
                               || (degree == a.degree && minute == a.minute && second < a.second);
                    } else {
                        return (degree > a.degree) ||(degree == a.degree && minute > a.minute)
                               || (degree == a.degree && minute == a.minute && second > a.second);
                    }
                } else {
                    return direction > a.direction;
                }
            } else {
                throw std::invalid_argument("cannot compare DMS of type latitude with DMS of type longitude");
            }
        }

        bool operator<=(const DMS& a) const{
            return *this < a || *this == a;
        }

        bool operator>=(const DMS& a) const{
            return *this > a || *this == a;
        }

        bool operator!=(const DMS& a) const{
            return  (direction != a.direction) ||
                ((abs(second - a.second) + abs(minute - a.minute) * 60 + abs(degree - a.degree) * 3600) > 1);
        }

        int total_seconds() const{
            int neg = (is_negative()? -1: 1);
            return neg * (second + minute * 60 + degree * 3600);
        }

        // This operator is reserved for averaging two DMS objects
        DMS operator%(const DMS& a) const{
            DMS result;
            if (direction % 2 == a.direction % 2){
                int s;
                if (direction == a.direction) {
                    result.direction = direction;
                    s = second + a.second + (minute + a.minute) * 60 + (degree + a.degree) * 3600;
                } else {
                    result.direction = (*this > a)? direction: a.direction;
                    s = abs(second - a.second) + abs(minute - a.minute) * 60 + abs(degree - a.degree) * 3600;
                }
                s = s / 2;
                result.degree = s / 3600;
                result.minute = (s - 3600 * result.degree) / 60;
                result.second = (s - 3600 * result.degree) % 60;
                return result;
            } else {
                throw std::invalid_argument("cannot compare DMS of type latitude with DMS of type longitude");
            }
        }

        double to_dec() const{
            double ang = degree * 1.0 + ((minute + (second / 60.0)) / 60.0) ;
            return (direction == WEST || direction == SOUTH)? -ang: ang;
        }

        bool is_negative() const{
            return (direction == WEST || direction == SOUTH);
        }

        string str() const{
            std::ostringstream os;
            //os << (is_negative()?"-":"") <<  degree << "d" << minute << "m" << second << "s";
            os <<  degree << "d " << minute << "m " << second << "s " << get_direction_str();
            return os.str();
        }
};

#endif //FINAL_PROJECT_DMS_H
