

import sys,os,ctypes,json,codecs,subprocess,random

def LoadJson(file_path):
    with codecs.open(file_path,'r','utf8') as f:
        data = json.load(f,)
    return data

def SaveJson(file_path,data):
    with codecs.open(file_path,'w',"utf8") as json_file:
       json.dump(data, json_file,ensure_ascii=False,indent=1)



def vehicles():
    vehicles_size = 20
    on_work_time_range = [360, 480]
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
def orders():
    orders_size = 100
    start_time_range = [360, 1020]
    duration_time_range = [10, 300]
    latitude_range = [2493993297143593, 2514983755246161]
    longitude_range = [12140405342515518, 12163988127252611]

    ret = []
    for _ in range(orders_size):
        start_time = random.randint(start_time_range[0], start_time_range[1])
        ret.append({
            "code":  'O-{:04}'.format(_),
            "start_time": start_time,
            "end_time": start_time + random.randint(duration_time_range[0], duration_time_range[1]),
            "direct_location": {
                "latitude": random.randint(latitude_range[0], latitude_range[1]),
                "longitude": random.randint(longitude_range[0], longitude_range[1])
            },
            "delivery_location": {
                "latitude": random.randint(latitude_range[0], latitude_range[1]),
                "longitude": random.randint(longitude_range[0], longitude_range[1])
            }
        })

    return ret


def strategy():
    ret = {
        "first_solution_strategy": 8,
        "metaheuristic": 3,
        "time_limit": 100
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