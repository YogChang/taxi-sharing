from flask import Flask, render_template, request, jsonify
from flask_sqlalchemy import SQLAlchemy
from geoalchemy2 import Geometry
from shapely.geometry import Point
from itertools import product
import os
import ctypes
import json
import urllib.parse
import time
import requests, functools



app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'postgresql://user:password@db:5432/your_database_name'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
db = SQLAlchemy(app)


class Route(db.Model):
    __tablename__ = 'routes'
    id = db.Column(db.Integer, primary_key=True)
    code = db.Column(db.String(100))
    distance = db.Column(db.Integer)
    time = db.Column(db.Integer)

    def to_dict(self):
        return {
            'id': self.id,
            'code': self.code,
            'distance': self.distance,
            'time': self.time
            # 'location': [self.location.x, self.location.y] if self.location else None,
        }

db.create_all()



@app.route('/')
def hello_world():
    return render_template('search.html')

@app.route('/cautare', methods=['POST'])
def get_val():
    val = request.form.get("Titlul_cartii")
    if val is None:
        return "No data received", 400
    return authors_recommendations(val)

@app.route('/api/routes', methods=['POST'])
def add_route():
    data = request.json
    routes = Route(code=data['code'], distance=data['distance'], time=data['time'])
    db.session.add(routes)
    db.session.commit()
    return jsonify(routes.to_dict()), 201

@app.route('/api/routes', methods=['GET'])
def get_routes():
    routes = Route.query.all()
    return jsonify([route.to_dict() for route in routes])



def authors_recommendations(input_data):
    start_time = time.time()

    # Get routes data
    j_data = json.loads(input_data)
    route_data = get_routes(j_data["locations"])

    j_data['routes'] = route_data
    
    # Mock result for testing purpose
    result_data = RunPyDispatch(j_data)
    
    # Simulate long computation time (remove this in production)
    time.sleep(5)  # Remove this line if you have an actual long computation
    
    end_time = time.time()
    elapsed_time = end_time - start_time

    if result_data.get("error"):
        return f"Error: {result_data['error']}"
    # 将 data 转换为 JSON 字符串
    data_json = json.dumps(result_data)
    
    # 对 JSON 字符串进行 URL 编码
    data_encoded = urllib.parse.quote(data_json)
    
    # 创建超链接
    ret = f"<a href='http://geojson.io/#data=data:application/json,{data_encoded}'> show result on http://geojson.io </a>"
    ret += f"<br>Calculation time: {elapsed_time:.2f} seconds"
    
    return ret


def get_osrm_routes(locations):

    coordinates_str = [ '{},{};'.format(ele['longitude'],ele['latitude']) for ele in locations ]
    url = 'http://router.project-osrm.org/table/v1/driving/' + functools.reduce(lambda a, b: a+b, coordinates_str)[:-1] + '?annotations=distance,duration'

    try:
        response = requests.get(url)

        if response.status_code == 200:
            posts = response.json()
            return posts
        else:
            print('Error:', response.status_code)
            return None
    except requests.exceptions.RequestException as e:
        print('Error:', e)
        return None

def get_routes(locations):
    separateSymbol = "||"
    posts = get_osrm_routes(locations)

    if posts:
        return [
            {
                "code": locations[from_i]["code"] + separateSymbol + locations[to_i]["code"],
                "distance": int(posts['distances'][from_i][to_i]),
                "time": int(posts['durations'][from_i][to_i] / 60)
            } for from_i, to_i in product(range(len(locations)), range(len(locations)))
        ]

    # make fake routes data
    else:
        print('Failed to fetch posts from API.')
        distance_coeff = 120000
        time_coeff = 150

        lat_lng_sum = lambda from_location,to_location : abs(from_location["longitude"] - to_location["longitude"]) + abs(from_location["longitude"] - to_location["longitude"])
        result_num = lambda abs_sum,coeff : max(int(abs_sum * coeff), 1)

        return [
            {
                "code": from_location["code"] + separateSymbol + to_location["code"],
                "distance": result_num(lat_lng_sum(from_location,to_location),distance_coeff),
                "time": result_num(lat_lng_sum(from_location,to_location),time_coeff)
            } for from_location, to_location in product(locations, locations)
        ]


def RunPyDispatch(input_data, THIS_SO_PATH="./lib/", AB_SO_PATH="/usr/local/lib"):
    ortools_lib = ctypes.cdll.LoadLibrary(os.path.join(AB_SO_PATH, "libortools.so.9")) # libortools 通常在 usr/local/lib
    project_lib = ctypes.cdll.LoadLibrary(os.path.join(THIS_SO_PATH, "libsharing_manager.so")) # 自己專案編譯的 .so

    # ctypes 型別 const char*(const char*)
    project_lib.PyDispatch.argtypes = [ctypes.c_char_p]
    project_lib.PyDispatch.restype = ctypes.c_char_p
    print("start Dispatch ...")
    
    # 确保 input_data 为字符串
    if isinstance(input_data, str):
        bytes_data = input_data.encode("utf-8")
    else:
        bytes_data = json.dumps(input_data).encode("utf-8")

    # PyDispatch = C++ 函式名:  json_obj --> string --> json_bytes ---> C++ ---> bytes --> string ----> json_obj
    result = project_lib.PyDispatch(bytes_data)
    decoded_result = result.decode("utf-8")
    return json.loads(decoded_result)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
