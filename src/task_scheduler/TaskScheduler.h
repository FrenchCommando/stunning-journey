//
// Created by frenchcommando on 22/06/2019.
//

#ifndef STUNNING_JOURNEY_TASK_SCHEDULER_H
#define STUNNING_JOURNEY_TASK_SCHEDULER_H

#include <set>
#include <map>


struct Task{
    int deadline;
    int minutes;
    bool operator<(const Task& other) const{
        if (this->deadline == other.deadline)
            return this->minutes < other.minutes;
        return this->deadline < other.deadline;
    }
    bool operator==(const Task& other) const{
        return this->deadline == other.deadline and this->minutes == other.minutes;
    }
}; // represents a task

struct TaskDupli{
    int deadline;
    int minutes;
    int index;
    bool operator<(const TaskDupli& other) const{
        if (this->deadline == other.deadline and this->minutes == other.minutes)
            return this->index < other.index;
        if (this->deadline == other.deadline)
            return this->minutes < other.minutes;
        return this->deadline < other.deadline;
    }
    bool operator==(const TaskDupli& other) const{
        return this->deadline == other.deadline and this->minutes == other.minutes and this->index == other.index;
    }
}; // represents a task but is unique

class TaskScheduler {
    // overshoot at zero while (sum(minutes) <= min(deadlines))

    // new identical task ranked before or after previous ones
    // -> either fewer termination+map value to add
    // -> or potentially more candidates in arg_max

    std::map<Task, int> dupli; // always accurate
    std::set<TaskDupli> tasks; // always accurate

    bool built = false; // simple mode when overshoot == 0

    // zero overshoot case
    int sum_minutes = 0;
    int min_deadline = std::numeric_limits<int>::max();

    // general case
    std::set<TaskDupli> arg_max; // always accurate
    int overshoot = 0; // I don't like max in prefix of variables - always accurate
    std::map<TaskDupli, int> termination_map;
    std::set<TaskDupli> buffer; // values that are not included in termination_map
public:
    TaskScheduler() = default;

    int estimate_termination(const TaskDupli& t) { // not const because adds to buffer
        int termination = t.minutes;
        // sum of minutes of buffer before t
        if(not buffer.empty()){
            const auto it0 = buffer.lower_bound(t);
            for(auto it = buffer.cbegin(); it != it0; it++){
                termination += it->minutes;
            }
            buffer.emplace_hint(it0, t);
        }
        // + value of previous termination_map
        if (not termination_map.empty()){
            const auto it1 = termination_map.lower_bound(t);
            if(it1 != termination_map.cend() and it1->first < t)
                termination += it1->second;
        }
        return termination;
    }

    int add_task(const int deadline, const int minutes){
        int index = dupli[Task{deadline, minutes}]++;
        TaskDupli td{deadline, minutes, index};
        tasks.emplace(td);

        if(not built){
            sum_minutes += minutes;
            if(deadline < min_deadline)
                min_deadline = deadline;
            if(min_deadline >= sum_minutes)
                return 0;
        }

        int buffer_termination = estimate_termination(td);
        int buffer_overshoot = buffer_termination - deadline;
        if (built and buffer_overshoot > overshoot + minutes){
            overshoot = buffer_overshoot;
            arg_max = {td};
        } // current task becomes sole arg_max
        else if (built and buffer_overshoot == overshoot + minutes){
            overshoot = buffer_overshoot;
            arg_max.emplace(td);
        } // current task is one of the max
        else{
            built = true;
            auto it = arg_max.upper_bound(td);
            if (arg_max.empty() or it == arg_max.end()){
                termination_map.clear();
                buffer.clear();
                std::map<int, std::set<TaskDupli>> overshoot_map;
                int termination = 0;
                for(const auto& t: tasks){
                    termination += t.minutes;
                    termination_map[t] = termination;
                    int current_overshoot = termination - t.deadline;
                    if (current_overshoot >= overshoot) {
                        overshoot = current_overshoot;
                        overshoot_map[current_overshoot].emplace(t);
                    }
                }
                arg_max = std::move(overshoot_map[overshoot]);
            } // reset everything - problem when overshoot is zero
            else{
                std::set<TaskDupli> s(it, arg_max.cend());
                arg_max = s;
                overshoot += minutes;
            }
        } // new max is a subset of old max

        return overshoot;
    }
};


#endif //STUNNING_JOURNEY_TASK_SCHEDULER_H
