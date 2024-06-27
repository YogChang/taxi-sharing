

import sys,os,ctypes,json,codecs,subprocess,random,functools
from geolib import geohash
from itertools import product

def LoadJson(file_path):
    with codecs.open(file_path,'r','utf8') as f:
        data = json.load(f,)
    return data

def SaveJson(file_path,data):
    with codecs.open(file_path,'w',"utf8") as json_file:
       json.dump(data, json_file,ensure_ascii=False,indent=1)



def vehicles():
    vehicles_size = 8
    on_work_time_range = [240, 420]
    off_work_time_range = [1020, 1320]
    capacity_range = [3, 6]

    return [
        {
            "code": 'V-{:03}'.format(index),
            "on_work_time": random.randint(on_work_time_range[0], on_work_time_range[1]),
            "off_work_time": random.randint(off_work_time_range[0], off_work_time_range[1]),
            "capacity":  random.randint(capacity_range[0], capacity_range[1])
        } for index in range(vehicles_size)
    ]



def orders(locations):
    orders_size = 50
    coor_a = [25.149837552461616, 121.40405342515518]
    coor_b = [24.939932971435937, 121.63988127252611]

    start_time_range = [360, 1020]
    duration_time_range = [60, 100]
    headcount_range = [1, 3]


    ave = lambda coor_a,coor_b,i : (coor_a[i] + coor_b[i]) / 2
    sigma = lambda coor_a,coor_b,i : abs(coor_a[i] - coor_b[i]) / 4

    random_lat = lambda : random.lognormvariate(0, sigma(coor_a,coor_b,0)) + ave(coor_a,coor_b,0) -1
    random_long = lambda : random.lognormvariate(0, sigma(coor_a,coor_b,1)) + ave(coor_a,coor_b,1) -1

    def random_location(latitude,longitude):
        location = {
            "code": geohash.encode(latitude, longitude, precision=12),
            "latitude": latitude,
            "longitude": longitude
        }
        locations.append(location)

        return location

    start_time_list = [random.randint(start_time_range[0], start_time_range[1]) for index in range(orders_size)]
    random_add_duration_time = lambda start_time : start_time + random.randint(duration_time_range[0], duration_time_range[1])


    return [
        {
            "code":  'O-{:04}'.format(i),
            "start_time": start_time_list[i],
            "end_time": random_add_duration_time(start_time_list[i]),
            "headcount": random.randint(headcount_range[0], headcount_range[1]),
            "direct_location": random_location(random_lat(),random_long()),
            "delivery_location": random_location(random_lat(),random_long())
        } for i in range(len(start_time_list))
    ]


def strategy():
    return {
        "first_solution_strategy": 8,
        "metaheuristic": 3,
        "time_limit": 15,
        "solution_limit": -1
    }


if __name__ == '__main__':

    locations = []

    ret = {
        "vehicles": vehicles(),
        "orders": orders(locations),
        "strategy": strategy(),
        "locations":locations
    }

    SaveJson('./test_data/input/parameter.json',ret)
    print("make test data done")