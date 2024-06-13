from flask import Flask, render_template, request
import os
import ctypes
import json
import urllib.parse
import time

app = Flask(__name__)

@app.route('/')
def hello_world():
    return render_template('search.html')

@app.route('/cautare', methods=['POST'])
def get_val():
    val = request.form.get("Titlul_cartii")
    if val is None:
        return "No data received", 400
    return authors_recommendations(val)

def authors_recommendations(input_data):
    start_time = time.time()

    # Mock result for testing purpose
    result_data = RunPyDispatch(input_data)
    
    # Simulate long computation time (remove this in production)
    time.sleep(5)  # Remove this line if you have an actual long computation
    
    end_time = time.time()
    elapsed_time = end_time - start_time

    if result_data.get("error"):
        return f"Error: {result_data['error']}"
    # 将 data 转换为 JSON 字符串
    data_json = json.dumps(result_data['geo_json'])
    
    # 对 JSON 字符串进行 URL 编码
    data_encoded = urllib.parse.quote(data_json)
    
    # 创建超链接
    ret = f"<a href='http://geojson.io/#data=data:application/json,{data_encoded}'> show result on http://geojson.io </a>"
    ret += f"<br>Calculation time: {elapsed_time:.2f} seconds"
    
    return ret

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
