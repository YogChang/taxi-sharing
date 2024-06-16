

import sys,os,ctypes,json,codecs,subprocess,random


def LoadJson(file_path):
    with codecs.open(file_path,'r','utf8') as f:
        data = json.load(f,)
    return data

def SaveJson(file_path,data):
    with codecs.open(file_path,'w',"utf8") as json_file:
       json.dump(data, json_file,ensure_ascii=False,indent=1)


def vehicles():
    vehicles_size = 6
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



def orders():
    orders_size = 80
    bali = [25.149837552461616, 121.40405342515518]
    lake = [24.939932971435937, 121.63988127252611]

    start_time_range = [360, 1020]
    duration_time_range = [40, 100]
    headcount_range = [1, 3]


    ave_latitude = (bali[0] + lake[0]) / 2
    ave_longitude = (lake[1] + bali[1]) / 2
    sigma_lat = (bali[0] - lake[0]) / 4
    sigma_long = (lake[1] - bali[1]) / 4

    start_time_list = [random.randint(start_time_range[0], start_time_range[1]) for index in range(orders_size)]

    return [
        {
            "code":  'O-{:04}'.format(i),
            "start_time": val,
            "end_time": val + random.randint(duration_time_range[0], duration_time_range[1]),
            "headcount": random.randint(headcount_range[0], headcount_range[1]),
            "direct_location": {
                "latitude": random.lognormvariate(0, sigma_lat) + ave_latitude -1,
                "longitude": random.lognormvariate(0, sigma_long) + ave_longitude -1
            },
            "delivery_location": {
                "latitude": random.lognormvariate(0, sigma_lat) + ave_latitude -1,
                "longitude": random.lognormvariate(0, sigma_long) + ave_longitude -1
            }
        } for i, val in enumerate(start_time_list)
    ]



def strategy():
    return {
        "first_solution_strategy": 8,
        "metaheuristic": 3,
        "time_limit": 30
    }


if __name__ == '__main__':
    ret = {
        "vehicles": vehicles(),
        "orders": orders(),
        "strategy": strategy()
    }

    SaveJson('./test_data/input/parameter.json',ret)
    print("make test data done")