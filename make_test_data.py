

import sys,os,ctypes,json,codecs,subprocess,random


def LoadJson(file_path):
    with codecs.open(file_path,'r','utf8') as f:
        data = json.load(f,)
    return data

def SaveJson(file_path,data):
    with codecs.open(file_path,'w',"utf8") as json_file:
       json.dump(data, json_file,ensure_ascii=False,indent=1)



def vehicles():
    vehicles_size = 5
    on_work_time_range = [300, 480]
    off_work_time_range = [1020, 1320]

    ret = []
    for _ in range(vehicles_size):
        ret.append({
            "code": 'V-{:03}'.format(_),
            "on_work_time": random.randint(on_work_time_range[0], on_work_time_range[1]),
            "off_work_time": random.randint(off_work_time_range[0], off_work_time_range[1]),
        })
    
    return ret



bali = [25.149837552461616, 121.40405342515518]
lake = [24.939932971435937, 121.63988127252611]

orders_size = 80
start_time_range = [360, 1020]
duration_time_range = [40, 100]


ave_latitude = (bali[0] + lake[0]) / 2
ave_longitude = (lake[1] + bali[1]) / 2
sigma_lat = (bali[0] - lake[0]) / 4
sigma_long = (lake[1] - bali[1]) / 4

def orders():

    ret = []
    for _ in range(orders_size):
        start_time = random.randint(start_time_range[0], start_time_range[1])
        ret.append({
            "code":  'O-{:04}'.format(_),
            "start_time": start_time,
            "end_time": start_time + random.randint(duration_time_range[0], duration_time_range[1]),
            "direct_location": {
                "latitude": random.lognormvariate(0, sigma_lat) + ave_latitude -1,
                "longitude": random.lognormvariate(0, sigma_long) + ave_longitude -1
            },
            "delivery_location": {
                "latitude": random.lognormvariate(0, sigma_lat) + ave_latitude -1,
                "longitude": random.lognormvariate(0, sigma_long) + ave_longitude -1
            }
        })

    return ret


def strategy():
    ret = {
        "first_solution_strategy": 8,
        "metaheuristic": 3,
        "time_limit": 30
    }
    return ret

def main():

    ret = {
        "vehicles": vehicles(),
        "orders": orders(),
        "strategy": strategy()
    }

    SaveJson('./test_data/input/parameter.json',ret)



if __name__ == '__main__':
    main()